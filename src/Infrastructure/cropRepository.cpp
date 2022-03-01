#include "CropProductionManager/Infrastructure/cropRepository.h"
#include "CropProductionManager/Infrastructure/namedParametersAdapter.hpp"

namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    CropRepository::CropRepository(IConfiguration config) : // context{},
                                                            _config{config}
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

    // GET
    std::vector<Crop> CropRepository::Get() const
    {
        auto retValue = std::vector<Crop>{};
        try
        {
            static constexpr auto npa = NamedParameterAdapter(R"(
            SELECT 
                id, name, variety, batch 
            FROM 
                crop 
            WHERE 1=:one
)");

            auto context = DbContext{};

            auto res = context
                           .SetServer(_config["server"])
                           ->SetUsername(_config["username"])
                           ->SetPassword(_config["password"])
                           ->SetDatabase(_config["database"])
                           ->Connect()
                           ->PrepareStatement(npa.GetStatement())
                           ->SetInt(npa["one"], 1)
                           ->ExecuteQuery()
                           ->GetResultSet();

            while (res->next())
            {
                retValue.push_back(Crop{
                    res->getInt("id"),
                    res->getString("name"),
                    res->getString("variety"),
                    res->getInt("batch")});
                std::cout << "id:" << res->getString("id") << '\t';
                std::cout << "name:" << res->getString("name") << '\t';
                std::cout << "variety:" << res->getString("variety") << '\t';
                std::cout << "batch:" << res->getString("batch") << '\n';
            }
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "In cropRepository::Get<Crop>()\n";
            std::cerr << e.what() << '\n';
            std::cerr << " (MySQL error code: " << e.getErrorCode() << '\n';
            std::cerr << ", SQLState: " << e.getSQLState() << " )" << '\n';
        }

        return retValue;
    }
    Crop CropRepository::Get(const int id) const
    {
        auto foundCrops{std::find_if(begin(crops), end(crops), [id](Crop c)
                                     { return c.id == id; })};
        if (foundCrops != end(crops))
        {
            return foundCrops[0];
        }
        throw std::invalid_argument("not found");
    }
    // POST
    void CropRepository::Post(Crop crop)
    {
        crops.push_back(crop);
    }
    // PUT
    void CropRepository::Put(Crop crop)
    {
        auto cropToModify{
            std::find_if(begin(crops), end(crops), [crop](Crop c)
                         { return c.id == crop.id; })};

        if (cropToModify != end(crops))
        {
            (*cropToModify).Update(crop);
        }
    }
    // DELETE
    void CropRepository::Remove(int id)
    {
        auto it = std::find_if(begin(crops), end(crops), [id](Crop crop)
                               { return crop.id == id; });

        if (it != end(crops))
        {
            crops.erase(it);
        }
    }
}