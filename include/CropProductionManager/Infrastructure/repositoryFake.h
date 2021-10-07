#pragma once

#include "CropProductionManager/Infrastructure/iRepository.h"
#include "CropProductionManager/InternalModel/entity.h"
#include <vector>

namespace CropProductionManager::Infrastructure
{

    template<typename T>
    class RepositoryFake : public CropProductionManager::Infrastructure::IRepository<T>
    {
        using Entity = CropProductionManager::InternalModel::Entity;
        
        static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity");

    private:
        std::vector<T> entities;
    public:
        // GET
        std::vector<T> Get() const;
        // POST
        void Post(T entity);
        // PUT
        void Put(T entity);
        // DELETE
        void Remove(int id);

        RepositoryFake();
    };
}