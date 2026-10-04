#include "handlers/faculty_handler.hpp"
#include <iostream>

ResultSet FacultyHandler::getAssignedSubjects(int facultyId) {
    Database& db = Database::getInstance();
    std::string sql = "SELECT sub.id, sub.subject_code, sub.name, sub.department, sub.semester "
                      "FROM subjects sub "
                      "JOIN faculty_subjects fs ON sub.id = fs.subject_id "
                      "WHERE fs.faculty_id = " + std::to_string(facultyId);
    return db.query(sql);
}

ResultSet FacultyHandler::getEnrolledStudentsForSubject(int subjectId) {
    Database& db = Database::getInstance();
    std::string sql = "SELECT s.id, s.roll_number, s.name, s.department "
                      "FROM students s "
                      "JOIN enrollments e ON s.id = e.student_id "
                      "WHERE e.subject_id = " + std::to_string(subjectId) + " "
                      "ORDER BY s.roll_number";
    return db.query(sql);
}

ResultSet FacultyHandler::getMarksForSubject(int subjectId) {
    Database& db = Database::getInstance();
    std::string sql = "SELECT s.id as student_id, s.roll_number, s.name, "
                      "COALESCE(m.internal1_marks, 0) as internal1_marks, "
                      "COALESCE(m.internal2_marks, 0) as internal2_marks, "
                      "COALESCE(m.assignment_marks, 0) as assignment_marks, "
                      "COALESCE(m.final_marks, 0) as final_marks "
                      "FROM students s "
                      "JOIN enrollments e ON s.id = e.student_id "
                      "LEFT JOIN marks m ON (s.id = m.student_id AND m.subject_id = " + std::to_string(subjectId) + ") "
                      "WHERE e.subject_id = " + std::to_string(subjectId) + " "
                      "ORDER BY s.roll_number";
    return db.query(sql);
}

bool FacultyHandler::markAttendance(int facultyId, int subjectId, const std::string& date, const std::vector<AttendanceRecord>& records) {
    Database& db = Database::getInstance();
    std::string cleanDate = db.escape(date);

    for (const auto& rec : records) {
        std::string sql = "INSERT INTO attendance (student_id, subject_id, faculty_id, date, status) VALUES (" +
                          std::to_string(rec.studentId) + ", " + std::to_string(subjectId) + ", " + std::to_string(facultyId) + ", '" +
                          cleanDate + "', '" + db.escape(rec.status) + "') " +
                          "ON DUPLICATE KEY UPDATE status = '" + db.escape(rec.status) + "', faculty_id = " + std::to_string(facultyId);
        db.execute(sql);
    }
    return true;
}

bool FacultyHandler::saveMarks(int studentId, int subjectId, double internal1, double internal2, double assignment, double finalMarks) {
    Database& db = Database::getInstance();
    std::string sql = "INSERT INTO marks (student_id, subject_id, internal1_marks, internal2_marks, assignment_marks, final_marks) VALUES (" +
                      std::to_string(studentId) + ", " + std::to_string(subjectId) + ", " +
                      std::to_string(internal1) + ", " + std::to_string(internal2) + ", " +
                      std::to_string(assignment) + ", " + std::to_string(finalMarks) + ") " +
                      "ON DUPLICATE KEY UPDATE internal1_marks = " + std::to_string(internal1) + ", " +
                      "internal2_marks = " + std::to_string(internal2) + ", " +
                      "assignment_marks = " + std::to_string(assignment) + ", " +
                      "final_marks = " + std::to_string(finalMarks);
    return (db.execute(sql) >= 0);
}
