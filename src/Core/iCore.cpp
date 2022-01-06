export module CropProductionManager.Core.ICore;

import <vector>;

export namespace CropProductionManager::Core
{
    template<typename T, typename TInfrastructure>
    class ICore
    {
    protected:

    public:
        virtual std::vector<T> Get() const = 0;
        // POST
        virtual T Post(T&) = 0;
        // PUT
        virtual void Put(T&) = 0;
        // DELETE
        virtual void Remove(int id) = 0;

        virtual TInfrastructure ToInfrastructure(T& c) const = 0;

        virtual ~ICore() = default;
    };
}