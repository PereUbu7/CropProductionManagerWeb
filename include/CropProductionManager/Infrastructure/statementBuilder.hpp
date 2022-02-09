#pragma once

#include <cppconn/statement.h>
#include <cassert>
#include <memory>

namespace CropProductionManager::Infrastructure
{
    class DbContext;
    class StatementBuilder
    {
    public:
        StatementBuilder() = default;
        StatementBuilder(sql::Statement *statement, DbContext *pwner) : 
            stmt{statement},
            owner{pwner}
        {}
        void SetStatement(sql::Statement *statement) { stmt = std::unique_ptr<sql::Statement>(statement); }
        DbContext *ExecuteQuery(const std::string &query);

    private:
        std::unique_ptr<sql::Statement> stmt;
        DbContext *owner;
    };
}