#ifndef ATTENDANCE_SERVICE_HPP
#define ATTENDANCE_SERVICE_HPP

#include <string>
#include <vector>
#include <unordered_map>

struct SubjectAttendance {
    int subjectId;
    std::string subjectCode;
    std::string subjectName;
    int classesHeld;
    int classesPresent;
    double percentage;
    bool isLowAttendance; // < 75%
};

struct StudentOverallAttendance {
    int studentId;
    std::string rollNumber;
    std::string name;
    std::string email;
    int totalClassesHeld;
    int totalClassesPresent;
    double overallPercentage;
    bool isLowAttendance;
    std::vector<SubjectAttendance> subjectBreakdown;
};

class AttendanceService {
public:
    static AttendanceService& getInstance();

    // Calculate attendance metrics for a single student
    StudentOverallAttendance getStudentAttendance(int studentId);

    // Calculate attendance for all students enrolled in a specific subject
    std::vector<StudentOverallAttendance> getSubjectAttendanceSummary(int subjectId);

    // Get list of all students whose overall attendance is below 75% threshold
    std::vector<StudentOverallAttendance> getLowAttendanceStudents(double threshold = 75.0);

private:
    AttendanceService() = default;
};

#endif // ATTENDANCE_SERVICE_HPP
