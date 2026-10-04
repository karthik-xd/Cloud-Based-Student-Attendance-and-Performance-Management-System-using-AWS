#ifndef DB_HPP
#define DB_HPP

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <memory>

// Database Row structure represented as key-value pairs (column_name -> string_value)
using Row = std::unordered_map<std::string, std::string>;
using ResultSet = std::vector<Row>;

class Database {
public:
    static Database& getInstance();

    // Connect to MySQL server / Amazon RDS instance
    bool connect();
    void disconnect();

    // Execute SQL query that returns data (SELECT)
    ResultSet query(const std::string& sql);

    // Execute DML statement (INSERT, UPDATE, DELETE) returning affected rows count
    int execute(const std::string& sql);

    // Helper to sanitize/escape user inputs preventing SQL injection
    std::string escape(const std::string& input);

    // Check database connection health
    bool isConnected();

private:
    Database();
    ~Database();
    
    MYSQL* conn;
    std::mutex dbMutex;
    bool connected;
};

#endif // DB_HPP
