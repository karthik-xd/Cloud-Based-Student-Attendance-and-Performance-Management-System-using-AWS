#include "handlers/student_handler.hpp"

StudentOverallAttendance StudentHandler::getStudentDashboardData(int studentId) {
    return AttendanceService::getInstance().getStudentAttendance(studentId);
}

ResultSet StudentHandler::getStudentMarks(int studentId) {
    Database& db = Database::getInstance();
    std::string sql = "SELECT sub.subject_code, sub.name as subject_name, "
                      "COALESCE(m.internal1_marks, 0) as internal1, "
                      "COALESCE(m.internal2_marks, 0) as internal2, "
                      "COALESCE(m.assignment_marks, 0) as assignment, "
                      "COALESCE(m.final_marks, 0) as final_exam, "
                      "(COALESCE(m.internal1_marks, 0) + COALESCE(m.internal2_marks, 0) + COALESCE(m.assignment_marks, 0) + COALESCE(m.final_marks, 0)) as total_marks "
                      "FROM subjects sub "
                      "JOIN enrollments e ON sub.id = e.subject_id "
                      "LEFT JOIN marks m ON (e.student_id = m.student_id AND e.subject_id = m.subject_id) "
                      "WHERE e.student_id = " + std::to_string(studentId) + " "
                      "ORDER BY sub.subject_code";
    return db.query(sql);
}
