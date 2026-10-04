#include "services/s3_service.hpp"
#include <iostream>
#include <cassert>

void testS3PresignedUrlFormat() {
    S3Service& s3 = S3Service::getInstance();
    std::string key = "reports/attendance_student_13.csv";
    std::string url = s3.generatePresignedUrl(key, 900);

    assert(url.find("https://") != std::string::npos);
    assert(url.find(key) != std::string::npos);
    assert(url.find("Expires=") != std::string::npos);

    std::cout << "[TEST PASSED] AWS S3 Pre-Signed URL Format Generation Test" << std::endl;
}

int main() {
    std::cout << "[RUNNING TESTS] Starting S3 Report Tests..." << std::endl;
    testS3PresignedUrlFormat();
    std::cout << "[ALL TESTS PASSED] S3 Report Test Suite Completed Successfully." << std::endl;
    return 0;
}
