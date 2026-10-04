#ifndef STUDENT_HANDLER_HPP
#define STUDENT_HANDLER_HPP

#include "db.hpp"
#include "services/attendance_service.hpp"
#include <string>

class StudentHandler {
public:
    static StudentOverallAttendance getStudentDashboardData(int studentId);
    static ResultSet getStudentMarks(int studentId);
};

#endif // STUDENT_HANDLER_HPP
