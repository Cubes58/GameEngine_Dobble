#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

using EntityID = std::size_t;

class Entity {
private:
	EntityID m_ID;
	bool m_Alive;

public:
	Entity(EntityID p_ID) : m_ID(p_ID), m_Alive(true) {}
	~Entity() = default;

	void SetAlive(bool p_Alive) {
		m_Alive = p_Alive;
	}
	bool GetAlive() const {
		return m_Alive;
	}

	bool operator == (Entity const &p_OtherEntity) {
		return this->m_ID == p_OtherEntity.m_ID;
	}

	bool operator < (Entity const &p_OtherEntity) {
		return this->m_ID < p_OtherEntity.m_ID;
	}
};