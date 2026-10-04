#include "db.hpp"
#include "config.hpp"
#include <iostream>

Database::Database() : conn(nullptr), connected(false) {}

Database::~Database() {
    disconnect();
}

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

bool Database::connect() {
    std::lock_guard<std::mutex> lock(dbMutex);
    if (connected && conn != nullptr) {
        if (mysql_ping(conn) == 0) return true;
    }

    if (conn != nullptr) {
        mysql_close(conn);
        conn = nullptr;
    }

    conn = mysql_init(NULL);
    if (conn == NULL) {
        std::cerr << "[ERROR] MySQL Initialization failed." << std::endl;
        connected = false;
        return false;
    }

    Config& config = Config::getInstance();
    
    // Connect to MySQL server (Local or Amazon RDS)
    if (mysql_real_connect(conn, config.dbHost.c_str(), config.dbUser.c_str(),
                           config.dbPassword.c_str(), config.dbName.c_str(),
                           config.dbPort, NULL, 0) == NULL) {
        std::cerr << "[ERROR] MySQL Connection Failed: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        conn = nullptr;
        connected = false;
        return false;
    }

    connected = true;
    std::cout << "[INFO] Successfully connected to MySQL database: " << config.dbName << std::endl;
    return true;
}

void Database::disconnect() {
    std::lock_guard<std::mutex> lock(dbMutex);
    if (conn != nullptr) {
        mysql_close(conn);
        conn = nullptr;
    }
    connected = false;
}

bool Database::isConnected() {
    std::lock_guard<std::mutex> lock(dbMutex);
    if (!connected || conn == nullptr) return false;
    return (mysql_ping(conn) == 0);
}

std::string Database::escape(const std::string& input) {
    std::lock_guard<std::mutex> lock(dbMutex);
    if (!connected || conn == nullptr) return input;
    
    std::vector<char> escaped(input.length() * 2 + 1);
    mysql_real_escape_string(conn, escaped.data(), input.c_str(), input.length());
    return std::string(escaped.data());
}

ResultSet Database::query(const std::string& sql) {
    std::lock_guard<std::mutex> lock(dbMutex);
    ResultSet results;
    
    if (!connected || conn == nullptr) {
        if (!connect()) return results;
    }

    if (mysql_query(conn, sql.c_str()) != 0) {
        std::cerr << "[ERROR] Query failed: " << mysql_error(conn) << " SQL: " << sql << std::endl;
        return results;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res == NULL) {
        return results;
    }

    int numFields = mysql_num_fields(res);
    MYSQL_FIELD* fields = mysql_fetch_fields(res);
    MYSQL_ROW row;

    while ((row = mysql_fetch_row(res))) {
        Row rowData;
        unsigned long* lengths = mysql_fetch_lengths(res);
        for (int i = 0; i < numFields; ++i) {
            std::string fieldName = fields[i].name;
            std::string fieldValue = (row[i] != NULL) ? std::string(row[i], lengths[i]) : "";
            rowData[fieldName] = fieldValue;
        }
        results.push_back(rowData);
    }

    mysql_free_result(res);
    return results;
}

int Database::execute(const std::string& sql) {
    std::lock_guard<std::mutex> lock(dbMutex);
    if (!connected || conn == nullptr) {
        if (!connect()) return -1;
    }

    if (mysql_query(conn, sql.c_str()) != 0) {
        std::cerr << "[ERROR] Execute failed: " << mysql_error(conn) << " SQL: " << sql << std::endl;
        return -1;
    }

    return (int)mysql_affected_rows(conn);
}
