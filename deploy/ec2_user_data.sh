#!/bin/bash
# ==============================================================================
# AWS EC2 UserData Bootstrap Script (Asia Pacific Hyderabad - ap-south-2)
# Clones GitHub Repository, installs C++ build dependencies, compiles the server,
# configures Nginx reverse proxy, and starts systemd service automatically.
# ==============================================================================

set -e

LOG_FILE="/var/log/ec2_user_data.log"
exec > >(tee -a ${LOG_FILE}) 2>&1

echo "[EC2 USERDATA] Starting AWS EC2 Instance Provisioning in ap-south-2 at $(date)..."

# 1. Update system packages and install build dependencies
if [ -f /etc/dnf/dnf.conf ] || [ -f /etc/yum.conf ]; then
    echo "[EC2 USERDATA] Package manager: DNF/YUM (Amazon Linux 2023)"
    dnf update -y
    dnf groupinstall -y "Development Tools"
    dnf install -y cmake gcc-c++ git nginx libcurl-devel openssl-devel mariadb-connector-c-devel
else
    echo "[EC2 USERDATA] Package manager: APT (Ubuntu 22.04)"
    apt-get update -y
    apt-get install -y build-essential cmake git nginx libcurl4-openssl-dev libssl-dev libmariadb-dev
fi

# 2. Clone GitHub repository
REPO_URL="https://github.com/karthik-xd/Cloud-Based-Student-Attendance-and-Performance-Management-System-using-AWS.git"
APP_DIR="/opt/cloud_student_system"

echo "[EC2 USERDATA] Cloning repository from ${REPO_URL} into ${APP_DIR}..."
rm -rf ${APP_DIR}
git clone ${REPO_URL} ${APP_DIR}

cd ${APP_DIR}

# 3. Create .env configuration file tuned for ap-south-2
cat << 'EOF' > ${APP_DIR}/.env
PORT=8080
SECRET_KEY=production-cloud-secret-key-2026
DB_HOST=rds-mysql-student-db.c0123456789.ap-south-2.rds.amazonaws.com
DB_PORT=3306
DB_USER=admin
DB_PASSWORD=StudentManagement2026
DB_NAME=student_management_db
AWS_REGION=ap-south-2
S3_BUCKET_NAME=student-system-reports-bucket-demo-hyderabad
SNS_TOPIC_ARN=arn:aws:sns:ap-south-2:494644230149:low-attendance-alerts
EOF

# 4. Build C++ CMake executable
echo "[EC2 USERDATA] Compiling C++ Application..."
mkdir -p build
cd build
cmake ..
make -j$(nproc)

echo "[EC2 USERDATA] Build completed successfully."

# 5. Configure Nginx Reverse Proxy (Port 80 -> Port 8080)
cat << 'EOF' > /etc/nginx/conf.d/cloud_student_system.conf
server {
    listen 80;
    server_name _;

    location / {
        proxy_pass http://127.0.0.1:8080;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }

    location /static/ {
        alias /opt/cloud_student_system/static/;
        expires 30d;
    }
}
EOF

# Remove default Nginx site if present
rm -f /etc/nginx/sites-enabled/default

systemctl restart nginx
systemctl enable nginx

# 6. Create Systemd Service for C++ Backend
cat << 'EOF' > /etc/systemd/system/cloud_student_system.service
[Unit]
Description=Cloud Student Attendance & Performance System (C++)
After=network.target mysql.service

[Service]
Type=simple
User=root
WorkingDirectory=/opt/cloud_student_system
ExecStart=/opt/cloud_student_system/build/cloud_student_system
Restart=always
RestartSec=5
EnvironmentFile=/opt/cloud_student_system/.env

[Install]
WantedBy=multi-user.target
EOF

# 7. Reload systemd and start application service
systemctl daemon-reload
systemctl enable cloud_student_system
systemctl start cloud_student_system

echo "[EC2 USERDATA SUCCESS] EC2 Instance Provisioning Complete in ap-south-2! Server listening on port 80."
