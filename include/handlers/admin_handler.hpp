#ifndef ADMIN_HANDLER_HPP
#define ADMIN_HANDLER_HPP

#include "db.hpp"
#include <string>
#include <unordered_map>

class AdminHandler {
public:
    static ResultSet getAllFaculty();
    static ResultSet getAllStudents();
    static ResultSet getAllSubjects();
    static ResultSet getFacultySubjectAssignments();

    static bool addFaculty(const std::string& email, const std::string& name, const std::string& code, const std::string& dept);
    static bool addStudent(const std::string& email, const std::string& name, const std::string& roll, const std::string& dept, int semester);
    static bool addSubject(const std::string& code, const std::string& name, const std::string& dept, int semester);
    static bool assignSubjectToFaculty(int facultyId, int subjectId);

    static bool deleteUser(int userId);
    static bool deleteSubject(int subjectId);
};

#endif // ADMIN_HANDLER_HPP
