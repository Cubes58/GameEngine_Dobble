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
		glm::vec2 entityOneXAxisLine(p_Position.X() - p_Size.X(), p_Position.X() + p_Size.X());
		glm::vec2 entityOneYAxisLine(p_Position.Y() - p_Size.Y(), p_Position.Y() + p_Size.Y());

		glm::vec2 entityTwoXAxisLine(p_MouseClickPosition.X(), p_MouseClickPosition.X());
		glm::vec2 entityTwoYAxisLine(p_MouseClickPosition.Y(), p_MouseClickPosition.Y());

		if (entityOneXAxisLine.x >= entityTwoXAxisLine.x && entityOneXAxisLine.y <= entityTwoXAxisLine.y
			&& entityOneYAxisLine.x >= entityTwoYAxisLine.x && entityOneYAxisLine.y <= entityTwoYAxisLine.y)
			return true;

		return false;
	}
};