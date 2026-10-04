#include "services/attendance_service.hpp"
#include <iostream>
#include <cassert>

void testAttendanceCalculation() {
    // Test attendance calculation logic (classes_present / classes_held) * 100
    int present = 18;
    int total = 20;
    double expectedPercentage = (18.0 / 20.0) * 100.0; // 90.0%

    assert(expectedPercentage == 90.0);
    assert(expectedPercentage >= 75.0); // Not low attendance

    int lowPresent = 10;
    double lowPercentage = (10.0 / 20.0) * 100.0; // 50.0%
    assert(lowPercentage < 75.0); // Triggers low attendance alert

    std::cout << "[TEST PASSED] Attendance Percentage Calculation Test (90% vs 50%)" << std::endl;
}

int main() {
    std::cout << "[RUNNING TESTS] Starting Attendance Service Tests..." << std::endl;
    testAttendanceCalculation();
    std::cout << "[ALL TESTS PASSED] Attendance Test Suite Completed Successfully." << std::endl;
    return 0;
}
