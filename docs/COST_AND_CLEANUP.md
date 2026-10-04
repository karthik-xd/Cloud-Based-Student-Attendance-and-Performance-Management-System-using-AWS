# AWS Cost Management & Teardown Cleanup Checklist

## 💰 Keeping AWS Costs at Zero

To ensure your AWS bill stays at **$0.00** during development and demonstration:

1. **Use Free Tier Eligible Resources**:
   - EC2: `t3.micro` or `t4g.micro` (750 hours/month free).
   - RDS: `db.t3.micro` (750 hours/month free + 20 GB SSD storage).
   - S3: Up to 5 GB storage free.
2. **Monitor AWS Credits**:
   - You currently have **$93.54 USD in AWS Credits** active until January 2027 in `eu-north-1`.
3. **Stop EC2 & RDS Instances when not testing**:
   - When you are not working on the project, stop your EC2 instances and pause your RDS database to conserve hours.

---

## 🗑️ Step-by-Step AWS Resource Teardown Checklist (After Final Viva Demo)

When your presentation and demo are finished, follow this exact order to delete all created resources and prevent any unexpected charges:

1. **Delete Auto Scaling Group & Launch Template**:
   - Go to [EC2 Auto Scaling Groups](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#AutoScalingGroups:) -> Select `student-system-asg` -> **Delete**.
   - Go to [Launch Templates](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#LaunchTemplates:) -> Delete `student-system-lt`.

2. **Delete Application Load Balancer & Target Group**:
   - Go to [Load Balancers](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#LoadBalancers:) -> Select `student-system-alb` -> **Delete**.
   - Go to [Target Groups](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#TargetGroups:) -> Delete `student-system-tg`.

3. **Delete Amazon RDS Database**:
   - Go to [RDS Console](https://eu-north-1.console.aws.amazon.com/rds/home?region=eu-north-1#databases:) -> Select `rds-mysql-student-db` -> Actions -> **Delete**.
   - Uncheck "Create final snapshot" -> Type `delete me` -> Confirm deletion.

4. **Empty & Delete S3 Bucket**:
   - Go to [S3 Console](https://s3.console.aws.amazon.com/s3/home?region=eu-north-1) -> Select `student-system-reports-bucket-demo`.
   - Click **Empty** -> Confirm emptying all objects -> Click **Delete**.

5. **Delete Lambda Function & EventBridge Rule**:
   - Go to [Lambda Console](https://eu-north-1.console.aws.amazon.com/lambda/home?region=eu-north-1#/functions) -> Delete `low_attendance_alert`.
   - Go to [EventBridge Console](https://eu-north-1.console.aws.amazon.com/events/home?region=eu-north-1#/rules) -> Delete schedule rule.

6. **Delete SNS Topic & Subscriptions**:
   - Go to [SNS Console](https://eu-north-1.console.aws.amazon.com/sns/v3/home?region=eu-north-1#/topics) -> Delete `low-attendance-alerts`.

7. **Delete Security Groups & Custom VPC**:
   - Go to [Security Groups](https://eu-north-1.console.aws.amazon.com/ec2/home?region=eu-north-1#SecurityGroups:) -> Delete `ALB-SG`, `EC2-SG`, `RDS-SG`.
   - Go to [VPC Console](https://eu-north-1.console.aws.amazon.com/vpc/home?region=eu-north-1#vpcs:) -> Select `student-system-vpc` -> Actions -> **Delete VPC** (automatically deletes attached subnets, route tables, and NAT gateways).
