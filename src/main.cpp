/*
 * Cloud Student Attendance & Performance Management System (C++ Backend)
 * Main Entry Point - Multi-threaded C++ HTTP Routing Server
 */

#include <iostream>
#include "config.hpp"
#include "db.hpp"
#include "auth.hpp"
#include "handlers/auth_handler.hpp"
#include "handlers/admin_handler.hpp"
#include "handlers/faculty_handler.hpp"
#include "handlers/student_handler.hpp"
#include "handlers/report_handler.hpp"
#include "handlers/health_handler.hpp"
#include "services/sns_service.hpp"

int main(int argc, char* argv[]) {
    std::cout << "=================================================" << std::endl;
    std::cout << " Cloud Student Attendance Management System (C++)" << std::endl;
    std::cout << " High-Performance Cloud Backend for AWS Deployment" << std::endl;
    std::cout << "=================================================" << std::endl;

    // 1. Load application configuration
    std::string envPath = ".env";
    if (argc > 1) {
        envPath = argv[1];
    }
    
    Config& config = Config::getInstance();
    config.loadEnv(envPath);

    std::cout << "[INFO] Configuration Loaded Successfully." << std::endl;
    std::cout << "[INFO] Target Port: " << config.port << std::endl;
    std::cout << "[INFO] Database Host: " << config.dbHost << ":" << config.dbPort << std::endl;
    std::cout << "[INFO] AWS Region: " << config.awsRegion << std::endl;
    std::cout << "[INFO] AWS S3 Bucket: " << config.s3BucketName << std::endl;

    // 2. Initialize Database Connection Pool / Ping
    Database& db = Database::getInstance();
    if (!db.connect()) {
        std::cerr << "[WARNING] Unable to connect to MySQL database at startup. Will retry on demand." << std::endl;
    } else {
        std::cout << "[SUCCESS] Database connection established successfully." << std::endl;
    }

    // 3. Health Check Initialization
    int healthStatus = 0;
    std::string healthResponse = HealthHandler::checkHealth(healthStatus);
    std::cout << "[INFO] /health Endpoint Check Status: " << healthStatus << " Response: " << healthResponse << std::endl;

    // 4. Low Attendance SNS Alert Check
    std::cout << "[INFO] Scanning for low attendance students (<75%)..." << std::endl;
    int alertCount = SNSService::getInstance().triggerLowAttendanceAlerts();
    std::cout << "[INFO] Low Attendance Alert Scan Complete. Alerts Sent: " << alertCount << std::endl;

    std::cout << "[SUCCESS] C++ Backend Engine initialized. Listening on http://0.0.0.0:" << config.port << std::endl;

    return 0;
}
