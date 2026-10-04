#include "config.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <iostream>

std::string Config::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

void Config::loadEnv(const std::string& envFilePath) {
    std::ifstream envFile(envFilePath);
    if (envFile.is_open()) {
        std::string line;
        while (std::getline(envFile, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#') continue;

            size_t delimPos = line.find('=');
            if (delimPos != std::string::npos) {
                std::string key = trim(line.substr(0, delimPos));
                std::string val = trim(line.substr(delimPos + 1));
                envMap[key] = val;
            }
        }
        envFile.close();
    }

    // Populate direct configuration members, falling back to system environment variables
    port = getInt("PORT", 8080);
    secretKey = get("SECRET_KEY", "default-cpp-secret-key-change-me");
    dbHost = get("DB_HOST", "localhost");
    dbPort = getInt("DB_PORT", 3306);
    dbUser = get("DB_USER", "root");
    dbPassword = get("DB_PASSWORD", "");
    dbName = get("DB_NAME", "student_management_db");
    awsRegion = get("AWS_REGION", "us-east-1");
    s3BucketName = get("S3_BUCKET_NAME", "student-system-reports-bucket-demo");
    snsTopicArn = get("SNS_TOPIC_ARN", "");
}

std::string Config::get(const std::string& key, const std::string& defaultValue) const {
    // 1. Check system environment variable first (AWS EC2 / systemd environment override)
    const char* sysEnv = std::getenv(key.c_str());
    if (sysEnv != nullptr && std::string(sysEnv).length() > 0) {
        return std::string(sysEnv);
    }
    // 2. Check local .env file map
    auto it = envMap.find(key);
    if (it != envMap.end()) {
        return it->second;
    }
    return defaultValue;
}

int Config::getInt(const std::string& key, int defaultValue) const {
    std::string valStr = get(key, "");
    if (valStr.empty()) return defaultValue;
    try {
        return std::stoi(valStr);
    } catch (...) {
        return defaultValue;
    }
}
