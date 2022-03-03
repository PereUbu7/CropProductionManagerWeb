#include "CropProductionManager/Infrastructure/cropRepository.h"
#include "CropProductionManager/Infrastructure/namedParametersAdapter.hpp"

namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    CropRepository::CropRepository(IConfiguration config) : 
        _config{config}
    {}

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
                retValue.push_back(Crop::FromSqlResultSet(*res));
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
        try
        {
            static constexpr auto npa = NamedParameterAdapter(R"(
            SELECT 
                id, name, variety, batch 
            FROM 
                crop 
            WHERE id=:id
)");

            auto context = DbContext{};

            auto res = context
                           .SetServer(_config["server"])
                           ->SetUsername(_config["username"])
                           ->SetPassword(_config["password"])
                           ->SetDatabase(_config["database"])
                           ->Connect()
                           ->PrepareStatement(npa.GetStatement())
                           ->SetInt(npa["id"], id)
                           ->ExecuteQuery()
                           ->GetResultSet();

            while (res->next())
            {
                return Crop::FromSqlResultSet(*res);
            }
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "In cropRepository::Get<Crop>(" << id << ")\n";
            std::cerr << e.what() << '\n';
            std::cerr << " (MySQL error code: " << e.getErrorCode() << '\n';
            std::cerr << ", SQLState: " << e.getSQLState() << " )" << '\n';
        }
        throw std::invalid_argument("not found");
    }
    // POST
    void CropRepository::Post(Crop crop) const
    {
        try
        {
            static constexpr auto npa = NamedParameterAdapter(R"(
            INSERT INTO crop (id, name, variety, batch)
            VALUES (:id, :name, :variety, :batch)
)");

            auto context = DbContext{};

            auto res = context
                           .SetServer(_config["server"])
                           ->SetUsername(_config["username"])
                           ->SetPassword(_config["password"])
                           ->SetDatabase(_config["database"])
                           ->Connect()
                           ->PrepareStatement(npa.GetStatement())
                           ->SetInt(npa["id"], crop.id)
                           ->SetString(npa["name"], crop.name)
                           ->SetString(npa["variety"], crop.variety)
                           ->SetInt(npa["batch"], crop.batch)
                           ->ExecuteQuery();
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "In cropRepository::Post<Crop>(" << 
                crop.id << ", " <<
                crop.name << ", " <<
                crop.variety << ", " <<
                crop.batch << ")\n";
            std::cerr << e.what() << '\n';
            std::cerr << " (MySQL error code: " << e.getErrorCode() << '\n';
            std::cerr << ", SQLState: " << e.getSQLState() << " )" << '\n';
        }
    }
    // PUT
    void CropRepository::Put(Crop crop) const
    {
        try
        {
            static constexpr auto npa = NamedParameterAdapter(R"(
            UPDATE crop SET
                name = :name, 
                variety = :variety, 
                batch = :batch
            WHERE
                id = :id
)");

            auto context = DbContext{};

            auto res = context
                           .SetServer(_config["server"])
                           ->SetUsername(_config["username"])
                           ->SetPassword(_config["password"])
                           ->SetDatabase(_config["database"])
                           ->Connect()
                           ->PrepareStatement(npa.GetStatement())
                           ->SetInt(npa["id"], crop.id)
                           ->SetString(npa["name"], crop.name)
                           ->SetString(npa["variety"], crop.variety)
                           ->SetInt(npa["batch"], crop.batch)
                           ->ExecuteQuery();
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "In cropRepository::Put<Crop>(" << 
                crop.id << ", " <<
                crop.name << ", " <<
                crop.variety << ", " <<
                crop.batch << ")\n";
            std::cerr << e.what() << '\n';
            std::cerr << " (MySQL error code: " << e.getErrorCode() << '\n';
            std::cerr << ", SQLState: " << e.getSQLState() << " )" << '\n';
        }
    }
    // DELETE
    void CropRepository::Remove(int id) const
    {
        try
        {
            static constexpr auto npa = NamedParameterAdapter(R"(
            DELETE FROM crop 
            WHERE
                id = :id
)");

            auto context = DbContext{};

            auto res = context
                           .SetServer(_config["server"])
                           ->SetUsername(_config["username"])
                           ->SetPassword(_config["password"])
                           ->SetDatabase(_config["database"])
                           ->Connect()
                           ->PrepareStatement(npa.GetStatement())
                           ->SetInt(npa["id"], id)
                           ->ExecuteQuery();
        }
        catch (const sql::SQLException &e)
        {
            std::cerr << "In cropRepository::Remove<Crop>(" << 
                id << ")\n";
            std::cerr << e.what() << '\n';
            std::cerr << " (MySQL error code: " << e.getErrorCode() << '\n';
            std::cerr << ", SQLState: " << e.getSQLState() << " )" << '\n';
        }
    }
}