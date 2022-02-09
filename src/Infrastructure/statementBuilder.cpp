#include "CropProductionManager/Infrastructure/statementBuilder.hpp"
#include "CropProductionManager/Infrastructure/dbContext.hpp"

namespace CropProductionManager::Infrastructure
{
DbContext *StatementBuilder::ExecuteQuery(const std::string &query)
{
    assert(owner && "StatementBuilder must have an owning DbContext");
    owner->SetResultSet(stmt->executeQuery(query));
    return owner;
}
}