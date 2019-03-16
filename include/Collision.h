/**
@file Collision.h
@brief Collision helper class.
*/
#pragma once

#include "Vector2D.h"

/*! \class Collision
	\brief Implements collision checking.
*/

class Collision {
public:
	Collision() = default;	//!< Constructor.
	~Collision() = default;	//!< Destructor.

	/*!
		\brief Checks if a point is within a circle.
		\param p_Position The position of the circle.
		\param p_Radius The radius of the circle.
		\param p_PointPosition The position of the point.
		\return Returns true if the point is within the circle, otherwise false.
	*/
	bool operator()(const Vector2Df &p_Position, const float &p_Radius, const Vector2Df &p_PointPosition) {
		Vector2Df distance(p_Position.X() - p_PointPosition.X(), p_Position.Y() - p_PointPosition.Y());

		if ((distance.X() * distance.X()) + (distance.Y() * distance.Y()) < p_Radius * p_Radius) 
			return true;

		return false;
	}

	/*!
		\brief Checks if a point is within an AABB.
		\param p_Position The position of the AABB.
		\param p_Size The size of the AABB.
		\param p_PointPosition The position of the point.
		\return Returns true if the point is within the AABB, otherwise false.
	*/
	bool operator()(const Vector2Df &p_Position, const Vector2Df &p_Size, const Vector2Df &p_PointPosition) {
		if (p_Position.X() + (p_Size.X() / 2.0f) >= p_PointPosition.X() && p_PointPosition.X() >= p_Position.X() - (p_Size.X() / 2.0f)
			&& p_Position.Y() + (p_Size.Y() /2.0f) >= p_PointPosition.Y() && p_PointPosition.Y() >= p_Position.Y() - (p_Size.Y() / 2.0f)) {
			return true;
		}

		return false;
	}

	/*!
		\brief Checks if a circle is within a circle.
		\param p_EntityOnePosition The position of the first circle.
		\param p_EntityOneRadius The radius of the first circle.
		\param p_EntityTwoPosition The position of the second circle.
		\param p_EntityTwoRadius The radius of the second circle.
		\return Returns true if a circle is within the other circle, otherwise false.
	*/
	bool operator()(const Vector2Df &p_EntityOnePosition, float p_EntityOneRadius, const Vector2Df &p_EntityTwoPosition, float p_EntityTwoRadius) {
		Vector2Df distance(p_EntityOnePosition.X() - p_EntityTwoPosition.X(), p_EntityOnePosition.Y() - p_EntityTwoPosition.Y());
		float radii = p_EntityOneRadius + p_EntityTwoRadius;

		if ((distance.X() * distance.X()) + (distance.Y() * distance.Y()) < radii * radii) {
			return true;
		}

		return false;
	}

	/*!
		\brief Checks whether a circle is within another circle.
		\param p_CircleCentrePosition The position of the circle.
		\param p_CircleRadius The radius of the circle.
		\param p_CircleToCheckIfInsidePosition The position of the inside circle.
		\param p_CircleToCheckIfInsideRadius The radius of the inside circle.
		\return Returns true if a circle is within the other circle, otherwise false.
	*/
	bool IsCircleInCircle(const Vector2Df &p_CircleCentrePosition, float p_CircleRadius, const Vector2Df &p_CircleToCheckIfInsidePosition, float p_CircleToCheckIfInsideRadius) {
		Vector2Df distance(p_CircleCentrePosition - p_CircleToCheckIfInsidePosition);
		float squareDist = distance.DotProduct(distance);
		float radiiSum = p_CircleRadius - p_CircleToCheckIfInsideRadius;

		return squareDist < radiiSum * radiiSum;
	}
};