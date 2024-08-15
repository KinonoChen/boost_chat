#include "db_mannager.h"

DBManager::DBManager(const std::string& host, const std::string& user, const std::string& password, const std::string& db) {
    driver = sql::mysql::get_mysql_driver_instance();
    con = std::unique_ptr<sql::Connection>(driver->connect(host, user, password));
    con->setSchema(db);
}

DBManager::~DBManager() {}

void DBManager::save_user(const std::string& username, const std::string& password) {
    std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO users (username, password) VALUES (?, ?)"));
    pstmt->setString(1, username);
    pstmt->setString(2, password);
    pstmt->execute();
}

bool DBManager::validate_user(const std::string& username, const std::string& password) {
    std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT * FROM users WHERE username = ? AND password = ?"));
    pstmt->setString(1, username);
    pstmt->setString(2, password);
    std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

    return res->next();
}

bool DBManager::is_user_exists(const std::string& username) {
        std::unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement("SELECT username FROM users WHERE username = ?")
        );
        pstmt->setString(1, username);
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        return res->next();  // Return true if the user exists
    }


