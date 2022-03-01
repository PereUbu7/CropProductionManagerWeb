#pragma once

#include "CropProductionManager/Infrastructure/compileTimeString.hpp"
#include <cstddef>
#include <array>

#include <algorithm>

namespace CropProductionManager::Infrastructure
{
    template <std::size_t N, std::size_t maxParameters = 20, std::size_t maxParamLength = 20>
    class NamedParameterAdapter
    {
    private:
        compiletime::string<N> _statement;
        std::array<compiletime::string<maxParamLength>, maxParameters> parameters;
        int numberOfRecorderdParameters;

    public:
        constexpr NamedParameterAdapter(const char (&statement)[N]) : 
            _statement{truncateWhitespace(statement)},
            parameters{},
            numberOfRecorderdParameters{}
        {
            extractParameters();
        }

        constexpr auto GetCompileTimeStatement() const { return _statement; }

        auto GetStatement() const
        {
            return std::string{_statement};
        }

        constexpr bool isAlphaNumerical(char c)
        {
            return (c >= 'a' && c <= 'z' ||
                    c >= 'A' && c <= 'Z' ||
                    c >= '0' && c <= '9');
        }

        constexpr bool is_whitespace(char c)
        {
            return (c == ' ') ||
                   (c == '\t') ||
                   (c == '\n') ||
                   (c == '\v') ||
                   (c == '\f') ||
                   (c == '\r');
        }

        constexpr auto truncateWhitespace(const char (&str)[N])
        {
            compiletime::string<N> tmp(str); // build instance
            return truncateWhitespace(tmp);  // run function
        }

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

        constexpr auto extractParameters()
        {
            int stringIndex{0};
            int parameterNumber{0};
            char indexedStatement[N] = {0};
            int indexedStatementIndex{0};
            while (_statement[stringIndex] != '\0')
            {
                /* Copy statement */
                indexedStatement[indexedStatementIndex] = _statement[stringIndex];

                /* Until we find a named parameter */
                if (_statement[stringIndex] == ':')
                {
                    /* Measure length of named parameter */
                    int parameterIndex{0};
                    while (isAlphaNumerical(_statement[stringIndex + parameterIndex + 1]))
                    {
                        ++parameterIndex;
                    }

                    /* Save it for later when we bind */
                    parameters[parameterNumber] = compiletime::string<maxParamLength>(&_statement[stringIndex + 1], parameterIndex);

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

        constexpr int operator[](const char *parameter) const
        {
            size_t parameterLength;
            for (int i{0}; i < numberOfRecorderdParameters; ++i)
            {
                if (parameters[i].compare(parameter))
                    return ++i;
            }
            throw;
        }
    };
}