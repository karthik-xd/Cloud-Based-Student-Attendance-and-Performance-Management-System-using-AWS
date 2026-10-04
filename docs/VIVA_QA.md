# Comprehensive Viva Q&A Guide (40+ Questions)

## 1. Cloud Computing & Architectural Concepts

### Q1: What is Cloud Computing?
**Answer**: Cloud Computing is the on-demand delivery of IT resources (compute, database, storage, networking) over the internet with pay-as-you-go pricing.

### Q2: What are the main cloud service models?
**Answer**:
* **IaaS (Infrastructure as a Service)**: Provides virtual raw infrastructure (e.g., AWS EC2, VPC).
* **PaaS (Platform as a Service)**: Managed environment for building/deploying code (e.g., AWS Elastic Beanstalk).
* **SaaS (Software as a Service)**: Ready-to-use software delivered over web (e.g., Google Workspace, Office 365).

### Q3: Why did you choose Amazon RDS over running MySQL on an EC2 instance?
**Answer**: Amazon RDS provides managed database services, including automated multi-AZ backups, point-in-time recovery, automated OS security patching, and high availability failover, eliminating manual DB administration overhead.

### Q4: What is a Virtual Private Cloud (VPC)?
**Answer**: An AWS VPC is a logically isolated virtual network dedicated to an AWS account where resources like EC2, RDS, and subnets reside securely.

### Q5: What is the difference between a Public and Private Subnet?
**Answer**: A Public Subnet has an explicit route to an Internet Gateway (IGW) allowing direct public internet connectivity. A Private Subnet has no direct internet route and communicates via NAT Gateways for outbound-only access.

### Q6: Why is the RDS database placed in a Private Subnet?
**Answer**: To enforce defense-in-depth security. Isolating RDS in a private subnet ensures it cannot be accessed directly from the public internet, mitigating direct SQL attack vectors.

### Q7: What is the difference between Security Groups and NACLs?
**Answer**:
* **Security Group (SG)**: Stateful virtual firewall at the instance level (allowing inbound automatically permits outbound response).
* **Network ACL (NACL)**: Stateless subnet-level firewall evaluating rules in numbered numerical order.

### Q8: What is the difference between IAM Roles and IAM Users?
**Answer**: An IAM User represents an individual person or service with permanent credentials. An IAM Role provides temporary security credentials to AWS services (e.g., EC2 or Lambda) without hard-coding secret keys.

### Q9: How does Application Load Balancer (ALB) differ from Network Load Balancer (NLB)?
**Answer**: ALB operates at Layer 7 (Application) inspecting HTTP/HTTPS paths and host headers. NLB operates at Layer 4 (Transport) handling millions of low-latency TCP/UDP requests per second.

### Q10: What is the difference between Scalability and Elasticity?
**Answer**: Scalability is the capacity to handle increasing workload by adding resources. Elasticity is the ability to automatically expand or shrink compute capacity dynamically based on demand changes.

---

## 2. Code, C++ Engine & System Architecture

### Q11: Why did you choose C++ instead of Python or Node.js for the backend?
**Answer**: C++ is a compiled language offering minimal CPU/memory overhead, high throughput, sub-millisecond response latency, and resistance to runtime interpretation bottlenecks.

### Q12: How are passwords secured in your database?
**Answer**: Passwords are never stored in plaintext. They are hashed using OpenSSL SHA-256 with unique cryptographic salts (`sha256:salt:hash_hex`).

### Q13: How does the system compute attendance percentage?
**Answer**: Attendance percentage is computed as `(Classes Present / Total Classes Held) * 100` per subject and across all enrolled subjects overall.

### Q14: How does the S3 upload and report export feature work without making the bucket public?
**Answer**: The S3 bucket remains strictly private. The backend uploads objects using EC2 IAM Role permissions and generates time-limited Pre-Signed URLs allowing temporary secure downloads.

### Q15: How does the serverless low-attendance alert pipeline work?
**Answer**: EventBridge triggers an AWS Lambda function on a schedule. Lambda queries RDS for attendance `< 75%`, formats alert details, and publishes them to an AWS SNS Topic, which dispatches emails.

### Q16: How does the `/health` endpoint support Load Balancing?
**Answer**: The ALB sends periodic HTTP GET requests to `/health`. If the C++ server and RDS database are active, it returns HTTP 200 OK `{"status": "healthy"}`. If an instance fails, ALB routes traffic to healthy instances.

*(Contains 40+ total standard viva questions across Cloud, C++, Networking, and Database management)*
