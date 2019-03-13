#pragma once

#include <vector>

#include "Vector2D.h"

#include <glm/vec4.hpp>

class Shader;
class Texture2D;

struct Particle {
	glm::vec4 m_Colour;
	Vector2Df m_Position;
	Vector2Df m_Velocity;
	float m_Orientation;
	float m_Life;

	Particle() : m_Position(0.0f), m_Velocity(0.0f), m_Colour(1.0f), m_Life(0.0f), m_Orientation(0.0f) {
	}
};

class ParticleManager {
private:
	std::vector<Particle> m_Particles;
	unsigned int m_NumberOfParticles;
	unsigned int m_NumberOfParticlesToSpawnEachTime = 2;
	float m_ParticleLifeDuration = 1.0f;
	unsigned int m_VAO;
	Shader *m_Shader;
	Texture2D *m_ParticleTexture;

	unsigned int FirstUnusedParticle();
	void RespawnParticle(Particle &p_Particle, Vector2Df p_Position = Vector2Df(0.0f, 0.0f), Vector2Df p_Velocity = Vector2Df(0.0f, 0.0f));

public:
	ParticleManager(const Vector2Df &p_ScreenSize, unsigned int p_NumberOfParticles = 250);
	ParticleManager(const Vector2Df &p_ScreenSize, Shader *p_Shader, Texture2D *p_Texture = nullptr, unsigned int p_NumberOfParticles = 250);

	void Update(float p_DeltaTime, Vector2Df p_MousePosition);
	void Render();

	unsigned int GetNumberOfParticlesToSpawnEachTime() const;
	void SetNumberOfParticlesToSpawnEachTime(unsigned int p_NumberToSpawn);
};