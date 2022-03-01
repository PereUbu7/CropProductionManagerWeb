#include "CropProductionManager/Infrastructure/cropRepository.h"
#include "CropProductionManager/Infrastructure/namedParametersAdapter.hpp"

namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    CropRepository::CropRepository(IConfiguration config) : 
        // context{},
        _config{config}
    {
        /* Compile time unit test of NamedParameterAdapeter */
        static constexpr auto npa = NamedParameterAdapter(R"(
            SELECT 
                id, name, variety, batch 
            FROM 
                crop 
            WHERE 
                id = :id
            AND batch = :batch
)")
        .BindInt("id", 3)
        .BindInt("batch", 2);

        constexpr compiletime::string<69> unitTestStatement(" SELECT id, name, variety, batch FROM crop WHERE id = ? AND batch = ?");
        constexpr auto unitTestAcutal = npa.GetCompileTimeStatement();
        static_assert(unitTestStatement.compare(unitTestAcutal, 69), "NamedParameterAdapter return wrong statement");

        constexpr auto bindings = npa.GetBindings<int>();
        static_assert(bindings[0].set, "Index 0 is not set");
        static_assert(bindings[0].index == 0, "Index 0 is set to wrong index");
        static_assert(bindings[0].value == 3, "Index 0 is set to wrong value");
        static_assert(bindings[1].set, "Index 1 is not set");
        static_assert(bindings[1].index == 1, "Index 1 is set to wrong index");
        static_assert(bindings[1].value == 2, "Index 1 is set to wrong value");
        static_assert(!bindings[2].set, "Index 2 is unproperly set");
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
)")
            .BindInt("one", 1);

            auto context = DbContext{};

            auto res = context
                           .SetServer(_config["server"])
                           ->SetUsername(_config["username"])
                           ->SetPassword(_config["password"])
                           ->SetDatabase(_config["database"])
                           ->Connect()
                           ->PrepareAndBind(npa)
                           ->GetResultSet();

            while (res->next())
            {
                auto j = res->getString("");
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