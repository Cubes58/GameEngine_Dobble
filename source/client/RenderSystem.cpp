#include "RenderSystem.h"

#include <glm/gtc/matrix_transform.hpp>

#include "EntityManager.h"
#include "ResourceManager.h"

#include "Shader.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "CircleShape.h"

RenderSystem::RenderSystem(float p_WindowWidth, float p_WindowHeight) {
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

void RenderSystem::Render() {
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