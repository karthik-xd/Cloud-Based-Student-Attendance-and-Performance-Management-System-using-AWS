# Presentation Outline (12-15 Slides with Speaker Notes)

**Team Members**:
* **Member 1 (Frontend & UI)**: Slides 1 to 4
* **Member 2 (Backend & Database)**: Slides 5 to 8
* **Member 3 (AWS Architecture & Cloud Deployment)**: Slides 9 to 13

---

### Slide 1: Title Slide
* **Title**: Cloud-Based Student Attendance & Performance Management System
* **Subtitle**: Deployed on AWS with Multi-Tier Security, High Availability, and Serverless Automation
* **Bullets**: Team Members, University Department, AWS Region (`eu-north-1`)
* **Speaker Notes (Member 1)**: "Good morning respected professors and external examiners. Today, our team is presenting our Cloud Computing project—a scalable, AWS-deployed Student Attendance and Performance Management System."

---

### Slide 2: Project Objectives & Problem Statement
* **Bullets**:
  * Eliminates manual attendance tracking errors.
  * Solves on-premises server crash issues during peak evaluation periods.
  * Provides real-time notifications for students falling below 75% attendance.
* **Speaker Notes (Member 1)**: "Traditional college management servers often crash when hundreds of students log in simultaneously. Our cloud architecture solves this by leveraging elasticity and auto-scaling."

---

### Slide 3: User Roles & Access Control
* **Bullets**:
  * **Admin**: User lifecycle management, subject creation, faculty mapping.
  * **Faculty**: Daily attendance marking, mark entry, S3 note upload.
  * **Student**: Interactive dashboards, attendance percentage indicators, CSV report exports.
* **Speaker Notes (Member 1)**: "We implemented strict Role-Based Access Control so that each user role accesses only their designated interfaces."

---

### Slide 4: Frontend UI Design & Visualizations
* **Bullets**:
  * Built using HTML5, Jinja2, Bootstrap 5, and Chart.js.
  * Interactive Doughnut chart for attendance ratios.
  * Stacked Bar chart for academic marks breakdown across internals and final exams.
* **Speaker Notes (Member 1)**: "The UI is clean and responsive. Chart.js visualizes student performance visually, making low attendance immediately noticeable via dynamic warning badges."

---

### Slide 5: High-Performance C++ Backend Engine
* **Bullets**:
  * Developed in C++17 for maximum speed and memory efficiency.
  * Multi-threaded HTTP router handling concurrent user sessions.
  * Native OpenSSL password encryption (SHA-256 salted hashing).
* **Speaker Notes (Member 2)**: "Unlike traditional dynamic backend languages, our C++ backend compiles directly to machine code, delivering ultra-low response latency and handling thousands of requests effortlessly."

---

### Slide 6: Database Design on Amazon RDS MySQL
* **Bullets**:
  * Relational model with primary keys, foreign keys, and indexes.
  * Schema includes Users, Students, Faculty, Subjects, Attendance, and Marks.
  * Automated foreign key cascading for data integrity.
* **Speaker Notes (Member 2)**: "Our database schema enforces strict relational constraints and indexing. For instance, the attendance table uses a unique composite key on student, subject, and date to prevent duplicate marking."

---

### Slide 7: Attendance Calculation Algorithm
* **Bullets**:
  * Formula: `(Total Classes Present / Total Classes Held) * 100`.
  * Calculated per subject and aggregated overall.
  * Threshold check: Flagged if `< 75.0%`.
* **Speaker Notes (Member 2)**: "The system dynamically computes attendance percentage queries on demand, allowing instant identification of students requiring warning alerts."

---

### Slide 8: Amazon S3 Document & Report Management
* **Bullets**:
  * Notes and documents uploaded directly to private S3 bucket.
  * Attendance reports exported as CSV to S3.
  * Access protected via time-limited AWS Pre-Signed GET URLs.
* **Speaker Notes (Member 2)**: "To maintain privacy, the S3 bucket blocks all public access. The backend generates Pre-Signed URLs valid for 15 to 30 minutes for secure temporary downloads."

---

### Slide 9: AWS VPC Network Security & Isolation
* **Bullets**:
  * Custom VPC (`10.0.0.0/16`) across 2 Availability Zones (`eu-north-1a`, `eu-north-1b`).
  * Public subnets for Application Load Balancer.
  * Private subnets for Amazon RDS database isolation.
* **Speaker Notes (Member 3)**: "From an AWS architecture perspective, security is paramount. The RDS instance resides in private subnets with no direct internet access, reachable only via security group rules from our EC2 servers."

---

### Slide 10: Auto Scaling & Load Balancing (ALB)
* **Bullets**:
  * Application Load Balancer distributes incoming HTTP traffic.
  * Auto Scaling Group maintains 1 to 3 EC2 instances based on CPU utilization.
  * Automated health check endpoint (`/health`).
* **Speaker Notes (Member 3)**: "If an EC2 instance experiences high CPU or hardware failure, ALB automatically reroutes traffic to healthy instances while Auto Scaling launches a fresh instance."

---

### Slide 11: Serverless Monitoring with AWS Lambda & SNS
* **Bullets**:
  * EventBridge daily cron schedule triggers AWS Lambda.
  * Lambda queries RDS for attendance `< 75%`.
  * Publishes warning notifications to AWS SNS, delivering instant emails.
* **Speaker Notes (Member 3)**: "We implemented a serverless pipeline using AWS Lambda and SNS. It runs automatically every day to notify students with low attendance without requiring manual intervention."

---

### Slide 12: Cost Optimization & AWS Free Tier
* **Bullets**:
  * Utilizes Free Tier eligible instances (`t3.micro` / `db.t3.micro`).
  * AWS credits ($93.54 available) cover infrastructure costs.
  * Zero-cost architecture design.
* **Speaker Notes (Member 3)**: "By leveraging the AWS Free Tier and optimizing compute allocation, our system operates at virtually zero cost while demonstrating enterprise-grade cloud capabilities."

---

### Slide 13: Live Demonstration & Viva Conclusion
* **Bullets**:
  * Demo: Admin management, Faculty marking, Student dashboard warning, S3 report export.
  * Failure Test: ALB failover during instance termination.
  * Summary & Q&A session.
* **Speaker Notes (Member 3)**: "We will now demonstrate the live system running on AWS. We welcome any questions from the examination committee."
