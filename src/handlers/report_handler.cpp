#include "handlers/report_handler.hpp"
#include "services/s3_service.hpp"

std::string ReportHandler::uploadCourseNote(const std::string& filename, const std::string& fileContent, const std::string& contentType) {
    std::string objectKey = "documents/notes_" + filename;
    S3Service& s3 = S3Service::getInstance();
    if (s3.uploadFile(objectKey, fileContent, contentType)) {
        return s3.generatePresignedUrl(objectKey, 1800); // 30 minutes validity
    }
    return "";
}

std::string ReportHandler::generateAttendanceReportPresignedUrl(int studentId) {
    return S3Service::getInstance().exportAttendanceCSVToS3(studentId);
}
