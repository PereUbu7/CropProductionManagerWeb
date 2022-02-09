#pragma once

#include "CropProductionManager/Infrastructure/statementBuilder.hpp"
#include "CropProductionManager/Infrastructure/preparedStatementBuilder.hpp"

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <string>

namespace CropProductionManager::Infrastructure
{    
    class DbContext
    {
    public:
        DbContext() : 
            driver{get_driver_instance()},
            _server{},
            _username{},
            _password{},
            _database{},
            stmtBuilder{nullptr, this},
            pstmtBuilder{this}
            {}

        DbContext* SetServer(const std::string& server) { _server = server; return this; }
        DbContext* SetUsername(const std::string& username) { _username = username; return this; }
        DbContext* SetPassword(const std::string& password) { _password = password; return this; }
        DbContext* SetDatabase(const std::string& database) { _database = database; return this; }
        DbContext* SetResultSet(sql::ResultSet *inRes) { res = std::unique_ptr<sql::ResultSet>(inRes); return this; }
        sql::ResultSet* GetResultSet() const { return res.get(); }

        DbContext* Connect()
        {
            con = std::unique_ptr<sql::Connection>(driver->connect(
                _server,
                _username,
                _password));

            con->setSchema(_database);
            return this;
        }

        StatementBuilder* CreateStatement()
        {
            assert(con && "must be connected before creating statement");
            stmtBuilder.SetStatement(con->createStatement());
            return &stmtBuilder;
        }

        PreparedStatementBuilder* PrepareStatement(const std::string& statement)
        {
            assert(con && "must be connected before creating prepared statement");
            pstmtBuilder.SetPreparedStatement(con->prepareStatement(statement));
            // pstmtBuilder.SetPreparedStatement(con->prepareStatement(statement), statement);
            return &pstmtBuilder;
        }

    private:
        std::string _server;
        std::string _username;
        std::string _password;
        std::string _database;

        sql::Driver *driver;
        std::unique_ptr<sql::Connection> con;
        std::unique_ptr<sql::ResultSet> res;
        StatementBuilder stmtBuilder;
        PreparedStatementBuilder pstmtBuilder;
    };
}