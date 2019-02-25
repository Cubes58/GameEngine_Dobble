#pragma once

#include "Vector2D.h"

class Collision {
public:
	Collision() = default;
	~Collision() = default;

	bool operator()(const Vector2D<float> &p_SymbolPosition, const float &p_Radius, const Vector2D<float> &p_MouseClickPosition) {
		Vector2D<float> distance(p_SymbolPosition.X() - p_MouseClickPosition.X(), p_SymbolPosition.Y() - p_MouseClickPosition.Y());

		if ((distance.X() * distance.X()) + (distance.Y() * distance.Y()) < p_Radius * p_Radius) 
			return true;

		return false;
	}
};