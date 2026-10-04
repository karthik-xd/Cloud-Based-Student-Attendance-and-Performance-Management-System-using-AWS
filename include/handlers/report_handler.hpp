#ifndef REPORT_HANDLER_HPP
#define REPORT_HANDLER_HPP

#include <string>

class ReportHandler {
public:
    // Upload course note/document to S3
    static std::string uploadCourseNote(const std::string& filename, const std::string& fileContent, const std::string& contentType);

    // Export student attendance report to S3 and generate pre-signed URL
    static std::string generateAttendanceReportPresignedUrl(int studentId);
};

#endif // REPORT_HANDLER_HPP
