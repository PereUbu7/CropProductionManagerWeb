#pragma once

#include "CropProductionManager/Infrastructure/compileTimeString.hpp"
#include <cstddef>
#include <array>

#include <algorithm>

namespace CropProductionManager::Infrastructure
{
    constexpr bool is_whitespace(char c)
    {
        return (c == ' ') ||
               (c == '\t') ||
               (c == '\n') ||
               (c == '\v') ||
               (c == '\f') ||
               (c == '\r');
    }

    template <std::size_t N>
    constexpr auto truncateWhitespace(compiletime::string<N> str)
    {
        // Need to use non-type template for string max size
        compiletime::string<N> result;
        bool previousIsWhitespace = false; // Keep track if the previous character was whitespace
        for (char c : str)
        {
            // Skip new lines
            if (c == '\n')
            {
                continue;
            }
            else if (is_whitespace(c))
            {
                // If the last character was whitespace, continue interation
                if (previousIsWhitespace)
                {
                    continue;
                }
                // Whitespace: Set flag
                previousIsWhitespace = true;
            }
            else
            {
                // Not whitespace: Reset flag
                previousIsWhitespace = false;
            }

            result.push_back(c); // Otherwise; add character to new string
        }
        return result;
    }

    template <std::size_t N>
    constexpr auto truncateWhitespace(const char (&str)[N])
    {
        compiletime::string<N> tmp(str); // build instance
        return truncateWhitespace(tmp);  // run function
    }

    template<typename T>
    struct Binding
    {
        T value;
        int index;
        bool set{false};
    };

    template<typename T, std::size_t maxParameters>
    class BindingCollection
    {
    private:
        std::array<Binding<T>, maxParameters> bindings;
        std::size_t m_size;
    public:
        constexpr BindingCollection() :
            bindings{},
            m_size{0}
        {}

        constexpr std::size_t size() const { return m_size; }

        constexpr auto& Bindings() const
        {
            return bindings;
        }

        constexpr BindingCollection& Bind(const T& value, int index)
        {
            bindings[m_size].value = value;
            bindings[m_size].index = index;
            bindings[m_size].set = true;
            ++m_size;
            return *this;
        }
    };

    template<std::size_t N, std::size_t maxParameters = 20, std::size_t maxParamLength = 20>
    class NamedParameterAdapter
    {
    private:
        compiletime::string<N> _statement;
        std::array<compiletime::string<maxParamLength>, maxParameters> parameters; 
        BindingCollection<int, maxParameters> boundInts;
        std::size_t len;
        int numberOfRecorderdParameters;
    public:
        constexpr NamedParameterAdapter(const char (&statement)[N]) :
            _statement{truncateWhitespace(statement)},
            parameters{},
            boundInts{},
            len{N},
            numberOfRecorderdParameters{}
        {
            extractParameters();
        }

        template<typename T>
        constexpr auto& GetBindings() const
        {
            if(std::is_same_v<T, int>) return boundInts.Bindings();
        }

        template<typename T>
        auto GetValue(int index) const -> T
        {
            std::array<Binding<T>, maxParameters> collection;
            if(std::is_same_v<T, int>) collection = boundInts.Bindings();

            auto item = std::find_if(
                collection.begin(), 
                collection.end(), 
                [&index](auto b){ return b.set && b.index == index; });
            if(item == collection.end()) throw;
            return item->value;
        }

        constexpr auto GetCompileTimeStatement() const { return _statement; }

        auto GetStatement() const
        {
            return std::string{_statement};
        }

        constexpr NamedParameterAdapter& BindInt(const compiletime::string<maxParamLength>& name, const int value)
        {
            for(int i{0}; i<maxParameters; ++i)
            {
                if(parameters[i] == name)
                {
                    boundInts.Bind(value, i);
                    return *this;
                }
            }
            return *this;
        }

        constexpr bool isAlphaNumerical(char c)
        {
            return (c >= 'a' && c <= 'z' ||
                c >= 'A' && c <= 'Z' ||
                c >= '0' && c <= '9');
        }

        constexpr auto extractParameters()
        {
            int stringIndex{0};
            int parameterNumber{0};
            char indexedStatement[N] = {0};
            int indexedStatementIndex{0};
            while(_statement[stringIndex] != '\0')
            {
                /* Copy statement */
                indexedStatement[indexedStatementIndex] = _statement[stringIndex];

                /* Until we find a named parameter */
                if(_statement[stringIndex] == ':')
                {
                    /* Measure length of named parameter */
                    int parameterIndex{0};
                    while(isAlphaNumerical(_statement[stringIndex+parameterIndex+1]))
                    {
                        ++parameterIndex;
                    }

                    /* Save it for later when we bind */
                    parameters[parameterNumber] = compiletime::string<maxParamLength>(&_statement[stringIndex+1], parameterIndex);

                    /* Replace named parameter with '?' */
                    indexedStatement[indexedStatementIndex] = '?';
                        
                    /* Prepare to get next parameter */
                    ++parameterNumber;

                    /* Skip parameter and continue search */
                    stringIndex += parameterIndex;
                }
                ++stringIndex;
                ++indexedStatementIndex;
            }

            numberOfRecorderdParameters = parameterNumber;

            /* Overwrite statement */
            _statement = compiletime::string<N>(indexedStatement);
        }

        // read:
        // https://mklimenko.github.io/english/2018/08/31/constexpr-array-size-calculation/

        // also check out:
        // https://davidgorski.ca/posts/truncate-string-whitespace-compiletime-cpp/
        // template <const NamedParameterAdapter& npa, std::size_t N>
    };
}