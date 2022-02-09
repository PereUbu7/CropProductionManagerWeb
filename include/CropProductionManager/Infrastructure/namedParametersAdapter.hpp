#include <string>
#include <array>

namespace CropProductionManager::Infrastructure
{
    template<size_t S>
    class NamedParametersAdapter
    {
    public:
        template<typename T>
        class NameValue
        {
        public:
            constexpr std::string GetName() { return name; }
            constexpr T GetValue() { return value; }
            constexpr void SetName(const std::string& newName) { name = newName; }
            constexpr void SetValue(const T& newValue) { value = newValue; }
            std::string name;
            T value;
        };
        NamedParametersAdapter() : 
            Statement{},
            intParams{},
            stringParams{}
        {}

        constexpr void BindInt(const std::string &name, int32_t value) { intParams.push_back(NameValue<int32_t>{name, value}); }
        void BindString(const std::string &name, const std::string &value) { stringParams.push_back(NameValue<std::string>{name, value}); }
        constexpr void SetStatement(const std::string& statement) { Statement = statement; }
        std::string Process();
        std::string Statement;
    private:
        std::array<NameValue<int32_t>, S> intParams;
        std::array<NameValue<std::string>, S> stringParams;
    };
}