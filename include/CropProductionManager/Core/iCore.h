#pragma once
#include "CropProductionManager/Infrastructure/iRepository.h"

#include <vector>

namespace CropProductionManager::Core
{
    template<typename T, typename TInfrastructure>
    class ICore
    {
    protected:

    public:
        virtual ~ICore() = default;
	    ICore() = default;
	    ICore(const ICore &) = default;
	    ICore& operator=(const ICore&) = default;
	    ICore(ICore &&) = default;
	    ICore& operator=(ICore &&) = default;

        virtual std::vector<T> Get() const = 0;
        virtual T Get(const int id) const = 0;
        virtual T Post(T&) = 0;
        virtual void Put(T&) = 0;
        virtual void Remove(int id) = 0;

        virtual TInfrastructure ToInfrastructure(T& c) const = 0;
    };
}