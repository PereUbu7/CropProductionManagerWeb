export module CropProductionManager.Infrastructure.RepositoryFake;

import CropProductionManager.Concept.HasId;
import CropProductionManager.Infrastructure.IRepository;
import <vector>;

export namespace CropProductionManager::Infrastructure
{
    template<HasId T>
    class RepositoryFake : public CropProductionManager::Infrastructure::IRepository<T>
    {
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
    };

    // GET
    template<HasId T>
    std::vector<T> RepositoryFake<T>::Get() const
    {
        return entities;
    }
    // POST
    template<HasId T>
    void RepositoryFake<T>::Post(T entity) 
    {
        entities.push_back(entity);
    }
    // PUT
    template<HasId T>
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
    template<HasId T>
    void RepositoryFake<T>::Remove(int id) 
    { 
        auto it = std::find_if(begin(entities), end(entities), [id](T entity){ return entity.id == id; });

        if(it != end(entities))
        {
            entities.erase(it);
        }
    }
}