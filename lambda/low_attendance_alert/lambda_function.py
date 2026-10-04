import os
import pymysql
import boto3

# Read AWS Environment Variables
DB_HOST = os.environ.get('DB_HOST')
DB_USER = os.environ.get('DB_USER', 'root')
DB_PASSWORD = os.environ.get('DB_PASSWORD')
DB_NAME = os.environ.get('DB_NAME', 'student_management_db')
SNS_TOPIC_ARN = os.environ.get('SNS_TOPIC_ARN')

def lambda_handler(event, context):
    """
    AWS Lambda Function to scan for low-attendance students (<75%)
    and send automatic alerts via AWS Simple Notification Service (SNS).
    """
    print(f"[LAMBDA SCAN] Connecting to Amazon RDS instance at {DB_HOST}...")
    
    conn = pymysql.connect(
        host=DB_HOST,
        user=DB_USER,
        password=DB_PASSWORD,
        database=DB_NAME,
        cursorclass=pymysql.cursors.DictCursor,
        connect_timeout=5
    )

    try:
        with conn.cursor() as cursor:
            # SQL Query to calculate attendance percentage for all students
            sql = """
                SELECT 
                    s.id as student_id,
                    s.roll_number,
                    s.name,
                    u.email,
                    COUNT(a.id) as total_sessions,
                    SUM(CASE WHEN a.status = 'present' THEN 1 ELSE 0 END) as present_sessions,
                    ROUND((SUM(CASE WHEN a.status = 'present' THEN 1 ELSE 0 END) / COUNT(a.id)) * 100, 2) as attendance_percentage
                FROM students s
                JOIN users u ON s.user_id = u.id
                JOIN attendance a ON s.id = a.student_id
                GROUP BY s.id, s.roll_number, s.name, u.email
                HAVING attendance_percentage < 75.00;
            """
            cursor.execute(sql)
            low_attendance_students = cursor.fetchall()

        print(f"[LAMBDA SCAN] Found {len(low_attendance_students)} students below 75% attendance threshold.")

        sns_client = boto3.client('sns')
        alert_count = 0

        for student in low_attendance_students:
            message = (
                f"CRITICAL ATTENDANCE ALERT\n"
                f"----------------------------------------\n"
                f"Student Name : {student['name']}\n"
                f"Roll Number  : {student['roll_number']}\n"
                f"Email        : {student['email']}\n"
                f"Attendance   : {student['attendance_percentage']}%\n"
                f"Threshold    : 75.00%\n"
                f"----------------------------------------\n"
                f"Action Required: Please report to your department head immediately."
            )
            subject = f"Low Attendance Warning - {student['roll_number']}"

            if SNS_TOPIC_ARN:
                sns_client.publish(
                    TopicArn=SNS_TOPIC_ARN,
                    Subject=subject,
                    Message=message
                )
                print(f"[SNS PUBLISHED] Sent alert for {student['roll_number']} ({student['attendance_percentage']}%)")
            else:
                print(f"[MOCK ALERT] {subject}: {student['name']} is at {student['attendance_percentage']}%")
            
            alert_count += 1

        return {
            'statusCode': 200,
            'body': f"Scan Complete. Alerts published for {alert_count} students."
        }

    except Exception as e:
        print(f"[LAMBDA ERROR] Failed to execute scan: {str(e)}")
        raise e
    finally:
        conn.close()
