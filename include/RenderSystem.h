#pragma once

#include <memory>

#include "GLCore.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EntitySystem.h"
#include "EntityManager.h"
#include "ResourceManager.h"

class RenderSystem : public EntitySystem {
private:
	unsigned int m_VBO;
	unsigned int m_VAO;

public:
	RenderSystem(float p_WindowWidth, float p_WindowHeight) {
		// Configure the shader.
		glm::mat4 projection = glm::ortho(0.0f, p_WindowWidth, p_WindowHeight, 0.0f, -1.0f, 1.0f);
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

	virtual void Render(Window &p_Window) {
		// Get the weak pointers. (Needs to be improved, I think - Get directly from the array of elements.)
		auto potentialRenderComponent = EntityManagerInstance.GetComponent<RenderComponent>("EntityOne");
		auto potentialPositionComponent = EntityManagerInstance.GetComponent<PositionComponent>("EntityOne");

		// If they're valid shared pointers do the following:
		if (potentialRenderComponent != nullptr && potentialPositionComponent != nullptr) {
			ResourceManager::GetShader("CircleShader").Use();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(potentialPositionComponent->m_XPosition, potentialPositionComponent->m_YPosition, 0.0f));

			model = glm::translate(model, glm::vec3(0.5f * potentialPositionComponent->m_Width, 0.5f * potentialPositionComponent->m_Height, 0.0f));
			model = glm::rotate(model, potentialPositionComponent->m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-0.5f * potentialPositionComponent->m_Width, -0.5f * potentialPositionComponent->m_Height, 0.0f));

			model = glm::scale(model, glm::vec3(potentialPositionComponent->m_Width, potentialPositionComponent->m_Height, 1.0f));

			ResourceManager::GetShader("CircleShader").SetMat4("model", model);
			ResourceManager::GetShader("CircleShader").SetInt("image", 0);
			gl::ActiveTexture(gl::TEXTURE0);
			ResourceManager::GetTexture(potentialRenderComponent->m_CardBackgroundTextureID).Bind();


			gl::BindVertexArray(m_VAO);
			gl::DrawArrays(gl::TRIANGLES, 0, 6);
			gl::BindVertexArray(0);
		}
	}
};