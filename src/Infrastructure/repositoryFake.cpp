#include "CropProductionManager/Infrastructure/repositoryFake.h"

#include <vector>
#include <algorithm>

namespace CropProductionManager::Infrastructure
{
    template<typename T>
    RepositoryFake<T>::RepositoryFake() {}

    // GET
    template<typename T>
    std::vector<T> RepositoryFake<T>::Get() const
    {
        return entities;
    }
    // POST
    template<typename T>
    void RepositoryFake<T>::Post(T entity) 
    {
        entities.push_back(entity);
    }
    // PUT
    template<typename T>
    void RepositoryFake<T>::Put(T entity) 
    {
        auto entityToModify
        {
            std::find_if(begin(entities), end(entities), [entity](T t){ return t.id == entity.id; })
        };

        if(entityToModify != end(entities))
        {
            (*entityToModify).Update(entity);
        }
    }
    // DELETE
    template<typename T>
    void RepositoryFake<T>::Remove(int id) 
    { 
        auto it = std::find_if(begin(entities), end(entities), [id](T entity){ return entity.id == id; });

        if(it != end(entities))
        {
            entities.erase(it);
        }
    }
}