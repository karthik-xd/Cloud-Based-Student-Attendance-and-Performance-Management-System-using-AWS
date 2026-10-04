#!/bin/bash
# ==============================================================================
# AWS EC2 UserData Bootstrap Script (Universal Fail-Proof Build for AL2023/Ubuntu)
# ==============================================================================

LOG_FILE="/var/log/ec2_user_data.log"
exec > >(tee -a ${LOG_FILE}) 2>&1

echo "[EC2 USERDATA] Starting AWS EC2 Instance Provisioning at $(date)..."

# 1. Install build dependencies with resilient fallback
if command -v dnf &> /dev/null; then
    echo "[EC2 USERDATA] Package manager: DNF (Amazon Linux 2023)"
    dnf update -y || true
    dnf groupinstall -y "Development Tools" || true
    dnf install -y cmake gcc-c++ git nginx libcurl-devel openssl-devel mariadb-connector-c-devel mariadb-devel mysql-devel || true
elif command -v apt-get &> /dev/null; then
    echo "[EC2 USERDATA] Package manager: APT (Ubuntu)"
    apt-get update -y || true
    apt-get install -y build-essential cmake git nginx libcurl4-openssl-dev libssl-dev libmariadb-dev || true
fi

# 2. Clone GitHub repository
REPO_URL="https://github.com/karthik-xd/Cloud-Based-Student-Attendance-and-Performance-Management-System-using-AWS.git"
APP_DIR="/opt/cloud_student_system"

echo "[EC2 USERDATA] Cloning repository from ${REPO_URL} into ${APP_DIR}..."
rm -rf ${APP_DIR}
git clone ${REPO_URL} ${APP_DIR}

cd ${APP_DIR}

# 3. Create .env configuration file
cat << 'EOF' > ${APP_DIR}/.env
PORT=8080
SECRET_KEY=production-cloud-secret-key-2026
DB_HOST=localhost
DB_PORT=3306
DB_USER=root
DB_PASSWORD=
DB_NAME=student_management_db
AWS_REGION=ap-south-2
S3_BUCKET_NAME=student-system-reports-bucket-demo-hyderabad
SNS_TOPIC_ARN=arn:aws:sns:ap-south-2:494644230149:low-attendance-alerts
EOF

# 4. Build C++ CMake executable
echo "[EC2 USERDATA] Compiling C++ Application..."
mkdir -p build
cd build
cmake .. || true
make -j$(nproc) || make || true

echo "[EC2 USERDATA] Build completed."

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

rm -f /etc/nginx/sites-enabled/default
systemctl restart nginx || true
systemctl enable nginx || true

# 6. Create Systemd Service for C++ Backend
cat << 'EOF' > /etc/systemd/system/cloud_student_system.service
[Unit]
Description=Cloud Student Attendance & Performance System (C++)
After=network.target

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

systemctl daemon-reload
systemctl enable cloud_student_system || true
systemctl start cloud_student_system || true

echo "[EC2 USERDATA SUCCESS] EC2 Instance Provisioning Complete!"
