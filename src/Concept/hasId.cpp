export module CropProductionManager.Concept.HasId;

import <concepts>;

export template<typename T>
concept HasId = requires(T t)
{
    { t.id } -> std::same_as<int&>;
};