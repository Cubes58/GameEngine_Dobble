#pragma once

#include "Vector2D.h"
#include "Texture2D.h"

class Shader;

class PostProcessor {
private:
	Texture2D m_Texture;
	Vector2Df m_TextureSize;

	unsigned int m_MultisampledFrameBufferObject;
	unsigned int m_FrameBufferObject;
	unsigned int m_RenderBufferObject;
	unsigned int m_VAO;
	unsigned int m_VBO;

	bool m_Shake = false;
	float m_ShakeTime = 0.0f;
	float m_TimePassedSinceShakeActive = 0.0f;
	float m_AccumulatedTime = 0.0f;

	bool m_InvertColours = false;
	bool m_Chaos = false;
	Shader *m_Shader;

	void InitializeRenderData();

public:
	PostProcessor(const Vector2Df &p_TextureSize);
	~PostProcessor();

	void Update(float p_DeltaTime);	// Updates the time, for the shader uniform.

	void BeginRender();		// Prepares the framebuffer operations, before redering.
	void EndRender();		// Called after rendering the game, so it stores all the rendered data into a texture object.
	void Render();			// Renders the PostProcessor texture quad (large screen sized sprite).

	void SetShakeTime(float p_ShakeTime = 0.05f) {
		m_ShakeTime = p_ShakeTime;
	}
	
	bool GetShakeState() const {
		return m_Shake;
	}
	void SetShakeState(bool p_ShakeState) {
		m_Shake = p_ShakeState;
		if (m_Shake) {
			m_TimePassedSinceShakeActive = 0.0f;
		}
	}

	bool GetInvertColoursState() const {
		return m_InvertColours;
	}
	void SetInvertColoursState(bool p_InvertColoursState) {
		m_InvertColours = p_InvertColoursState;
	}

	bool GetChaosState() const {
		return m_Chaos;
	}
	void SetChaosState(bool p_ChaosState) {
		m_Chaos = p_ChaosState;
	}

	void SetTextureSize(const Vector2Df &p_TextureSize) {
		m_TextureSize = p_TextureSize;
	}
};