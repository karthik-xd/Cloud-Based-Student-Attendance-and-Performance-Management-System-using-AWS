#ifndef HEALTH_HANDLER_HPP
#define HEALTH_HANDLER_HPP

#include <string>

class HealthHandler {
public:
    // Application Load Balancer (ALB) health check response
    static std::string checkHealth(int& outStatusCode);
};

#endif // HEALTH_HANDLER_HPP
