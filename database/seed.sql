-- Seed Data for Student Attendance & Performance Management System (C++ Backend)
-- Database: MySQL on Amazon RDS / Local MySQL

-- ============================================================================
-- DEMO CREDENTIALS FOR VIVA PRESENTATION & LIVE DEMO:
-- System Administrator: admin@university.edu / Admin@123
-- Faculty Members:      prof.sharma@university.edu / Faculty@123 (Dr. Rajesh Sharma)
--                       prof.verma@university.edu / Faculty@123 (Prof. Anita Verma)
--                       prof.gupta@university.edu / Faculty@123 (Dr. Suresh Gupta)
-- Students:             student1@university.edu to student15@university.edu / Student@123
--
-- STUDENTS WITH LOW ATTENDANCE (< 75% TO DEMONSTRATE LAMBDA + SNS ALERTS):
--   1. student13@university.edu (Rahul Verma - CS202413) -> Attendance: ~50%
--   2. student14@university.edu (Priya Singh - CS202414) -> Attendance: ~60%
--   3. student15@university.edu (Amit Patel - EC202405)  -> Attendance: ~65%
-- ============================================================================

USE student_management_db;

-- 1. Insert System Users (1 Admin, 3 Faculty, 15 Students)
-- Passwords formatted as sha256:<salt>:<hex_hash> using OpenSSL / standard SHA256
INSERT INTO users (id, email, password_hash, role) VALUES
(1, 'admin@university.edu', 'sha256:univ2026:52ac81e20f35e6a646f3c82d74a27855c8223870b9dafad748b277f978654d19', 'admin'),
(2, 'prof.sharma@university.edu', 'sha256:univ2026:2403fc5397fad2632c5aa49516d07562753c3a57a6ace1a37f7d84802606bc68', 'faculty'),
(3, 'prof.verma@university.edu', 'sha256:univ2026:2403fc5397fad2632c5aa49516d07562753c3a57a6ace1a37f7d84802606bc68', 'faculty'),
(4, 'prof.gupta@university.edu', 'sha256:univ2026:2403fc5397fad2632c5aa49516d07562753c3a57a6ace1a37f7d84802606bc68', 'faculty'),
(5, 'student1@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(6, 'student2@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(7, 'student3@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(8, 'student4@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(9, 'student5@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(10, 'student6@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(11, 'student7@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(12, 'student8@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(13, 'student9@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(14, 'student10@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(15, 'student11@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(16, 'student12@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(17, 'student13@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(18, 'student14@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student'),
(19, 'student15@university.edu', 'sha256:univ2026:5a0a0bba334b31a7af359b28abf7f82f420b0683e57a0d2489adef728ccb8660', 'student');

-- 2. Insert Faculty Profiles
INSERT INTO faculty (id, user_id, faculty_code, name, department) VALUES
(1, 2, 'FAC101', 'Dr. Rajesh Sharma', 'Computer Science'),
(2, 3, 'FAC102', 'Prof. Anita Verma', 'Computer Science'),
(3, 4, 'FAC103', 'Dr. Suresh Gupta', 'Electronics');

-- 3. Insert Student Profiles
INSERT INTO students (id, user_id, roll_number, name, department, semester) VALUES
(1, 5, 'CS202401', 'Aarav Kumar', 'Computer Science', 6),
(2, 6, 'CS202402', 'Ananya Roy', 'Computer Science', 6),
(3, 7, 'CS202403', 'Rohan Mehta', 'Computer Science', 6),
(4, 8, 'CS202404', 'Sneha Reddy', 'Computer Science', 6),
(5, 9, 'CS202405', 'Vikram Joshi', 'Computer Science', 6),
(6, 10, 'CS202406', 'Kavya Nair', 'Computer Science', 6),
(7, 11, 'CS202407', 'Aditya Sharma', 'Computer Science', 6),
(8, 12, 'CS202408', 'Diya Kapoor', 'Computer Science', 6),
(9, 13, 'CS202409', 'Siddharth Rao', 'Computer Science', 6),
(10, 14, 'CS202410', 'Neha Agarwal', 'Computer Science', 6),
(11, 15, 'CS202411', 'Manish Pandey', 'Computer Science', 6),
(12, 16, 'CS202412', 'Pooja Bhatia', 'Computer Science', 6),
(13, 17, 'CS202413', 'Rahul Verma', 'Computer Science', 6),   -- Low attendance student (~50%)
(14, 18, 'CS202414', 'Priya Singh', 'Computer Science', 6),   -- Low attendance student (~60%)
(15, 19, 'EC202405', 'Amit Patel', 'Electronics', 6);         -- Low attendance student (~65%)

-- 4. Insert Academic Subjects
INSERT INTO subjects (id, subject_code, name, department, semester) VALUES
(1, 'CS301', 'Cloud Computing', 'Computer Science', 6),
(2, 'CS302', 'Database Systems', 'Computer Science', 6),
(3, 'CS303', 'Data Structures & Algorithms', 'Computer Science', 6),
(4, 'CS304', 'Computer Networks', 'Computer Science', 6),
(5, 'EC301', 'Digital Electronics', 'Electronics', 6);

-- 5. Faculty Subject Assignments
INSERT INTO faculty_subjects (faculty_id, subject_id) VALUES
(1, 1), -- Dr. Rajesh Sharma -> Cloud Computing
(1, 4), -- Dr. Rajesh Sharma -> Computer Networks
(2, 2), -- Prof. Anita Verma -> Database Systems
(2, 3), -- Prof. Anita Verma -> Data Structures
(3, 5); -- Dr. Suresh Gupta -> Digital Electronics

-- 6. Student Course Enrollments
INSERT INTO enrollments (student_id, subject_id) VALUES
(1, 1), (2, 1), (3, 1), (4, 1), (5, 1), (6, 1), (7, 1), (8, 1), (9, 1), (10, 1), (11, 1), (12, 1), (13, 1), (14, 1),
(1, 2), (2, 2), (3, 2), (4, 2), (5, 2), (6, 2), (7, 2), (8, 2), (9, 2), (10, 2), (11, 2), (12, 2), (13, 2), (14, 2),
(1, 3), (2, 3), (3, 3), (4, 3), (5, 3), (6, 3), (7, 3), (8, 3), (9, 3), (10, 3), (11, 3), (12, 3), (13, 3), (14, 3),
(15, 5);

-- 7. Sample Attendance Records (10 sessions per course)
INSERT INTO attendance (student_id, subject_id, faculty_id, date, status) VALUES
-- Date 2026-09-01
(1, 1, 1, '2026-09-01', 'present'), (2, 1, 1, '2026-09-01', 'present'), (3, 1, 1, '2026-09-01', 'present'),
(4, 1, 1, '2026-09-01', 'present'), (5, 1, 1, '2026-09-01', 'present'), (6, 1, 1, '2026-09-01', 'present'),
(7, 1, 1, '2026-09-01', 'present'), (8, 1, 1, '2026-09-01', 'present'), (9, 1, 1, '2026-09-01', 'present'),
(10, 1, 1, '2026-09-01', 'present'), (11, 1, 1, '2026-09-01', 'present'), (12, 1, 1, '2026-09-01', 'present'),
(13, 1, 1, '2026-09-01', 'absent'), (14, 1, 1, '2026-09-01', 'absent'), (15, 5, 3, '2026-09-01', 'absent'),

-- Date 2026-09-02
(1, 1, 1, '2026-09-02', 'present'), (2, 1, 1, '2026-09-02', 'present'), (3, 1, 1, '2026-09-02', 'present'),
(4, 1, 1, '2026-09-02', 'present'), (5, 1, 1, '2026-09-02', 'present'), (6, 1, 1, '2026-09-02', 'present'),
(7, 1, 1, '2026-09-02', 'present'), (8, 1, 1, '2026-09-02', 'present'), (9, 1, 1, '2026-09-02', 'present'),
(10, 1, 1, '2026-09-02', 'present'), (11, 1, 1, '2026-09-02', 'present'), (12, 1, 1, '2026-09-02', 'present'),
(13, 1, 1, '2026-09-02', 'present'), (14, 1, 1, '2026-09-02', 'present'), (15, 5, 3, '2026-09-02', 'present'),

-- Date 2026-09-03
(1, 1, 1, '2026-09-03', 'present'), (2, 1, 1, '2026-09-03', 'present'), (3, 1, 1, '2026-09-03', 'present'),
(4, 1, 1, '2026-09-03', 'present'), (5, 1, 1, '2026-09-03', 'present'), (6, 1, 1, '2026-09-03', 'present'),
(7, 1, 1, '2026-09-03', 'present'), (8, 1, 1, '2026-09-03', 'present'), (9, 1, 1, '2026-09-03', 'present'),
(10, 1, 1, '2026-09-03', 'present'), (11, 1, 1, '2026-09-03', 'present'), (12, 1, 1, '2026-09-03', 'present'),
(13, 1, 1, '2026-09-03', 'absent'), (14, 1, 1, '2026-09-03', 'absent'), (15, 5, 3, '2026-09-03', 'absent'),

-- Date 2026-09-04
(1, 1, 1, '2026-09-04', 'present'), (2, 1, 1, '2026-09-04', 'present'), (3, 1, 1, '2026-09-04', 'present'),
(4, 1, 1, '2026-09-04', 'present'), (5, 1, 1, '2026-09-04', 'present'), (6, 1, 1, '2026-09-04', 'present'),
(7, 1, 1, '2026-09-04', 'present'), (8, 1, 1, '2026-09-04', 'present'), (9, 1, 1, '2026-09-04', 'present'),
(10, 1, 1, '2026-09-04', 'present'), (11, 1, 1, '2026-09-04', 'present'), (12, 1, 1, '2026-09-04', 'present'),
(13, 1, 1, '2026-09-04', 'present'), (14, 1, 1, '2026-09-04', 'absent'), (15, 5, 3, '2026-09-04', 'present'),

-- Sessions from 2026-09-05 to 2026-09-10
(1, 1, 1, '2026-09-05', 'present'), (2, 1, 1, '2026-09-05', 'present'), (13, 1, 1, '2026-09-05', 'absent'), (14, 1, 1, '2026-09-05', 'present'), (15, 5, 3, '2026-09-05', 'absent'),
(1, 1, 1, '2026-09-06', 'present'), (2, 1, 1, '2026-09-06', 'present'), (13, 1, 1, '2026-09-06', 'absent'), (14, 1, 1, '2026-09-06', 'absent'), (15, 5, 3, '2026-09-06', 'present'),
(1, 1, 1, '2026-09-07', 'present'), (2, 1, 1, '2026-09-07', 'present'), (13, 1, 1, '2026-09-07', 'present'), (14, 1, 1, '2026-09-07', 'present'), (15, 5, 3, '2026-09-07', 'absent'),
(1, 1, 1, '2026-09-08', 'present'), (2, 1, 1, '2026-09-08', 'present'), (13, 1, 1, '2026-09-08', 'absent'), (14, 1, 1, '2026-09-08', 'absent'), (15, 5, 3, '2026-09-08', 'present'),
(1, 1, 1, '2026-09-09', 'present'), (2, 1, 1, '2026-09-09', 'present'), (13, 1, 1, '2026-09-09', 'absent'), (14, 1, 1, '2026-09-09', 'present'), (15, 5, 3, '2026-09-09', 'absent'),
(1, 1, 1, '2026-09-10', 'present'), (2, 1, 1, '2026-09-10', 'present'), (13, 1, 1, '2026-09-10', 'present'), (14, 1, 1, '2026-09-10', 'absent'), (15, 5, 3, '2026-09-10', 'present');

-- 8. Sample Marks / Grades
INSERT INTO marks (student_id, subject_id, internal1_marks, internal2_marks, assignment_marks, final_marks) VALUES
(1, 1, 18.50, 19.00, 9.50, 45.00),
(2, 1, 17.00, 18.00, 9.00, 42.50),
(3, 1, 16.00, 15.50, 8.50, 39.00),
(4, 1, 19.00, 19.50, 10.00, 48.00),
(5, 1, 14.00, 15.00, 8.00, 36.00),
(6, 1, 18.00, 17.50, 9.00, 44.00),
(7, 1, 15.50, 16.00, 8.50, 40.00),
(8, 1, 20.00, 19.50, 10.00, 49.00),
(9, 1, 13.00, 14.00, 7.50, 33.00),
(10, 1, 17.50, 18.00, 9.00, 43.00),
(11, 1, 16.50, 17.00, 8.50, 41.00),
(12, 1, 19.00, 18.50, 9.50, 46.50),
(13, 1, 10.00, 11.50, 6.00, 25.00),  -- Low performance
(14, 1, 11.50, 12.00, 6.50, 28.00),  -- Low performance
(15, 5, 12.00, 11.00, 7.00, 29.50);  -- Low performance
