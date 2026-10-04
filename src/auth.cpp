#include "auth.hpp"
#include "db.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <iostream>

AuthManager& AuthManager::getInstance() {
    static AuthManager instance;
    return instance;
}

std::string AuthManager::hashPassword(const std::string& password, const std::string& salt) {
    std::string saltedPassword = password + salt;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen = 0;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, saltedPassword.c_str(), saltedPassword.length());
    EVP_DigestFinal_ex(ctx, hash, &hashLen);
    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    ss << "sha256:" << salt << ":";
    for (unsigned int i = 0; i < hashLen; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

bool AuthManager::verifyPassword(const std::string& password, const std::string& storedHash) {
    // Parse stored hash format sha256:salt:hash
    size_t pos1 = storedHash.find(':');
    if (pos1 == std::string::npos) return false;
    size_t pos2 = storedHash.find(':', pos1 + 1);
    if (pos2 == std::string::npos) return false;

    std::string salt = storedHash.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string expectedHash = hashPassword(password, salt);
    return (expectedHash == storedHash);
}

bool AuthManager::authenticateUser(const std::string& email, const std::string& password, UserSession& outSession) {
    Database& db = Database::getInstance();
    std::string cleanEmail = db.escape(email);

    std::string sql = "SELECT id, email, password_hash, role FROM users WHERE email = '" + cleanEmail + "'";
    ResultSet res = db.query(sql);

    if (res.empty()) {
        return false;
    }

    Row user = res[0];
    if (!verifyPassword(password, user["password_hash"])) {
        return false;
    }

    outSession.userId = std::stoi(user["id"]);
    outSession.email = user["email"];
    outSession.role = user["role"];
    outSession.csrfToken = generateCSRFToken();
    outSession.createdAt = (long)std::time(nullptr);
    outSession.roleTableId = 0;

    // Fetch corresponding profile ID (student_id or faculty_id)
    if (outSession.role == "student") {
        ResultSet sRes = db.query("SELECT id FROM students WHERE user_id = " + user["id"]);
        if (!sRes.empty()) {
            outSession.roleTableId = std::stoi(sRes[0]["id"]);
        }
    } else if (outSession.role == "faculty") {
        ResultSet fRes = db.query("SELECT id FROM faculty WHERE user_id = " + user["id"]);
        if (!fRes.empty()) {
            outSession.roleTableId = std::stoi(fRes[0]["id"]);
        }
    }

    return true;
}

std::string AuthManager::generateCSRFToken() {
    unsigned char buffer[16];
    RAND_bytes(buffer, sizeof(buffer));
    std::stringstream ss;
    for (int i = 0; i < 16; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i];
    }
    return ss.str();
}

std::string AuthManager::createSession(const UserSession& session) {
    std::lock_guard<std::mutex> lock(sessionMutex);
    std::string sessionToken = generateCSRFToken() + generateCSRFToken();
    activeSessions[sessionToken] = session;
    return sessionToken;
}

bool AuthManager::getSession(const std::string& token, UserSession& outSession) {
    std::lock_guard<std::mutex> lock(sessionMutex);
    auto it = activeSessions.find(token);
    if (it != activeSessions.end()) {
        outSession = it->second;
        return true;
    }
    return false;
}

void AuthManager::destroySession(const std::string& token) {
    std::lock_guard<std::mutex> lock(sessionMutex);
    activeSessions.erase(token);
}
