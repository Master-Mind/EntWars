#pragma once
//adding this lib because sfml doesn't have any math functions ._.
#include <SFML/System/Vector2.hpp>

inline float VecLenSquared(const sf::Vector2f& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

inline float VecLen(const sf::Vector2f& vec)
{
	return std::sqrt(VecLenSquared(vec));
}

inline sf::Vector2f VecNorm(const sf::Vector2f& vec)
{
	float len = VecLen(vec);
	if (len == 0)
	{
		return vec;
	}
	return vec / len;
}

inline float VecDot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}