#pragma once
#include <type_traits>
// Returns enum value
template <typename Enumeration>
constexpr std::underlying_type_t<Enumeration> enum2val(Enumeration const value)
{
	return static_cast<typename std::underlying_type_t<Enumeration>>(value);
};