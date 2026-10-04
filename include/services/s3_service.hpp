#ifndef S3_SERVICE_HPP
#define S3_SERVICE_HPP

#include <string>

class S3Service {
public:
    static S3Service& getInstance();

    // Upload local file content to Amazon S3 bucket
    bool uploadFile(const std::string& objectKey, const std::string& fileContent, const std::string& contentType = "text/plain");

    // Generate AWS S3 Pre-Signed GET URL (valid for specified expiration seconds, default 15 minutes)
    std::string generatePresignedUrl(const std::string& objectKey, int expirationSeconds = 900);

    // Export student attendance summary as CSV to S3 and return presigned URL
    std::string exportAttendanceCSVToS3(int studentId);

private:
    S3Service() = default;
};

#endif // S3_SERVICE_HPP
