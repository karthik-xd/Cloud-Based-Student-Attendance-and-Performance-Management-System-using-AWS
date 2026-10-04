cmake_minimum_required(VERSION 3.14)
project(CloudStudentManagement LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find System Dependencies
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
)

# MySQL Client Library link check (libmysqlclient or mariadb client)
find_library(MYSQL_LIB NAMES mysqlclient mariadb mariadbclient)
if(NOT MYSQL_LIB)
    message(FATAL_ERROR "MySQL client library (libmysqlclient / libmariadb) not found!")
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/config.cpp
    src/db.cpp
    src/auth.cpp
    src/services/attendance_service.cpp
    src/services/s3_service.cpp
    src/services/sns_service.cpp
    src/handlers/auth_handler.cpp
    src/handlers/admin_handler.cpp
    src/handlers/faculty_handler.cpp
    src/handlers/student_handler.cpp
    src/handlers/report_handler.cpp
    src/handlers/health_handler.cpp
)

# Application executable
add_executable(cloud_student_system ${SOURCES})

# Target link libraries
target_link_libraries(cloud_student_system PRIVATE
    ${MYSQL_LIB}
    OpenSSL::Crypto
    OpenSSL::SSL
    CURL::libcurl
    pthread
)

# Compiler definitions & optimizations
target_compile_options(cloud_student_system PRIVATE -Wall -Wextra -O2)
