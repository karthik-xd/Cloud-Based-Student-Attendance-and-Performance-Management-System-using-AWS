#include "handlers/health_handler.hpp"
#include "db.hpp"

std::string HealthHandler::checkHealth(int& outStatusCode) {
    Database& db = Database::getInstance();
    
    // Check RDS / MySQL Database Ping status
    if (db.isConnected()) {
        outStatusCode = 200;
        return "{\"status\":\"healthy\",\"database\":\"connected\",\"service\":\"student_management_system_cpp\"}";
    } else {
        outStatusCode = 500;
        return "{\"status\":\"unhealthy\",\"database\":\"disconnected\",\"service\":\"student_management_system_cpp\"}";
    }
}
