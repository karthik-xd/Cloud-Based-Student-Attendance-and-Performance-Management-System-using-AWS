# Complete AWS Cloud Deployment Guide

**Target AWS Region**: `eu-north-1` (Europe - Stockholm)  
**GitHub Repository**: `https://github.com/karthik-xd/Cloud-Based-Student-Attendance-and-Performance-Management-System-using-AWS.git`

---

## 🚀 Quick Step-by-Step AWS Deployment Steps

### Step 1: Create VPC & Subnets
1. Open the [AWS VPC Console](https://eu-north-1.console.aws.amazon.com/vpc/home?region=eu-north-1).
2. Click **Create VPC** -> Select **VPC and more**.
3. Name tag: `student-system-vpc`.
4. IPv4 CIDR: `10.0.0.0/16`.
5. Number of Availability Zones: `2` (`eu-north-1a` and `eu-north-1b`).
6. Public Subnets: `2` (`10.0.1.0/24`, `10.0.2.0/24`).
7. Private Subnets: `2` (`10.0.10.0/24`, `10.0.20.0/24`).
8. NAT Gateways: `1 per AZ` or `1 in 1 AZ` (for private RDS access).
9. Click **Create VPC**.

---

### Step 2: Create Security Groups
Open [EC2 Security Groups Console](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#SecurityGroups:).

1. **ALB-SG (Public Load Balancer Security Group)**:
   - Inbound: Allow `HTTP (80)` from Anywhere (`0.0.0.0/0`).
2. **EC2-SG (Application Instance Security Group)**:
   - Inbound: Allow `HTTP (8080)` from `ALB-SG`.
   - Inbound: Allow `SSH (22)` from My IP.
3. **RDS-SG (Database Security Group)**:
   - Inbound: Allow `MySQL/Aurora (3306)` from `EC2-SG`.

---

### Step 3: Create Amazon RDS MySQL Instance (Private Subnet)
1. Open [AWS RDS Console](https://eu-north-1.console.aws.amazon.com/rds/home?region=eu-north-1).
2. Click **Create Database**.
3. Engine: **MySQL 8.0**.
4. Templates: **Free Tier** or **Dev/Test**.
5. DB Instance Identifier: `rds-mysql-student-db`.
6. Master Username: `admin`.
7. Master Password: `StudentManagement2026`.
8. VPC: `student-system-vpc`.
9. Subnet Group: Select Private Subnet Group.
10. Public Access: **No** (Private subnet security enforcement).
11. VPC Security Group: Choose `RDS-SG`.
12. Click **Create Database**.

---

### Step 4: Create Amazon S3 Bucket & SNS Topic
1. **S3 Bucket**:
   - Open [AWS S3 Console](https://s3.console.aws.amazon.com/s3/home?region=eu-north-1).
   - Click **Create bucket**. Name: `student-system-reports-bucket-demo`. Region: `eu-north-1`.
   - Keep "Block all public access" ENABLED (pre-signed URL security).
2. **SNS Topic**:
   - Open [AWS SNS Console](https://eu-north-1.console.aws.amazon.com/sns/v3/home?region=eu-north-1#/topics).
   - Click **Create topic** -> Type: **Standard** -> Name: `low-attendance-alerts`.
   - Copy Topic ARN: `arn:aws:sns:eu-north-1:494644230149:low-attendance-alerts`.
   - Click **Create subscription** -> Protocol: **Email** -> Endpoint: Enter student/faculty email -> Confirm Email verification link.

---

### Step 5: Create EC2 Launch Template & Auto Scaling Group
1. Open [EC2 Launch Templates](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#LaunchTemplates:).
2. Click **Create launch template** -> Name: `student-system-lt`.
3. AMI: **Amazon Linux 2023** or **Ubuntu 22.04 LTS**.
4. Instance Type: `t3.micro` or `t4g.micro` (Free Tier Eligible).
5. Security Group: Select `EC2-SG`.
6. Advanced Details -> **User Data**: Paste contents of `deploy/ec2_user_data.sh`.
7. Click **Create Launch Template**.
8. Go to **Auto Scaling Groups** -> Create Auto Scaling Group:
   - Desired: `2`, Min: `1`, Max: `3`.
   - Attach to Application Load Balancer target group.

---

### Step 6: Create Application Load Balancer (ALB)
1. Open [EC2 Load Balancers](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#LoadBalancers:).
2. Click **Create Load Balancer** -> Select **Application Load Balancer**.
3. Name: `student-system-alb`. Scheme: **Internet-facing**.
4. Network Mapping: Select `student-system-vpc` and 2 Public Subnets.
5. Security Group: Select `ALB-SG`.
6. Listeners: `HTTP:80` -> Target Group `student-system-tg` (Health check path: `/health`).
7. Click **Create Load Balancer**.

---

### Step 7: Verification & Troubleshooting

1. **Verify Health Check**:
   Open browser at `http://<ALB-DNS-NAME>/health` -> Should return HTTP 200 `{"status": "healthy"}`.

2. **Troubleshooting 502 Bad Gateway**:
   - Check if C++ backend service is running: `sudo systemctl status cloud_student_system`.
   - View app logs: `journalctl -u cloud_student_system -n 50 --no-pager`.

3. **Troubleshooting Database Connection Timeout**:
   - Ensure `RDS-SG` allows port 3306 inbound from `EC2-SG`.
   - Test DB reachability from EC2: `nc -zv <RDS-ENDPOINT> 3306`.
