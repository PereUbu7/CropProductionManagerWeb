#pragma once
#include <concepts>

template<typename T>
concept HasId = requires(T t)
{
    { t.id } -> std::same_as<int&>;
};