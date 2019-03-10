#include "RectangleShape.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

RectangleShape::RectangleShape(const Vector2Df &p_Position, const Vector2Df &p_Size, Texture2D *p_Texture)
	: Shape(p_Position, p_Texture), m_Size(p_Size) {
	GenerateVertexData();
}

void RectangleShape::GenerateVertexData() {
	GLfloat squareVertices[] = {
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	gl::GenBuffers(1, &m_VBO);
	gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(squareVertices), &squareVertices[0], gl::STATIC_DRAW);

	gl::GenVertexArrays(1, &m_VAO);
	gl::BindVertexArray(m_VAO);
	gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
	// Vertices - Two floats for position, two for the texture coordinates.
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 2, gl::FLOAT, gl::FALSE_, 0, (void*)0);

	gl::BindVertexArray(0);
}

void RectangleShape::Render(Shader &p_Shader) {
	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(m_Position.X(), m_Position.Y(), 0.0f));
	model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	// Centre the object.
	model = glm::translate(model, glm::vec3(-m_Size.X() / 2, -m_Size.Y() / 2, 0.0f));

	model = glm::scale(model, glm::vec3(m_Size.X(), m_Size.Y(), 1.0f));
	p_Shader.SetMat4("model", model);

	p_Shader.SetVec4("colour", m_Colour);
	if (m_Texture != nullptr) {
		p_Shader.SetInt("image", 0);
		gl::ActiveTexture(gl::TEXTURE0);
		m_Texture->Bind();
	}
	p_Shader.SetFloat("mixValue", m_ColourTextureMixValue);
	p_Shader.SetBool("usingColourAsTextureMask", m_UsingColourAsTextureMask);

	gl::BindVertexArray(m_VAO);
	gl::DrawArrays(gl::TRIANGLES, 0, 6);
	gl::BindVertexArray(0);
}