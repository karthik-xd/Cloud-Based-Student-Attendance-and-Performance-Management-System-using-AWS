#include "handlers/auth_handler.hpp"
#include "auth.hpp"
#include <iostream>

std::string AuthHandler::handleLogin(const std::unordered_map<std::string, std::string>& params, std::string& outCookieToken, std::string& outRedirectUrl) {
    auto emailIt = params.find("email");
    auto passIt = params.find("password");

    if (emailIt == params.end() || passIt == params.end()) {
        return "ERROR: Missing email or password field";
    }

    UserSession session;
    AuthManager& auth = AuthManager::getInstance();

    if (auth.authenticateUser(emailIt->second, passIt->second, session)) {
        outCookieToken = auth.createSession(session);
        if (session.role == "admin") {
            outRedirectUrl = "/admin/dashboard";
        } else if (session.role == "faculty") {
            outRedirectUrl = "/faculty/dashboard";
        } else if (session.role == "student") {
            outRedirectUrl = "/student/dashboard";
        } else {
            outRedirectUrl = "/";
        }
        return "SUCCESS";
    }

    return "ERROR: Invalid email or password credentials";
}

std::string AuthHandler::handleLogout(const std::string& sessionToken) {
    if (!sessionToken.empty()) {
        AuthManager::getInstance().destroySession(sessionToken);
    }
    return "LOGOUT_SUCCESS";
}
