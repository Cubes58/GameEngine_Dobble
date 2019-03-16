/**
@file PostProcessor.h
@brief Manages the post processor effects.
*/
#pragma once

#include <array>

#include "Vector2D.h"
#include "Texture2D.h"

class Shader;

static const constexpr unsigned int s_NumberOfBlurKernels = 10;	//!< The number of blur kernels, used.

/*! \class PostProcessor
	\brief Manages the post processor effects.
*/

class PostProcessor {
private:
	Texture2D m_Texture;	//!< Stores the frame buffer object information.
	
	std::array<std::array<float, 9>, s_NumberOfBlurKernels> m_BlurKernels;	//!< An array of blur kernels, with different sigmas.
	Vector2Df m_TextureSize;	//!< The size of the texture.

	unsigned int m_MultisampledFrameBufferObject;	//!< The ID of the multisampled frame buffer object.
	unsigned int m_FrameBufferObject;	//!< The ID of the frame buffer object.
	unsigned int m_RenderBufferObject;	//!< The ID to the render buffer object that's used by the frame buffer object.
	unsigned int m_VAO;	//!< The VAO ID for Post processor.
	unsigned int m_VBO;	//!< Stores the vertex data and texture coordinates for the shape.

	float m_ShakeTime = 0.0f;	//!< The amount of time the screen should shake for.
	float m_TimePassedSinceShakeActive = 0.0f;	//!< The amount of time that's passed since the screen has started shaking.
	float m_AccumulatedTime = 0.0f;	//!< The amount of time that's passed, since the instance was created.

	bool m_Shake = false;	//!< Manages whether the screen should shake, or not.
	bool m_InvertColours = false;	//!< Manages whether the screen's colours should be inverted.
	bool m_Chaos = false;	//!< Manages whether the edge kernel should be used.
	Shader *m_Shader;	//!< A pointer to the shader used by the post processor.

	/*!
		\brief Initialize the render data.
		\return Nothing.
	*/
	void InitializeRenderData();
	/*!
		\briefInitialize the blur kernel.
		\return Nothing.
	*/
	void InitializeBlurKernels();

public:
	/*!
		\brief Constructor.
		\param p_TextureSize The size of the texture, to be used for the post processing renderer.
	*/
	PostProcessor(const Vector2Df &p_TextureSize);
	/*!
		\brief Destructor.
	*/
	~PostProcessor();

	/*!
		\brief Updates the post processor.
		\return Nothing.
	*/
	void Update(float p_DeltaTime);	// Updates the time, for the shader uniform.

	/*!
		\brief Prepares the framebuffer operations, before redering.
		\return Nothing.
	*/
	void BeginRender();
	/*!
		\brief Called after rendering the game, so it stores all the rendered data into a texture object.
		\return Nothing.
	*/
	void EndRender();
	/*!
		\brief Renders the PostProcessor texture quad (large screen sized sprite).
		\return Nothing.
	*/
	void Render();

	/*!
		\brief Sets the shake effect duration.
		\return Nothing.
	*/
	void SetShakeTime(float p_ShakeTime = 0.05f) {
		m_ShakeTime = p_ShakeTime;
	}
	
	/*!
		\brief Gets the screen shake state.
		\return Returns the screen shake state, true if the screen if shaking, otherwise false.
	*/
	bool GetShakeState() const {
		return m_Shake;
	}
	/*!
		\brief Sets the screen shaking state.
		\param p_ShakeState The new state of the shake effect.
		\return Nothing.
	*/
	void SetShakeState(bool p_ShakeState) {
		m_Shake = p_ShakeState;
		if (m_Shake) {
			m_TimePassedSinceShakeActive = 0.0f;
		}
	}

	/*!
		\brief Gets the inverted colours state.
		\return Returns true if the inverted colours state is set to true, otherwise false.
	*/
	bool GetInvertColoursState() const {
		return m_InvertColours;
	}
	/*!
		\brief Sets the inverted colours state.
		\param p_InvertColoursState The state to set the inverted colours state to.
		\return Nothing.
	*/
	void SetInvertColoursState(bool p_InvertColoursState) {
		m_InvertColours = p_InvertColoursState;
	}

	/*!
		\brief Gets the state of the chaos.
		\return Returns true if the chaos state is activated, otherwise false.
	*/
	bool GetChaosState() const {
		return m_Chaos;
	}
	/*!
		\brief Sets the the chaos state.
		\param p_ChaosState The new state of the chaos settings. 
		\return Nothing.
	*/
	void SetChaosState(bool p_ChaosState) {
		m_Chaos = p_ChaosState;
	}

	/*!
		\brief Sets the size of the screen texture.
		\param p_TextureSize The new size of the texture.
		\return Nothing.
	*/
	void SetTextureSize(const Vector2Df &p_TextureSize) {
		m_TextureSize = p_TextureSize;
	}
};