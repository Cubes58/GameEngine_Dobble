#pragma once

#include "Vector2D.h"

class Collision {
public:
	Collision() = default;
	~Collision() = default;

	// Circle/Point.
	bool operator()(const Vector2Df &p_Position, const float &p_Radius, const Vector2Df &p_MouseClickPosition) {
		Vector2Df distance(p_Position.X() - p_MouseClickPosition.X(), p_Position.Y() - p_MouseClickPosition.Y());

		if ((distance.X() * distance.X()) + (distance.Y() * distance.Y()) < p_Radius * p_Radius) 
			return true;

		return false;
	}

	// AABB/Point.
	bool operator()(const Vector2Df &p_Position, const Vector2Df &p_Size, const Vector2Df &p_MouseClickPosition) {
		if (p_Position.X() + (p_Size.X() / 2.0f) >= p_MouseClickPosition.X() && p_MouseClickPosition.X() >= p_Position.X() - (p_Size.X() / 2.0f)
			&& p_Position.Y() + (p_Size.Y() /2.0f) >= p_MouseClickPosition.Y() && p_MouseClickPosition.Y() >= p_Position.Y() - (p_Size.Y() / 2.0f)) {
			return true;
		}

		return false;
	}

	// Circle/Circle
	bool operator()(const Vector2Df &p_EntityOnePosition, float p_EntityOneRadius, const Vector2Df &p_EntityTwoPosition, float p_EntityTwoRadius) {
		Vector2Df distance(p_EntityOnePosition.X() - p_EntityTwoPosition.X(), p_EntityOnePosition.Y() - p_EntityTwoPosition.Y());
		float radii = p_EntityOneRadius + p_EntityTwoRadius;

		if ((distance.X() * distance.X()) + (distance.Y() * distance.Y()) < radii * radii) {
			return true;
		}

		return false;
	}

	// Check whether a circle is within another circle.
	bool IsCircleInCircle(const Vector2Df &p_CircleCentrePosition, float p_CircleRadius, const Vector2Df &p_CircleToCheckIfInsidePosition, float p_CircleToCheckIfInsideRadius) {
		Vector2Df distance(p_CircleCentrePosition - p_CircleToCheckIfInsidePosition);
		float squareDist = distance.DotProduct(distance);
		float radiiSum = p_CircleRadius - p_CircleToCheckIfInsideRadius;

		return squareDist < radiiSum * radiiSum;
	}
};