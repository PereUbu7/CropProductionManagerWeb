export module CropProductionManager.ModelApi.Serialization;

import CropProductionManager.ModelApi.Crop;
import "json.hpp";

export namespace CropProductionManager::ModelApi
{
    void to_json(nlohmann::json& j, const Crop& crop);
    void from_json(const nlohmann::json& j, Crop& crop);
}

namespace CropProductionManager::ModelApi
{
    void to_json(nlohmann::json& j, const Crop& crop)
    {
        j = nlohmann::json
        {
            {"name", crop.name},
            {"variety", crop.variety},
            {"batch", crop.batch}
        };
    }

    void from_json(const nlohmann::json& j, Crop& crop)
    {
        j.at("name").get_to(crop.name);
        j.at("variety").get_to(crop.variety);
        j.at("batch").get_to(crop.batch);
    }
}