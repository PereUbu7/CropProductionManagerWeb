#pragma once
#include <vector>

namespace CropProductionManager::Infrastructure
{
    template<typename T>
    class IRepository
    {
    public:
        virtual std::vector<T> Get() const = 0;
        // POST
        virtual void Post(T) = 0;
        // PUT
        virtual void Put(T) = 0;
        // DELETE
        virtual void Remove(int id) = 0;

        virtual ~IRepository() {}
    };
}