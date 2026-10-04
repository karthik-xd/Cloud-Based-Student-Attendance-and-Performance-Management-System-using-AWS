#include "handlers/admin_handler.hpp"
#include "auth.hpp"
#include <iostream>

ResultSet AdminHandler::getAllFaculty() {
    Database& db = Database::getInstance();
    return db.query("SELECT f.id, f.faculty_code, f.name, f.department, u.email, u.id as user_id "
                    "FROM faculty f JOIN users u ON f.user_id = u.id ORDER BY f.name");
}

ResultSet AdminHandler::getAllStudents() {
    Database& db = Database::getInstance();
    return db.query("SELECT s.id, s.roll_number, s.name, s.department, s.semester, u.email, u.id as user_id "
                    "FROM students s JOIN users u ON s.user_id = u.id ORDER BY s.roll_number");
}

ResultSet AdminHandler::getAllSubjects() {
    Database& db = Database::getInstance();
    return db.query("SELECT id, subject_code, name, department, semester FROM subjects ORDER BY subject_code");
}

ResultSet AdminHandler::getFacultySubjectAssignments() {
    Database& db = Database::getInstance();
    return db.query("SELECT fs.id, f.name as faculty_name, sub.subject_code, sub.name as subject_name "
                    "FROM faculty_subjects fs "
                    "JOIN faculty f ON fs.faculty_id = f.id "
                    "JOIN subjects sub ON fs.subject_id = sub.id");
}

bool AdminHandler::addFaculty(const std::string& email, const std::string& name, const std::string& code, const std::string& dept) {
    Database& db = Database::getInstance();
    AuthManager& auth = AuthManager::getInstance();

    std::string defaultHash = auth.hashPassword("Faculty@123");
    std::string sqlUser = "INSERT INTO users (email, password_hash, role) VALUES ('" + 
                          db.escape(email) + "', '" + defaultHash + "', 'faculty')";
    if (db.execute(sqlUser) <= 0) return false;

    ResultSet uRes = db.query("SELECT id FROM users WHERE email = '" + db.escape(email) + "'");
    if (uRes.empty()) return false;

    std::string userId = uRes[0]["id"];
    std::string sqlFaculty = "INSERT INTO faculty (user_id, faculty_code, name, department) VALUES (" + 
                             userId + ", '" + db.escape(code) + "', '" + db.escape(name) + "', '" + db.escape(dept) + "')";
    return (db.execute(sqlFaculty) > 0);
}

bool AdminHandler::addStudent(const std::string& email, const std::string& name, const std::string& roll, const std::string& dept, int semester) {
    Database& db = Database::getInstance();
    AuthManager& auth = AuthManager::getInstance();

    std::string defaultHash = auth.hashPassword("Student@123");
    std::string sqlUser = "INSERT INTO users (email, password_hash, role) VALUES ('" + 
                          db.escape(email) + "', '" + defaultHash + "', 'student')";
    if (db.execute(sqlUser) <= 0) return false;

    ResultSet uRes = db.query("SELECT id FROM users WHERE email = '" + db.escape(email) + "'");
    if (uRes.empty()) return false;

    std::string userId = uRes[0]["id"];
    std::string sqlStudent = "INSERT INTO students (user_id, roll_number, name, department, semester) VALUES (" + 
                             userId + ", '" + db.escape(roll) + "', '" + db.escape(name) + "', '" + db.escape(dept) + "', " + std::to_string(semester) + ")";
    return (db.execute(sqlStudent) > 0);
}

bool AdminHandler::addSubject(const std::string& code, const std::string& name, const std::string& dept, int semester) {
    Database& db = Database::getInstance();
    std::string sql = "INSERT INTO subjects (subject_code, name, department, semester) VALUES ('" + 
                      db.escape(code) + "', '" + db.escape(name) + "', '" + db.escape(dept) + "', " + std::to_string(semester) + ")";
    return (db.execute(sql) > 0);
}

bool AdminHandler::assignSubjectToFaculty(int facultyId, int subjectId) {
    Database& db = Database::getInstance();
    std::string sql = "INSERT INTO faculty_subjects (faculty_id, subject_id) VALUES (" + 
                      std::to_string(facultyId) + ", " + std::to_string(subjectId) + ")";
    return (db.execute(sql) > 0);
}

bool AdminHandler::deleteUser(int userId) {
    Database& db = Database::getInstance();
    std::string sql = "DELETE FROM users WHERE id = " + std::to_string(userId);
    return (db.execute(sql) > 0);
}

bool AdminHandler::deleteSubject(int subjectId) {
    Database& db = Database::getInstance();
    std::string sql = "DELETE FROM subjects WHERE id = " + std::to_string(subjectId);
    return (db.execute(sql) > 0);
}
