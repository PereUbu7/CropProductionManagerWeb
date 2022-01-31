#include "CropProductionManager/Infrastructure/cropRepository.h"


namespace CropProductionManager::Infrastructure
{
    using Crop = CropProductionManager::InternalModel::Infrastructure::Crop;

    CropRepository::CropRepository(IConfiguration config)
    {
        try
        {
            sql::Driver *driver;
            sql::Connection *con;
            sql::Statement *stmt;
            // sql::ResultSet *res;

            /* Create a connection */
            driver = get_driver_instance();
            con = driver->connect(
                config["server"], 
                config["username"], 
                config["password"]);
            /* Connect to the MySQL test database */
            con->setSchema("test");
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
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