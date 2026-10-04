# Test Plan & Verification Documentation

## 🧪 Test Cases Execution Matrix

| Test ID | Category | Feature Under Test | Input Data / Trigger | Expected Result | Result |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **TC-01** | Auth | Admin Login | `admin@university.edu` / `Admin@123` | Redirects to `/admin/dashboard`, session role='admin' | **PASS** |
| **TC-02** | Auth | Faculty Login | `prof.sharma@university.edu` / `Faculty@123` | Redirects to `/faculty/dashboard`, session role='faculty' | **PASS** |
| **TC-03** | Auth | Student Login | `student13@university.edu` / `Student@123` | Redirects to `/student/dashboard`, session role='student' | **PASS** |
| **TC-04** | Auth | Invalid Credentials | `admin@university.edu` / `WrongPass` | Rejects login, returns HTTP 401 error message | **PASS** |
| **TC-05** | Attendance | Percentage Calculation | Present=18, Held=20 | Attendance % calculated as exactly `90.00%` | **PASS** |
| **TC-06** | Attendance | Low Attendance Detection | Present=10, Held=20 (50%) | Flags student `< 75%`, renders red warning badge | **PASS** |
| **TC-07** | Serverless | Lambda Scan & SNS Alert | EventBridge cron or Admin button trigger | Queries RDS, finds 3 low-attendance students, publishes to SNS | **PASS** |
| **TC-08** | Cloud | S3 Document Upload | Faculty uploads PDF notes | Stores object in S3, generates 30-min Pre-Signed GET URL | **PASS** |
| **TC-09** | Cloud | S3 CSV Report Export | Student requests CSV export | Generates attendance CSV, uploads to S3, returns download URL | **PASS** |
| **TC-10** | Resilience | ALB Instance Failover | Stop 1 EC2 instance in Auto Scaling | ALB detects unhealthy target, routes 100% traffic to 2nd EC2 | **PASS** |
| **TC-11** | Health | ALB Health Check Endpoint | `GET /health` | Returns HTTP 200 OK `{"status": "healthy"}` | **PASS** |

---

## ⚡ Load Balancer Failover & High CPU Alarm Test Instructions

### 1. ALB High Availability & Failover Test
1. Open [EC2 Console](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#Instances:).
2. Select one of the 2 running EC2 instances in your Auto Scaling Group and click **Instance State -> Terminate / Stop**.
3. Continuously refresh the application web URL.
4. **Result**: The Application Load Balancer detects the stopped instance, marks it unhealthy, and seamlessly routes all incoming traffic to the surviving EC2 instance without dropping any user requests!

### 2. CloudWatch High CPU Alarm Test
1. SSH into one of the EC2 instances.
2. Run CPU stress command: `stress --cpu 2 --timeout 300`.
3. Check CloudWatch Alarm `EC2-High-CPU-Alarm` -> State changes from `OK` to `ALARM` -> Triggers SNS notification email!
