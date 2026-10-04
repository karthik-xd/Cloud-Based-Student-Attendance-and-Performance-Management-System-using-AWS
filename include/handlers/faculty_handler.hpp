#ifndef FACULTY_HANDLER_HPP
#define FACULTY_HANDLER_HPP

#include "db.hpp"
#include <string>
#include <vector>
#include <unordered_map>

struct AttendanceRecord {
    int studentId;
    std::string status; // present / absent
};

class FacultyHandler {
public:
    static ResultSet getAssignedSubjects(int facultyId);
    static ResultSet getEnrolledStudentsForSubject(int subjectId);
    static ResultSet getMarksForSubject(int subjectId);

    static bool markAttendance(int facultyId, int subjectId, const std::string& date, const std::vector<AttendanceRecord>& records);
    static bool saveMarks(int studentId, int subjectId, double internal1, double internal2, double assignment, double finalMarks);
};

#endif // FACULTY_HANDLER_HPP
