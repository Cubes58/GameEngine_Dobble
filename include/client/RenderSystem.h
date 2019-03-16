/**
@file RenderSystem.h
@brief An entity system, which renders the entity data.
*/
#pragma once

#include <map>
#include <vector>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "EntitySystem.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Vector2D.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "CircleShape.h"

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
	RenderSystem(float p_WindowWidth, float p_WindowHeight) {
		// Get access to the components the system requires.
		m_RenderComponents = GetComponentArray(typeid(RenderComponent));
		m_TransformComponents = GetComponentArray(typeid(TransformComponent));

		m_TextureIDs = &ResourceManagerInstance.m_TextureIDs;

		// Configure the shader.
		m_Shader = ResourceManagerInstance.LoadShader("resources/shaders/ShapesShader.vert", "resources/shaders/ShapesShader.frag");
		glm::mat4 projection = glm::ortho(0.0f, p_WindowWidth, p_WindowHeight, 0.0f, -1.0f, 1.0f);

		m_Shader->Use();
		m_Shader->SetMat4("projection", projection);
	}
	~RenderSystem() = default;	//!< Destructor.

	/*!
		\brief Renders the entity data.
		\return Nothing.
	*/
	virtual void Render() override {
		for (const auto &entity : *m_Entities) {
			auto potentialRenderComponent = m_RenderComponents->find(entity);
			auto potentialTransformComponent = m_TransformComponents->find(entity);
			if (potentialRenderComponent != m_RenderComponents->end() && potentialTransformComponent != m_TransformComponents->end()) {
				std::shared_ptr<RenderComponent> renderComponent = std::static_pointer_cast<RenderComponent>(potentialRenderComponent->second);
				std::shared_ptr<TransformComponent> transformComponent = std::static_pointer_cast<TransformComponent>(potentialTransformComponent->second);
				
				m_Shader->Use();
				for (unsigned int i = 0; i < renderComponent->m_SymbolTextureIDs.size(); ++i) {
					CircleShape symbol(transformComponent->m_CircleTransforms[i].m_Position, 64, transformComponent->m_CircleTransforms[i].m_Radius);
					i == 0 ? symbol.SetTexture(ResourceManagerInstance.GetTexture("cardBackground")) : symbol.SetTexture(m_TextureIDs->at(renderComponent->m_SymbolTextureIDs[i]));
					symbol.SetRotation(transformComponent->m_CircleTransforms[i].m_Rotation);

					symbol.Render(*m_Shader);
				}
			}
		}
	}
};