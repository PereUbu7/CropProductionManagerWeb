#pragma once

#include "CropProductionManager/Infrastructure/namedParametersAdapter.hpp"

#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <cassert>
#include <memory>
#include <string>

namespace CropProductionManager::Infrastructure
{
    class DbContext;
    class PreparedStatementBuilder
    {
    public:
        PreparedStatementBuilder() = default;
        PreparedStatementBuilder(DbContext *pwner) :
            stmt{},
            owner{pwner}
        {}
        void SetPreparedStatement(sql::PreparedStatement *statement) { stmt = std::unique_ptr<sql::PreparedStatement>(statement); }
        DbContext *ExecuteQuery();
        DbContext *ExecuteUpdate();
        // PreparedStatementBuilder *BindInt(const std::string& name, int32_t value) { namedParametersAdapter.BindInt(name, value); return this; }
        PreparedStatementBuilder *SetInt(unsigned int index, int32_t value) { stmt->setInt(index, value); return this;};
        PreparedStatementBuilder *SetString(unsigned int index, const sql::SQLString& value) { stmt->setString(index, value); return this; };

    private:
        std::unique_ptr<sql::PreparedStatement> stmt;
        // std::string stmt;
        DbContext *owner;
    };
}