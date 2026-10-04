# Live Viva Demo Script (5-7 Minutes)

## ⏱️ Step-by-Step Live Demo Roadmap

### Minute 1: AWS Console Infrastructure Tour
1. Open the [AWS Console (eu-north-1)](https://eu-north-1.console.aws.amazon.com/ec2/).
2. Show the active **Application Load Balancer (ALB)** DNS URL.
3. Show the **Auto Scaling Group** showing 2 healthy EC2 instances running across Availability Zones `eu-north-1a` and `eu-north-1b`.
4. Show **Amazon RDS MySQL** running in the private subnet.

---

### Minute 2: Administrator Flow
1. Open the live web application in browser using the ALB DNS URL.
2. Click **Quick Demo Login -> Admin** (`admin@university.edu` / `Admin@123`).
3. Show the Admin Dashboard KPI cards (Total Faculty, Students, Subjects, AWS Region).
4. Navigate to **Manage Users** and add a demo Faculty member.
5. Navigate to **Manage Subjects** and assign a subject to the new faculty member.
6. Click **Logout**.

---

### Minute 3: Faculty Flow & S3 Upload
1. Click **Quick Demo Login -> Faculty** (`prof.sharma@university.edu` / `Faculty@123`).
2. Navigate to **Mark Attendance** -> Select Course `CS301 (Cloud Computing)` and Date `2026-10-04`.
3. Mark attendance for students (mark `student13` as absent to demonstrate low attendance calculation).
4. Click **Save Attendance Sheet**.
5. Navigate to **Upload S3 Notes** -> Select a sample PDF document -> Click **Upload to Amazon S3 Bucket**.
6. Show the generated **AWS S3 Pre-Signed Download URL** and click it to demonstrate secure file retrieval.
7. Click **Logout**.

---

### Minute 4: Student Flow & Low-Attendance Warning
1. Click **Quick Demo Login -> Student (<75%)** (`student13@university.edu` / `Student@123`).
2. Point out the **CRITICAL ATTENDANCE WARNING (< 75%)** red alert banner displaying `50.00%` overall attendance.
3. Show the interactive **Chart.js Doughnut Chart** for present vs absent sessions.
4. Show the **Chart.js Bar Chart** for academic marks across Internal 1, Internal 2, Assignment, and Final Exam.
5. Click **Download S3 Attendance Report** -> Show the downloaded CSV file generated from AWS S3.
6. Click **Logout**.

---

### Minute 5: Serverless Alert & ALB Failover Scenario (Highlight for Full Marks!)
1. **Serverless SNS Alert Demonstration**:
   - Open AWS Lambda Console -> Test-trigger `low_attendance_alert` Lambda function.
   - Show the execution output: `Scan Complete. Alerts published for 3 students`.
   - Open email inbox to show the received AWS SNS email alert notification!
2. **ALB High-Availability Failover Demonstration**:
   - Open AWS EC2 Console -> Select EC2 Instance 1 -> Click **Terminate Instance**.
   - Immediately refresh the web application in browser.
   - **Result**: The site continues working without any downtime or 502 error because the ALB instantly routes traffic to EC2 Instance 2!
