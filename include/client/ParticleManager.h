/**
@file ParticleManager.h
@brief Manages the particles.
*/
#pragma once

#include <vector>

#include "Vector2D.h"

#include "Particle.h"

class Shader;
class Texture2D;

/*! \class ParticleManager
	\brief Manages the particles.
*/

class ParticleManager {
private:
	std::vector<Particle> m_Particles;	//!< A vector of the particles.
	unsigned int m_NumberOfParticles;	//!< Keeps track of the number of particles.
	unsigned int m_NumberOfParticlesToSpawnEachTime = 2;	//!< Manages how many particles should be spawned at once.
	float m_ParticleLifeDuration = 1.0f;	//!< Manages how long a particle should live for.
	Shader *m_Shader;	//!< Raw pointer to the shaders that'll be used to render the particles.
	Texture2D *m_ParticleTexture;	//!< A pointer to the particles' texture.

	/*!
		\brief Gets the first unused particle index.
		\return Returns the index to the first unused particle, or 0 if they're all in use.
	*/
	unsigned int FirstUnusedParticle();

	/*!
		\brief Respawns a particle
		\param p_Particle The "dead" particle (0.0f time left on life).
		\param p_Position The new position, of the particle.
		\param p_Velocity The new velocity, of the particle.
		\return Nothing.
	*/
	void RespawnParticle(Particle &p_Particle, Vector2Df p_Position = Vector2Df(0.0f, 0.0f), Vector2Df p_Velocity = Vector2Df(0.0f, 0.0f));

public:
	/*!
		\brief Constructor.
		\param p_ScreenSize The size of the screen.
		\param p_NumberOfParticles The number of particles that'll be used.
	*/
	ParticleManager(const Vector2Df &p_ScreenSize, unsigned int p_NumberOfParticles = 250);
	/*!
		\brief Constructor.
		\param p_ScreenSize The size of the screen.
		\param p_Texture A pointer to the texture the particles will use.
		\param p_NumberOfParticles The number of particles that'll be used.
	*/
	ParticleManager(const Vector2Df &p_ScreenSize, Shader *p_Shader, Texture2D *p_Texture = nullptr, unsigned int p_NumberOfParticles = 250);

	/*!
		\brief Updates the particles and respawns new ones.
		\param p_DeltaTime The delta time between the current frame and the previous one.
		\param p_MousePosition The position of the mouse (where the particle will be spawned).
		\return Nothing.
	*/
	void Update(float p_DeltaTime, Vector2Df p_MousePosition);
	/*!
		\brief Renders the particles.
		\return Nothing.
	*/
	void Render();

	/*!
		\brief Gets the number of particles to spawn on each update.
		\return Returns the number of particles to spawn on each update.
	*/
	unsigned int GetNumberOfParticlesToSpawnEachTime() const;
	/*!
		\brief Sets the number of particles to spawn on each update.
		\param p_NumberToSpawn The number of particles to spawn on each update.
		\return Nothing.
	*/
	void SetNumberOfParticlesToSpawnEachTime(unsigned int p_NumberToSpawn);
};