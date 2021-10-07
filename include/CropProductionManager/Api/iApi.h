#pragma once

#include <vector>

namespace CropProductionManager::Api
{
    template<typename T, typename TCore>
    class IApi
    {
    protected:

    public:
        virtual std::vector<T> Get() const = 0;
        // POST
        virtual T Post(const T&) = 0;
        // PUT
        virtual void Put(const T&) = 0;
        // DELETE
        virtual void Remove(int id) = 0;
    };
}