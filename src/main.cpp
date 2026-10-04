/*
 * Cloud Student Attendance & Performance Management System (C++ Backend)
 * Main Entry Point - Multi-threaded C++ HTTP Routing Server
 */

#include <iostream>
#include <string>
#include "httplib.h"
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

    // 2. Initialize Database Connection Pool / Ping
    Database& db = Database::getInstance();
    if (!db.connect()) {
        std::cerr << "[WARNING] Unable to connect to MySQL database at startup. Will retry on demand." << std::endl;
    } else {
        std::cout << "[SUCCESS] Database connection established successfully." << std::endl;
    }

    // 3. HTTP Server Setup
    httplib::Server svr;

    // Health Endpoint
    svr.Get("/health", [](const httplib::Request& req, httplib::Response& res) {
        int statusCode = 200;
        std::string healthResponse = HealthHandler::checkHealth(statusCode);
        res.status = statusCode;
        res.set_content(healthResponse, "application/json");
    });

    // Root Endpoint (serve index.html indirectly or a welcome message)
    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("<html><body><h1>Welcome to the Cloud Student Management System</h1><a href='/login'>Login Here</a></body></html>", "text/html");
    });

    // Login Endpoint
    svr.Get("/login", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("<html><body><h2>Login</h2><form method='POST' action='/api/login'><input type='text' name='email' placeholder='Email'><input type='password' name='password' placeholder='Password'><button type='submit'>Login</button></form></body></html>", "text/html");
    });

    // Handle Login POST (Basic Mock)
    svr.Post("/api/login", [](const httplib::Request& req, httplib::Response& res) {
        std::unordered_map<std::string, std::string> params;
        if (req.has_param("email") && req.has_param("password")) {
            params["email"] = req.get_param_value("email");
            params["password"] = req.get_param_value("password");
        }
        std::string token, redirectUrl;
        std::string result = AuthHandler::handleLogin(params, token, redirectUrl);
        if (result == "SUCCESS") {
            res.set_header("Set-Cookie", "session=" + token + "; Path=/");
            res.set_redirect(redirectUrl);
        } else {
            res.status = 401;
            res.set_content("Login Failed: " + result, "text/plain");
        }
    });

    // Dashboards
    svr.Get("/admin/dashboard", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("<html><body><h1>Admin Dashboard</h1><a href='/'>Home</a></body></html>", "text/html");
    });
    svr.Get("/faculty/dashboard", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("<html><body><h1>Faculty Dashboard</h1><a href='/'>Home</a></body></html>", "text/html");
    });
    svr.Get("/student/dashboard", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("<html><body><h1>Student Dashboard</h1><a href='/'>Home</a></body></html>", "text/html");
    });

    std::cout << "[SUCCESS] C++ Backend Engine initialized. Listening on http://0.0.0.0:" << config.port << std::endl;
    
    // Start listening on port
    svr.listen("0.0.0.0", config.port);

    return 0;
}
