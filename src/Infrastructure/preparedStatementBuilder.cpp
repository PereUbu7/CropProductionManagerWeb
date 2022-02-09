#include "CropProductionManager/Infrastructure/preparedStatementBuilder.hpp"
#include "CropProductionManager/Infrastructure/dbContext.hpp"

namespace CropProductionManager::Infrastructure
{
DbContext *PreparedStatementBuilder::ExecuteQuery()
{
    assert(owner && "StatementBuilder must have an owning DbContext");
    owner->SetResultSet(stmt->executeQuery());
    return owner;
}
}