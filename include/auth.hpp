#ifndef AUTH_HPP
#define AUTH_HPP

#include <string>
#include <unordered_map>
#include <mutex>

struct UserSession {
    int userId;
    std::string email;
    std::string role; // admin, faculty, student
    int roleTableId;  // student_id or faculty_id
    std::string csrfToken;
    long createdAt;
};

class AuthManager {
public:
    static AuthManager& getInstance();

    // Generate SHA-256 salted hash using OpenSSL
    std::string hashPassword(const std::string& password, const std::string& salt = "univ2026");

    // Verify plaintext password against stored hash format sha256:salt:hash
    bool verifyPassword(const std::string& password, const std::string& storedHash);

    // Authenticate user credentials against database
    bool authenticateUser(const std::string& email, const std::string& password, UserSession& outSession);

    // Create session token and map to active user session
    std::string createSession(const UserSession& session);

    // Retrieve session by session token
    bool getSession(const std::string& token, UserSession& outSession);

    // Destroy active session on logout
    void destroySession(const std::string& token);

    // Generate secure random CSRF token
    std::string generateCSRFToken();

private:
    AuthManager() = default;
    std::unordered_map<std::string, UserSession> activeSessions;
    std::mutex sessionMutex;
};

#endif // AUTH_HPP
