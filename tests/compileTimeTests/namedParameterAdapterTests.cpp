#include "CropProductionManager/Infrastructure/namedParametersAdapter.hpp"

namespace CropProductionManager::Infrastructure
{
    /* Compile time unit test of NamedParameterAdapeter */
    static constexpr auto npa = NamedParameterAdapter(R"(
            SELECT 
                id, name, variety, batch 
            FROM 
                crop    
            LEFT JOIN
                event
            ON event.crop = crop.id 
                AND event.timestamp < :startDate
                AND event.timestamp > :endDate
            WHERE 
                id = :id
            AND batch = :batch
            AND family = :family
)");

    constexpr compiletime::string<172> unitTestStatement(" SELECT id, name, variety, batch FROM crop LEFT JOIN event ON event.crop = crop.id AND event.timestamp < ? AND event.timestamp > ? WHERE id = ? AND batch = ? AND family = ?");
    constexpr auto unitTestAcutal = npa.GetCompileTimeStatement();
    static_assert(unitTestStatement.compare(unitTestAcutal, 172), "NamedParameterAdapter return wrong statement");

    constexpr auto startDateIndex = npa["startDate"];
    constexpr auto endDateIndex = npa["endDate"];
    constexpr auto idIndex = npa["id"];
    constexpr auto batchIndex = npa["batch"];
    constexpr auto familyIndex = npa["family"];

    static_assert(startDateIndex == 1, "StartDate has wrong index");
    static_assert(endDateIndex == 2, "EndDate has wrong index");
    static_assert(idIndex == 3, "Id has wrong index");
    static_assert(batchIndex == 4, "Batch has wrong index");
    static_assert(familyIndex == 5, "Family has wrong index");
}