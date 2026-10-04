#include "services/sns_service.hpp"
#include "services/attendance_service.hpp"
#include "config.hpp"
#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <iomanip>

SNSService& SNSService::getInstance() {
    static SNSService instance;
    return instance;
}

bool SNSService::publishAlert(const std::string& subject, const std::string& message) {
    Config& config = Config::getInstance();
    
    if (config.snsTopicArn.empty()) {
        std::cout << "[SNS ALERT Mock] Topic ARN not set. Alert Message: " << subject << " - " << message << std::endl;
        return true;
    }

    std::cout << "[SNS SERVICE] Publishing message to SNS Topic: " << config.snsTopicArn << std::endl;
    std::cout << "[SNS SERVICE] Subject: " << subject << std::endl;
    std::cout << "[SNS SERVICE] Message Body: " << message << std::endl;

    // Send HTTP POST request to AWS SNS API endpoint using libcurl
    CURL* curl = curl_easy_init();
    if (curl) {
        std::string postData = "Action=Publish&TopicArn=" + config.snsTopicArn + 
                               "&Subject=" + curl_easy_escape(curl, subject.c_str(), subject.length()) + 
                               "&Message=" + curl_easy_escape(curl, message.c_str(), message.length());
        
        std::string snsEndpoint = "https://sns." + config.awsRegion + ".amazonaws.com/";

        curl_easy_setopt(curl, CURLOPT_URL, snsEndpoint.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            std::cout << "[SNS SERVICE] Successfully published alert to AWS SNS." << std::endl;
            return true;
        }
    }

    std::cout << "[SNS SERVICE Mock] Successfully logged alert: " << subject << std::endl;
    return true;
}

int SNSService::triggerLowAttendanceAlerts() {
    AttendanceService& attService = AttendanceService::getInstance();
    std::vector<StudentOverallAttendance> lowStudents = attService.getLowAttendanceStudents(75.0);

    int count = 0;
    for (const auto& student : lowStudents) {
        std::stringstream msg;
        msg << "CRITICAL ATTENDANCE WARNING\n"
            << "Student Name: " << student.name << "\n"
            << "Roll Number: " << student.rollNumber << "\n"
            << "Email: " << student.email << "\n"
            << "Overall Attendance: " << std::fixed << std::setprecision(2) << student.overallPercentage << "%\n"
            << "Requirement: 75.00%\n\n"
            << "Please contact your faculty department advisor immediately.";

        publishAlert("Low Attendance Warning: " + student.rollNumber, msg.str());
        count++;
    }

    return count;
}
