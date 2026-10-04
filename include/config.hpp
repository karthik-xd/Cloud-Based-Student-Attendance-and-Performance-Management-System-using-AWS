#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <unordered_map>

class Config {
public:
    static Config& getInstance();

    // Load configuration from .env file or system environment
    void loadEnv(const std::string& envFilePath = ".env");

    std::string get(const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;

    // Direct Accessors for frequently used configurations
    int port;
    std::string secretKey;
    std::string dbHost;
    int dbPort;
    std::string dbUser;
    std::string dbPassword;
    std::string dbName;
    std::string awsRegion;
    std::string s3BucketName;
    std::string snsTopicArn;

private:
    Config() = default;
    std::unordered_map<std::string, std::string> envMap;
    std::string trim(const std::string& str);
};

#endif // CONFIG_HPP
