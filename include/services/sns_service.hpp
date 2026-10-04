#ifndef SNS_SERVICE_HPP
#define SNS_SERVICE_HPP

#include <string>

class SNSService {
public:
    static SNSService& getInstance();

    // Publish email/SMS alert to AWS Simple Notification Service (SNS) topic
    bool publishAlert(const std::string& subject, const std::string& message);

    // Trigger low attendance alert scan for all students below 75%
    int triggerLowAttendanceAlerts();

private:
    SNSService() = default;
};

#endif // SNS_SERVICE_HPP
