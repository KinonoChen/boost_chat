#ifndef DB_MANNAGER_H
#define DB_MANNAGER_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

class DBManager
{
private:
    sql::mysql::MySQL_Driver* driver;
    std::unique_ptr<sql::Connection> con;

public:
    DBManager(const std::string& host, const std::string& user, const std::string& password, const std::string& db);
    ~DBManager();

    void save_user(const std::string& username, const std::string& password);
    bool validate_user(const std::string& username, const std::string& password);
    bool is_user_exists(const std::string& username);
};
#endif // DB_MANNAGER_H

