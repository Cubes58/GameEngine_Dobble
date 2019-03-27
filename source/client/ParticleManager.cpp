#include "ParticleManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "GLCore.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Randomiser.h"
#include "ResourceManager.h"

#include "Shader.h"
#include "Texture2D.h"
#include "RectangleShape.h"

ParticleManager::ParticleManager(const Vector2Df &p_ScreenSize, unsigned int p_NumberOfParticles) : m_NumberOfParticles(p_NumberOfParticles) {
	m_Shader = ResourceManagerInstance.LoadShader("resources/shaders/ShapesShader.vert", "resources/shaders/ShapesShader.frag");
	m_Shader->Use();
	glm::mat4 projection = glm::ortho(0.0f, p_ScreenSize.X(), p_ScreenSize.Y(), 0.0f, -1.0f, 1.0f);
	m_Shader->SetMat4("projection", projection);

	m_ParticleTexture = ResourceManagerInstance.GetTexture("particle");
	for (unsigned int i = 0; i < m_NumberOfParticles; ++i) {
		m_Particles.emplace_back(Particle());
	}
}

ParticleManager::ParticleManager(const Vector2Df &p_ScreenSize, Shader *p_Shader, Texture2D *p_Texture, unsigned int p_NumberOfParticles)
	: m_Shader(p_Shader), m_ParticleTexture(p_Texture), m_NumberOfParticles(p_NumberOfParticles) {

	m_Shader->Use();
	glm::mat4 projection = glm::ortho(0.0f, p_ScreenSize.X(), p_ScreenSize.Y(), 0.0f, -1.0f, 1.0f);
	m_Shader->SetMat4("projection", projection);

	m_Particles.reserve(m_NumberOfParticles);
	for (unsigned int i = 0; i < m_NumberOfParticles; ++i) {
		m_Particles.emplace_back(Particle());
	}
}

unsigned int ParticleManager::FirstUnusedParticle() {
	static unsigned int s_LastUsedParticleIndex = 0;

	// Search from the last used particle, should return almost instantly.
	for (unsigned int i = s_LastUsedParticleIndex; i < m_NumberOfParticles; ++i) {
		if (m_Particles[i].m_Life <= 0.0f) {
			s_LastUsedParticleIndex = i;
			return i;
		}
	}

	// Do a linear search.
	for (unsigned int i = 0; i < s_LastUsedParticleIndex; ++i) {
		if (m_Particles[i].m_Life <= 0.0f) {
			s_LastUsedParticleIndex = i;
			return i;
		}
	}

	// If all of the particles are taken, then override the first one.
	s_LastUsedParticleIndex = 0;
	return 0;
}

void ParticleManager::RespawnParticle(Particle &p_Particle, Vector2Df p_Position, Vector2Df p_Velocity) {
	// Another particle is spawned in the same position as the previous, but the velocity is nothing.
	// This causes the particle to "aim" in a different direction. (More aesthetically pleasing.)
	static float s_PreviousAngle(0.0f);
	float randomNumber = RandomiserInstance.GetUniformRealRandomNumber(0.0f, 1.0f);

	p_Particle.m_Position = p_Position + randomNumber;
	p_Particle.m_Colour = glm::vec4(randomNumber, randomNumber, randomNumber, 1.0f);
	p_Particle.m_Life = m_ParticleLifeDuration;
	p_Particle.m_Velocity = p_Velocity * 0.1f;

	float angle(0.0f);
	if (p_Particle.m_Velocity.Magnitude() <= 0.05f) {
		angle = s_PreviousAngle;
	}
	else {
		// Calculate an angle between where the particle and the direction it's heading in.
		Vector2Df difference = (p_Particle.m_Position - p_Particle.m_Velocity * 100.0f) - p_Particle.m_Position;
		angle = std::atan2f(difference.Y(), difference.X());
		angle = static_cast<float>((angle * 180.0f / M_PI) + 90.0f);
	}

	p_Particle.m_Orientation = angle;
	s_PreviousAngle = angle;
}

void ParticleManager::Update(float p_DeltaTime, Vector2Df p_MousePosition) {
	static Vector2Df s_PreviousMousePosition(p_MousePosition);

	// Calculate the speed of the mouse's movement.
	// Speed = Distance / Time.
	Vector2Df velocity((p_MousePosition - s_PreviousMousePosition) / p_DeltaTime);

	// Spawn new particles.
	for (unsigned int i = 0; i < m_NumberOfParticlesToSpawnEachTime; ++i) {
		unsigned int particleIndex = FirstUnusedParticle();
		RespawnParticle(m_Particles[particleIndex], p_MousePosition, velocity);
	}

	// Update all the particles.
	for (auto &particle : m_Particles) {
		particle.m_Life -= p_DeltaTime;
		if (particle.m_Life > 0.0f) {
			particle.m_Position = particle.m_Position - (particle.m_Velocity * p_DeltaTime);
			particle.m_Colour.a -= p_DeltaTime * 2.5f;
		}
	}

	s_PreviousMousePosition = p_MousePosition;
}

void ParticleManager::Render() {
	static RectangleShape s_RectangleShape(Vector2Df(0.0f, 0.0f), Vector2Df(15.0f, 15.0f), m_ParticleTexture);

	m_Shader->Use();
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE);
	for (auto &particle : m_Particles) {
		if (particle.m_Life > 0.0f) {
			s_RectangleShape.SetPosition(particle.m_Position);
			s_RectangleShape.SetColour(particle.m_Colour);
			s_RectangleShape.SetRotation(particle.m_Orientation);
			s_RectangleShape.Render(*m_Shader);
		}
	}
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
}

unsigned int ParticleManager::GetNumberOfParticlesToSpawnEachTime() const {
	return m_NumberOfParticlesToSpawnEachTime;
}

void ParticleManager::SetNumberOfParticlesToSpawnEachTime(unsigned int p_NumberToSpawn) {
	m_NumberOfParticlesToSpawnEachTime = p_NumberToSpawn;
}