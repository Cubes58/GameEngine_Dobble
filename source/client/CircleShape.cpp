#include "CircleShape.h"

#include <glm/gtc/matrix_transform.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"

CircleShape::CircleShape(const Vector2Df &p_Position, unsigned int p_NumberOfFaces, float p_Radius, Texture2D *p_Texture) 
	: Shape(p_Position, p_Texture), m_NumberOfFaces(p_NumberOfFaces), m_Radius(p_Radius) {
	GenerateFaces(m_NumberOfFaces);
}

void CircleShape::Render(Shader &p_Shader) {
	glm::mat4 model = glm::mat4(1.0f);
	m_Radius *= 2.0f;
	model = glm::translate(model, glm::vec3(m_Position.X(), m_Position.Y(), 0.0f));
	model = glm::rotate(model, glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));	
	// Centre the object.
	model = glm::translate(model, glm::vec3(-m_Radius / 2.0f, -m_Radius / 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(m_Radius, m_Radius, 1.0f));
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
	gl::DrawArrays(gl::TRIANGLES, 0, m_NumberOfVerticesToDraw);
	gl::BindVertexArray(0);
}

void CircleShape::GenerateFaces(unsigned int p_NumberOfFaces) {
	const constexpr unsigned int numberOfAxis = 2; // X and Y
	const constexpr unsigned int numberOfVerticesPerTriangle = 3; // Three vertices to create a triangle shape.

	m_NumberOfFaces = p_NumberOfFaces;
	const int numberOfVertices = m_NumberOfFaces + numberOfAxis;
	m_NumberOfVerticesToDraw = ((numberOfVertices) * numberOfAxis) * numberOfVerticesPerTriangle;

	float twicePi = 2.0f * (float)M_PI;
	float radius = 0.5f;	// Scale using model matrix, later.

	GLfloat *circleVerticesX = new GLfloat[numberOfVertices];
	GLfloat *circleVerticesY = new GLfloat[numberOfVertices];

	// Set the first vertex to the centre!
	circleVerticesX[0] = 0.5f;
	circleVerticesY[0] = 0.5f;

	// Construct all of the vertex point positions, for the cirle. (Add two float, one for x, the other for y.)
	for (int i = 1; i < numberOfVertices; i++) {
		circleVerticesX[i] = 0.5f + (radius * cos(i *  twicePi / m_NumberOfFaces));
		circleVerticesY[i] = 1.0f -  (0.5f + (radius * sin(i * twicePi / m_NumberOfFaces)));
	}

	// Pack all of the vertex points into an array (6 floats - 3 vertex positions, 2 floats per point - x and y).
	GLfloat *packVerticesAsTriangles = new GLfloat[((numberOfVertices) * numberOfAxis) * numberOfVerticesPerTriangle];
	for (int i = 2; i < numberOfVertices; i++) {
		// Add the vertex at 0, 0
		packVerticesAsTriangles[(i * 6) + 0] = circleVerticesX[0];
		packVerticesAsTriangles[(i * 6) + 1] = circleVerticesY[0];


		// Add the previous vertex.
		packVerticesAsTriangles[(i * 6) + 2] = circleVerticesX[i - 1];
		packVerticesAsTriangles[(i * 6) + 3] = circleVerticesY[i - 1];

		// Add the new vertex.
		packVerticesAsTriangles[(i * 6) + 4] = circleVerticesX[i];
		packVerticesAsTriangles[(i * 6) + 5] = circleVerticesY[i];
	}

	// Generate VBO.
	gl::GenBuffers(1, &m_VBO);
	gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
	gl::BufferData(gl::ARRAY_BUFFER, ((numberOfVertices) * 2) * 3 * sizeof(float), &packVerticesAsTriangles[0], gl::STATIC_DRAW);

	gl::GenVertexArrays(1, &m_VAO);
	gl::BindVertexArray(m_VAO);
	gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);

	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 2, gl::FLOAT, gl::FALSE_, 0, NULL);

	gl::BindVertexArray(0);

	// Clean up the allocated memory.
	delete circleVerticesX;
	delete circleVerticesY;
	delete packVerticesAsTriangles;
}