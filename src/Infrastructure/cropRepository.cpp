#include "CropProductionManager/Infrastructure/cropRepository.h"

namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    CropRepository::CropRepository(IConfiguration config) :
    context{}
    {
        try
        {
            auto res = context
                .SetServer(config["server"])
                ->SetUsername(config["username"])
                ->SetPassword(config["password"])
                ->SetDatabase(config["database"])
                ->Connect() // TODO: Checkup named parameters
                ->PrepareStatement("SELECT id, name, variety, batch FROM crop WHERE id = ?")
                ->SetInt(1, 1)
                ->ExecuteQuery()
                ->GetResultSet();

            while (res->next())
            {
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
    }

    // GET
    std::vector<Crop> CropRepository::Get() const
    {
        return crops;
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