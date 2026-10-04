#ifndef AUTH_HANDLER_HPP
#define AUTH_HANDLER_HPP

#include <string>
#include <unordered_map>

class AuthHandler {
public:
    static std::string handleLogin(const std::unordered_map<std::string, std::string>& params, std::string& outCookieToken, std::string& outRedirectUrl);
    static std::string handleLogout(const std::string& sessionToken);
};

#endif // AUTH_HANDLER_HPP
