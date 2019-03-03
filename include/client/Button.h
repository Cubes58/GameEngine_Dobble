#pragma once

#include <typeindex>
#include <memory>

#include "Vector2D.h"
#include "GameState.h"

class Shape;

struct Button {
	std::shared_ptr<Shape> m_Shape;
	std::type_index m_ShapeType;
	GameState m_GameState;

	Button() = default;
	Button(std::shared_ptr<Shape> p_Shape, const std::type_index &p_ShapeType) 
		: m_Shape(p_Shape), m_ShapeType(p_ShapeType) { }
	Button(std::shared_ptr<Shape> p_Shape, const std::type_index &p_ShapeType, const GameState &p_GameState)
		: m_Shape(p_Shape), m_ShapeType(p_ShapeType), m_GameState(p_GameState) { }
	~Button() = default;
};