#pragma once

#include "Vector2D.h"

class Collision {
public:
	Collision() = default;
	~Collision() = default;

	bool operator()(const Vector2D<float> &p_Position, const float &p_Radius, const Vector2D<float> &p_MouseClickPosition) {
		Vector2D<float> distance(p_Position.X() - p_MouseClickPosition.X(), p_Position.Y() - p_MouseClickPosition.Y());

		if ((distance.X() * distance.X()) + (distance.Y() * distance.Y()) < p_Radius * p_Radius) 
			return true;

		return false;
	}

	bool operator()(const Vector2D<float> &p_Position, const Vector2D<float> &p_Size, const Vector2D<float> &p_MouseClickPosition) {
		if (p_Position.X() + (p_Size.X() / 2.0f) >= p_MouseClickPosition.X() && p_MouseClickPosition.X() >= p_Position.X() - (p_Size.X() / 2.0f)
			&& p_Position.Y() + (p_Size.Y() /2.0f) >= p_MouseClickPosition.Y() && p_MouseClickPosition.Y() >= p_Position.Y() - (p_Size.Y() / 2.0f)) {
			return true;
		}

		return false;
	}
};