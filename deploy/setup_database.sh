#!/bin/bash
# ==============================================================================
# Database Provisioning Script for Amazon RDS MySQL Instance
# Loads database schema and demo seed records.
# ==============================================================================

set -e

# Load environment variables if .env exists
if [ -f ../.env ]; then
    export $(cat ../.env | grep -v '#' | xargs)
fi

DB_HOST=${DB_HOST:-"localhost"}
DB_PORT=${DB_PORT:-3306}
DB_USER=${DB_USER:-"root"}
DB_PASSWORD=${DB_PASSWORD:-""}

echo "[DB SETUP] Target RDS Endpoint: ${DB_HOST}:${DB_PORT}"
echo "[DB SETUP] Importing database schema from database/schema.sql..."

mysql -h ${DB_HOST} -P ${DB_PORT} -u ${DB_USER} -p"${DB_PASSWORD}" < ../database/schema.sql

echo "[DB SETUP] Importing demo seed records from database/seed.sql..."
mysql -h ${DB_HOST} -P ${DB_PORT} -u ${DB_USER} -p"${DB_PASSWORD}" < ../database/seed.sql

echo "[DB SETUP SUCCESS] Database schema and seed data loaded successfully into ${DB_HOST}!"
