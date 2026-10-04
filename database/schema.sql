-- Database Schema for Student Attendance & Performance Management System (C++ Backend)
-- Database Engine: MySQL 8.0 on Amazon RDS / Local MySQL

CREATE DATABASE IF NOT EXISTS student_management_db;
USE student_management_db;

-- Drop existing tables to ensure clean re-initialization
DROP TABLE IF EXISTS marks;
DROP TABLE IF EXISTS attendance;
DROP TABLE IF EXISTS enrollments;
DROP TABLE IF EXISTS faculty_subjects;
DROP TABLE IF EXISTS subjects;
DROP TABLE IF EXISTS students;
DROP TABLE IF EXISTS faculty;
DROP TABLE IF EXISTS users;

-- 1. Users table (authentication & system roles)
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    email VARCHAR(100) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    role ENUM('admin', 'faculty', 'student') NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_user_email (email),
    INDEX idx_user_role (role)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 2. Faculty profile table
CREATE TABLE faculty (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL UNIQUE,
    faculty_code VARCHAR(20) NOT NULL UNIQUE,
    name VARCHAR(100) NOT NULL,
    department VARCHAR(50) NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    INDEX idx_faculty_code (faculty_code),
    INDEX idx_faculty_dept (department)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 3. Students profile table
CREATE TABLE students (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL UNIQUE,
    roll_number VARCHAR(20) NOT NULL UNIQUE,
    name VARCHAR(100) NOT NULL,
    department VARCHAR(50) NOT NULL,
    semester INT NOT NULL CHECK (semester BETWEEN 1 AND 8),
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,
    INDEX idx_student_roll (roll_number),
    INDEX idx_student_dept_sem (department, semester)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 4. Subjects table
CREATE TABLE subjects (
    id INT AUTO_INCREMENT PRIMARY KEY,
    subject_code VARCHAR(20) NOT NULL UNIQUE,
    name VARCHAR(100) NOT NULL,
    department VARCHAR(50) NOT NULL,
    semester INT NOT NULL CHECK (semester BETWEEN 1 AND 8),
    INDEX idx_subject_code (subject_code),
    INDEX idx_subject_dept_sem (department, semester)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 5. Faculty to Subject Assignment mapping table
CREATE TABLE faculty_subjects (
    id INT AUTO_INCREMENT PRIMARY KEY,
    faculty_id INT NOT NULL,
    subject_id INT NOT NULL,
    FOREIGN KEY (faculty_id) REFERENCES faculty(id) ON DELETE CASCADE,
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE,
    UNIQUE KEY uq_faculty_subject (faculty_id, subject_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 6. Student Enrollments table
CREATE TABLE enrollments (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    subject_id INT NOT NULL,
    FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE,
    UNIQUE KEY uq_student_subject (student_id, subject_id),
    INDEX idx_enrollment_student (student_id),
    INDEX idx_enrollment_subject (subject_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 7. Daily Attendance table
CREATE TABLE attendance (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    subject_id INT NOT NULL,
    faculty_id INT NOT NULL,
    date DATE NOT NULL,
    status ENUM('present', 'absent') NOT NULL,
    FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE,
    FOREIGN KEY (faculty_id) REFERENCES faculty(id) ON DELETE CASCADE,
    UNIQUE KEY uq_student_subject_date (student_id, subject_id, date),
    INDEX idx_attendance_student_subject (student_id, subject_id),
    INDEX idx_attendance_date (date),
    INDEX idx_attendance_status (status)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 8. Student Marks / Academic Performance table
CREATE TABLE marks (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    subject_id INT NOT NULL,
    internal1_marks DECIMAL(5,2) DEFAULT 0.00 CHECK (internal1_marks BETWEEN 0 AND 20),
    internal2_marks DECIMAL(5,2) DEFAULT 0.00 CHECK (internal2_marks BETWEEN 0 AND 20),
    assignment_marks DECIMAL(5,2) DEFAULT 0.00 CHECK (assignment_marks BETWEEN 0 AND 10),
    final_marks DECIMAL(5,2) DEFAULT 0.00 CHECK (final_marks BETWEEN 0 AND 50),
    FOREIGN KEY (student_id) REFERENCES students(id) ON DELETE CASCADE,
    FOREIGN KEY (subject_id) REFERENCES subjects(id) ON DELETE CASCADE,
    UNIQUE KEY uq_student_marks_subject (student_id, subject_id),
    INDEX idx_marks_student (student_id),
    INDEX idx_marks_subject (subject_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
