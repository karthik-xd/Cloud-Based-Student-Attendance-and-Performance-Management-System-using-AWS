#include "services/attendance_service.hpp"
#include "db.hpp"
#include <cmath>
#include <iostream>

AttendanceService& AttendanceService::getInstance() {
    static AttendanceService instance;
    return instance;
}

StudentOverallAttendance AttendanceService::getStudentAttendance(int studentId) {
    StudentOverallAttendance result;
    result.studentId = studentId;
    result.totalClassesHeld = 0;
    result.totalClassesPresent = 0;
    result.overallPercentage = 0.0;
    result.isLowAttendance = false;

    Database& db = Database::getInstance();
    std::string sIdStr = std::to_string(studentId);

    // Fetch student info
    ResultSet sRes = db.query("SELECT s.roll_number, s.name, u.email FROM students s JOIN users u ON s.user_id = u.id WHERE s.id = " + sIdStr);
    if (!sRes.empty()) {
        result.rollNumber = sRes[0]["roll_number"];
        result.name = sRes[0]["name"];
        result.email = sRes[0]["email"];
    }

    // Fetch enrolled subjects for student
    std::string subjSql = "SELECT sub.id, sub.subject_code, sub.name FROM subjects sub "
                          "JOIN enrollments e ON sub.id = e.subject_id "
                          "WHERE e.student_id = " + sIdStr;
    ResultSet subjects = db.query(subjSql);

    for (const auto& sub : subjects) {
        SubjectAttendance sa;
        sa.subjectId = std::stoi(sub.at("id"));
        sa.subjectCode = sub.at("subject_code");
        sa.subjectName = sub.at("name");

        // Total classes held for subject
        std::string heldSql = "SELECT COUNT(*) as total FROM attendance WHERE student_id = " + sIdStr + 
                              " AND subject_id = " + sub.at("id");
        ResultSet heldRes = db.query(heldSql);
        sa.classesHeld = heldRes.empty() ? 0 : std::stoi(heldRes[0].at("total"));

        // Classes present
        std::string presSql = "SELECT COUNT(*) as present FROM attendance WHERE student_id = " + sIdStr + 
                              " AND subject_id = " + sub.at("id") + " AND status = 'present'";
        ResultSet presRes = db.query(presSql);
        sa.classesPresent = presRes.empty() ? 0 : std::stoi(presRes[0].at("present"));

        if (sa.classesHeld > 0) {
            sa.percentage = (double)sa.classesPresent / sa.classesHeld * 100.0;
        } else {
            sa.percentage = 100.0; // Default if no classes held yet
        }
        sa.isLowAttendance = (sa.percentage < 75.0);

        result.totalClassesHeld += sa.classesHeld;
        result.totalClassesPresent += sa.classesPresent;
        result.subjectBreakdown.push_back(sa);
    }

    if (result.totalClassesHeld > 0) {
        result.overallPercentage = (double)result.totalClassesPresent / result.totalClassesHeld * 100.0;
    } else {
        result.overallPercentage = 100.0;
    }

    result.isLowAttendance = (result.overallPercentage < 75.0);
    return result;
}

std::vector<StudentOverallAttendance> AttendanceService::getLowAttendanceStudents(double threshold) {
    std::vector<StudentOverallAttendance> lowList;
    Database& db = Database::getInstance();

    ResultSet students = db.query("SELECT id FROM students");
    for (const auto& s : students) {
        int studentId = std::stoi(s.at("id"));
        StudentOverallAttendance att = getStudentAttendance(studentId);
        if (att.overallPercentage < threshold && att.totalClassesHeld > 0) {
            lowList.push_back(att);
        }
    }
    return lowList;
}

std::vector<StudentOverallAttendance> AttendanceService::getSubjectAttendanceSummary(int subjectId) {
    std::vector<StudentOverallAttendance> summaryList;
    Database& db = Database::getInstance();

    std::string sql = "SELECT student_id FROM enrollments WHERE subject_id = " + std::to_string(subjectId);
    ResultSet enrollments = db.query(sql);

    for (const auto& e : enrollments) {
        int studentId = std::stoi(e.at("student_id"));
        summaryList.push_back(getStudentAttendance(studentId));
    }
    return summaryList;
}
