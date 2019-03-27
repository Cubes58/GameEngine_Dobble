/**
@file RenderSystem.h
@brief An entity system, which renders the entity data.
*/
#pragma once

#include <map>
#include <vector>
#include <memory>

#include "EntitySystem.h"
#include "Vector2D.h"

struct Component;
class Texture2D;
class Shader;

/*! \class RenderSystem
	\brief An entity system, which renders the entity data.
*/

class RenderSystem : public EntitySystem {
private:
	std::map<EntityID, std::shared_ptr<Component>> *m_RenderComponents;		//!< A pointer to the render components.
	std::map<EntityID, std::shared_ptr<Component>> *m_TransformComponents;	//!< A pointer to the transform components.
	std::vector<Texture2D*> *m_TextureIDs;	//!< A pointer to the texture ID's.
	Shader *m_Shader;	//!< A pointer to the shader used, to render.

public:
	/*!
		\brief Constructor.
		\param p_WindowWidth the width of the window.
		\param p_WindowHeight the height of the window.
	*/
	RenderSystem(float p_WindowWidth, float p_WindowHeight);
	~RenderSystem() = default;	//!< Destructor.

	/*!
		\brief Renders the entity data.
		\return Nothing.
	*/
	virtual void Render() override;
};