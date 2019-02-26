#pragma once

#include <map>
#include <vector>
#include <memory>

#include "GLCore.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EntitySystem.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "Shader.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

class RenderSystem : public EntitySystem {
private:
	std::map<EntityID, std::shared_ptr<Component>> *m_RenderComponents;
	std::map<EntityID, std::shared_ptr<Component>> *m_TransformComponents;
	std::vector<Texture2D*> *m_TextureIDs;

	Shader *m_Shader;
	unsigned int m_VBO;
	unsigned int m_VAO;

public:
	RenderSystem(float p_WindowWidth, float p_WindowHeight) {
		// Get access to the components the system requires.
		m_RenderComponents = GetComponentArray(typeid(RenderComponent));
		m_TransformComponents = GetComponentArray(typeid(TransformComponent));

		m_TextureIDs = &ResourceManagerInstance.m_TextureIDs;

		// Configure the shader.
		m_Shader = ResourceManagerInstance.LoadShader("resources/shaders/CardShader.vert", "resources/shaders/CardShader.frag");
		glm::mat4 projection = glm::ortho(0.0f, p_WindowWidth, p_WindowHeight, 0.0f, -1.0f, 1.0f);

		m_Shader->Use();
		m_Shader->SetMat4("projection", projection);

		// Circle vertices.
		GLfloat cardVertices[] = {
			// Position. // Texture Coordinates.
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		gl::GenBuffers(1, &m_VBO);
		gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
		gl::BufferData(gl::ARRAY_BUFFER, sizeof(cardVertices), &cardVertices, gl::STATIC_DRAW);

		gl::GenVertexArrays(1, &m_VAO);
		gl::BindVertexArray(m_VAO);
		gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
		// Vertices - Two floats for position, two for the texture coordinates.
		gl::EnableVertexAttribArray(0);
		gl::VertexAttribPointer(0, 4, gl::FLOAT, gl::FALSE_, 4 * sizeof(gl::FLOAT), (void*)0);

		gl::BindVertexArray(0);
	}
	~RenderSystem() {
		// Clean up buffers.
		gl::DeleteBuffers(1, &m_VBO);
		gl::DeleteVertexArrays(1, &m_VAO);
	}

	virtual void Render(Window &p_Window) override {
		for (const auto &entity : *m_Entities) {
			auto potentialRenderComponent = m_RenderComponents->find(entity);
			auto potentialTransformComponent = m_TransformComponents->find(entity);
			if (potentialRenderComponent != m_RenderComponents->end() && potentialTransformComponent != m_TransformComponents->end()) {
				std::shared_ptr<RenderComponent> renderComponent = std::static_pointer_cast<RenderComponent>(potentialRenderComponent->second);
				std::shared_ptr<TransformComponent> transformComponent = std::static_pointer_cast<TransformComponent>(potentialTransformComponent->second);
				
				m_Shader->Use();
				for (unsigned int i = 0; i < renderComponent->m_SymbolTextureIDs.size(); ++i) {
					glm::mat4 model = glm::mat4(1.0f);
					float radius = transformComponent->m_CircleTransforms[i].m_Radius;

					model = glm::translate(model, glm::vec3(transformComponent->m_CircleTransforms[i].m_Position.X(), transformComponent->m_CircleTransforms[i].m_Position.Y(), 0.0f));
					model = glm::rotate(model, glm::radians(transformComponent->m_CircleTransforms[i].m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
					// Centre the object.
					model = glm::translate(model, glm::vec3(-radius, -radius, 0.0f));

					model = glm::scale(model, glm::vec3(radius * 2, radius * 2, 1.0f));

					m_Shader->SetMat4("model", model);
					m_Shader->SetInt("image", 0);
					gl::ActiveTexture(gl::TEXTURE0);

					if (i == 0)
						ResourceManagerInstance.GetTexture("cardBackground")->Bind();
					else
						m_TextureIDs->at(renderComponent->m_SymbolTextureIDs[i])->Bind();

					gl::BindVertexArray(m_VAO);
					gl::DrawArrays(gl::TRIANGLES, 0, 6);
					gl::BindVertexArray(0);
				}
			}
		}
	}
};