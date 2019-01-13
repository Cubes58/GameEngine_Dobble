#pragma once

#include <memory>

#include "GLCore.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EntitySystem.h"
#include "Window.h"
#include "EntityManager.h"
#include "ResourceManager.h"

class RenderSystem : public EntitySystem {
private:
	Window &m_Window;

	unsigned int m_VBO;
	unsigned int m_VAO;

public:
	RenderSystem(Window &p_Window) : m_Window(p_Window) {
		// Configure the shader.
		glm::mat4 projection = glm::ortho(0.0f, (float)m_Window.GetWidth(), (float)m_Window.GetHeight(), 0.0f, -1.0f, 1.0f);
		ResourceManager::LoadShader("resources/shaders/CircleShader.vert", "resources/shaders/CircleShader.frag", nullptr, "CircleShader");
		ResourceManager::GetShader("CircleShader").Use();
		ResourceManager::GetShader("CircleShader").SetMat4("projection", projection);

		// Generate a massive one, for the card, then 8 smaller ones for the symbols?
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
		// Vertices
		gl::EnableVertexAttribArray(0);
		gl::VertexAttribPointer(0, 4, gl::FLOAT, gl::FALSE_, 4 * sizeof(gl::FLOAT), (void*)0);

		gl::BindVertexArray(0);
	}
	~RenderSystem() = default;

	void Process(const EntityID &p_Entity) override {
		// Get the weak pointers. (Needs to be improved, I think - Get directly from the array of elements.)
		auto potentialRenderComponent = EntityManagerInstance.GetComponent<RenderComponent>(p_Entity);
		auto potentialPositionComponent = EntityManagerInstance.GetComponent<PositionComponent>(p_Entity);
		
		// If the memory is still allocated then turn it into a shared pointer - so they can be used.
		auto renderComponent = potentialRenderComponent.lock(); 
		auto positionComponent = potentialPositionComponent.lock();

		// If they're valid shared pointers do the following:
		if (renderComponent != nullptr && positionComponent != nullptr) {
			ResourceManager::GetShader("CircleShader").Use();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(positionComponent->m_XPosition, positionComponent->m_YPosition, 0.0f));

			model = glm::translate(model, glm::vec3(0.5f * positionComponent->m_Width, 0.5f * positionComponent->m_Height, 0.0f));
			model = glm::rotate(model, positionComponent->m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5f * positionComponent->m_Width, -0.5f * positionComponent->m_Height, 0.0f));

			model = glm::scale(model, glm::vec3(positionComponent->m_Width, positionComponent->m_Height, 1.0f));

			ResourceManager::GetShader("CircleShader").SetMat4("model", model);
			ResourceManager::GetShader("CircleShader").SetInt("image", 0);
			gl::ActiveTexture(gl::TEXTURE0);
			ResourceManager::GetTexture(renderComponent->m_CardBackgroundTextureID).Bind();
			

			gl::BindVertexArray(m_VAO);
			gl::DrawArrays(gl::TRIANGLES, 0, 6);
			gl::BindVertexArray(0);
		}
	}
};