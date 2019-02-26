#pragma once

#include <typeindex>
#include <map>
#include <set>
#include <memory>

using EntityID = std::size_t;
struct Component;
class Window;

class EntitySystem {
protected:
	std::set<EntityID> *m_Entities;

	std::map<EntityID, std::shared_ptr<Component>> *GetComponentArray(std::type_index p_TypeIndex);

public:
	EntitySystem();
	virtual ~EntitySystem() = default;

	virtual void Update(float p_DeltaTime) {}
	virtual void Render(Window &p_Window) {}
};