#pragma once
#include <vector>

namespace CropProductionManager::Infrastructure
{
    template<typename T>
    class IRepository
    {
    public:
        virtual ~IRepository() = default;
	    IRepository() = default;
	    IRepository(const IRepository &) = default;
	    IRepository& operator=(const IRepository&) = default;
	    IRepository(IRepository &&) = default;
	    IRepository& operator=(IRepository &&) = default;

        virtual std::vector<T> Get() const = 0;
        virtual T Get(const int id) const = 0;
        // POST
        virtual void Post(T) = 0;
        // PUT
        virtual void Put(T) = 0;
        // DELETE
        virtual void Remove(int id) = 0;
    };
}