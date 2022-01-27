#pragma once

#include <vector>

namespace CropProductionManager::Api
{
    template<typename T, typename TCore>
    class IApi
    {
    protected:

    public:
        virtual ~IApi() = default;
	    IApi() = default;
	    IApi(const IApi &) = default;
	    IApi& operator=(const IApi&) = default;
	    IApi(IApi &&) = default;
	    IApi& operator=(IApi &&) = default;

        virtual std::vector<T> Get() const = 0;
        virtual T Get(const int id) const = 0;
        virtual T Post(const T&) = 0;
        virtual void Put(const T&) = 0;
        virtual void Remove(int id) = 0;
    };
}