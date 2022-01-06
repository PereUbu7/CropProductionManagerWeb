export module CropProductionManager.Serializer.Serializer;

import CropProductionManager.Serializer.Crop;
import "json.hpp";

import <string>;
import <iostream>;

export namespace CropProductionManager::Serializer
{
    using json = nlohmann::json;

    template<typename T>
    class Serializer
    {
    private:
        json j;
    public:
        Serializer& Serialize(const T& obj);
        T Deserialize(const json& json);
        json GetJson() const;
    };

    template<typename T>
    Serializer<T>& Serializer<T>::Serialize(const T& obj)
    {
        j = obj;

        return static_cast<Serializer<T>&>(*this); 
    }

    template<typename T>
    T Serializer<T>::Deserialize(const json& json)
    {
        j = json;

        return j.get<T>();
    }

    template<typename T>
    json Serializer<T>::GetJson() const
    {
        return j;
    }

    template<typename T>
    std::ostream& operator <<(std::ostream& os, const Serializer<T> s)
    {
        os << s.GetJson();
        return os;
    }
}