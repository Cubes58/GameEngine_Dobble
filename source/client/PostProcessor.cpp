#include "PostProcessor.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "ResourceManager.h"
#include "Logger.h"

#include "RectangleShape.h"

PostProcessor::PostProcessor(const Vector2Df &p_TextureSize) : m_TextureSize(p_TextureSize) {
	m_Shader = ResourceManagerInstance.GetShader("PostProcessingEffects");

	Log(Type::INFO) << "Texture width: " << m_TextureSize.X() << "\t" << "Texture height: " << m_TextureSize.Y();

	// Initialize the Render Buffer and Frame Buffer Objects.
	gl::GenFramebuffers(1, &m_MultisampledFrameBufferObject);
	gl::GenFramebuffers(1, &m_FrameBufferObject);
	gl::GenRenderbuffers(1, &m_RenderBufferObject);

	// Initialize the Render Buffer Storage, with a multisampled colour buffer (No need for a depth or stencil buffer).
	gl::BindFramebuffer(gl::FRAMEBUFFER, m_MultisampledFrameBufferObject);
	gl::BindRenderbuffer(gl::RENDERBUFFER, m_RenderBufferObject);
	// Allocate storage for the render buffer object.
	gl::RenderbufferStorageMultisample(gl::RENDERBUFFER, 8, gl::RGB, (int)m_TextureSize.X(), (int)m_TextureSize.Y());
	// Attach the Multisampled Render Buffer Object.
	gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::RENDERBUFFER, m_RenderBufferObject);

	// Check for any errors.
	if (gl::CheckFramebufferStatus(gl::FRAMEBUFFER) != gl::FRAMEBUFFER_COMPLETE) {
		Log(Type::FAULT) << "POST_PROCESSOR: Failed to initialize m_MultisampledFrameBufferObject.";
	}

	// Initialize the Frame Buffer Object/Texture to blit multisampled colour-buffer.
	gl::BindFramebuffer(gl::FRAMEBUFFER, m_FrameBufferObject);
	m_Texture.Generate((int)m_TextureSize.X(), (int)m_TextureSize.Y(), NULL);
	gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, m_Texture.GetID(), 0);

	// Check for any errors.
	if (gl::CheckFramebufferStatus(gl::FRAMEBUFFER) != gl::FRAMEBUFFER_COMPLETE) {
		Log(Type::FAULT) << "POST_PROCESSOR: Failed to initialize m_FrameBufferObject.";
	}
	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

	InitializeRenderData();

	// Set uniforms.
	m_Shader->Use();
	m_Shader->SetInt("scene", 0);
	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // Top-left.
		{  0.0f,    offset  },  // Top-center.
		{  offset,  offset  },  // Top-right.
		{ -offset,  0.0f    },  // Center-left.
		{  0.0f,    0.0f    },  // Center-center.
		{  offset,  0.0f    },  // Center-right.
		{ -offset, -offset  },  // Bottom-left.
		{  0.0f,   -offset  },  // Bottom-center.
		{  offset, -offset  }   // Bottom-right.
	};
	gl::Uniform2fv(gl::GetUniformLocation(m_Shader->GetID(), "offsets"), 9, (GLfloat*)offsets);

	int edgeKernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	gl::Uniform1iv(gl::GetUniformLocation(m_Shader->GetID(), "edgeKernel"), 9, edgeKernel);

	InitializeBlurKernels();
	gl::Uniform1fv(gl::GetUniformLocation(m_Shader->GetID(), "blurKernel"), 9, &m_BlurKernels[9][0]);
}

PostProcessor::~PostProcessor() {
	// Set the Frame Buffer Object back to the default one.
	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);

	// Clean up the memory.
	gl::DeleteBuffers(1, &m_VBO);
	gl::DeleteVertexArrays(1, &m_VAO);
	gl::DeleteRenderbuffers(1, &m_RenderBufferObject);
	gl::DeleteFramebuffers(1, &m_MultisampledFrameBufferObject);
	gl::DeleteFramebuffers(1, &m_FrameBufferObject);
}

void PostProcessor::InitializeRenderData() {
	float vertices[] = {
		// Positions  // Texture Coordinates.
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	gl::GenBuffers(1, &m_VBO);
	gl::BindBuffer(gl::ARRAY_BUFFER, m_VBO);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(vertices), &vertices[0], gl::STATIC_DRAW);

	gl::GenVertexArrays(1, &m_VAO);
	gl::BindVertexArray(m_VAO);

	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 4, gl::FLOAT, gl::FALSE_, 4 * sizeof(gl::FLOAT), NULL);

	// Unbind.
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);
	gl::BindVertexArray(0);
}

void PostProcessor::InitializeBlurKernels() {
	// Sigma 0.1
	m_BlurKernels[0] = {
		0,	0,			0,
		0,	0.999998,	0,
		0,	0,			0
	};
	// Sigma 0.2
	m_BlurKernels[1] = {
		0.000039,	0.006133,	0.000039,
		0.006133,	0.975316,	0.006133,
		0.000039,	0.006133,	0.000039
	};
	// Sigma 0.3
	m_BlurKernels[2] = {
		0.002284,	0.043222,	0.002284,
		0.043222,	0.817976,	0.043222,
		0.002284,	0.043222,	0.002284
	};
	// Sigma 0.4
	m_BlurKernels[3] = {
		0.011147,	0.083286,	0.011147,
		0.083286,	0.622269,	0.083286,
		0.011147,	0.083286,	0.011147
	};
	// Sigma 0.5
	m_BlurKernels[4] = {
		0.024879,	0.107973,	0.024879,
		0.107973,	0.468592,	0.107973,
		0.024879,	0.107973,	0.024879
	};
	// Sigma 0.6
	m_BlurKernels[5] = {
		0.039436,	0.119713,	0.039436,
		0.119713,	0.363404,	0.119713,
		0.039436,	0.119713,	0.039436
	};
	// Sigma 0.7
	m_BlurKernels[6] = {
		0.052356,	0.124103,	0.052356,
		0.124103,	0.294168,	0.124103,
		0.052356,	0.124103,	0.052356
	};
	// Sigma 0.8
	m_BlurKernels[7] = {
		0.06292,	0.124998,	0.06292,
		0.124998,	0.248326,	0.124998,
		0.06292,	0.124998,	0.06292
	};
	// Sigma 0.9
	m_BlurKernels[8] = {
		0.071282,	0.124423,	0.071282,
		0.124423,	0.217183,	0.124423,
		0.071282,	0.124423,	0.071282
	};
	// Sigma 1.0
	m_BlurKernels[9] = {
		0.077847,	0.123317,	0.077847,
		0.123317,	0.195346,	0.123317,
		0.077847,	0.123317,	0.077847
	};
}

void PostProcessor::Update(float p_DeltaTime) {
	m_AccumulatedTime += p_DeltaTime;
	m_TimePassedSinceShakeActive += p_DeltaTime;

	m_Shader->Use();
	m_Shader->SetFloat("time", m_AccumulatedTime);

	if (m_TimePassedSinceShakeActive >= m_ShakeTime) {
		m_Shake = false;
	}
}

void PostProcessor::BeginRender() {
	gl::BindFramebuffer(gl::FRAMEBUFFER, m_MultisampledFrameBufferObject);
	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT);		// Maybe also the depth buffer.
}

void PostProcessor::EndRender() {
	// Resolve multisampled colour-buffer into intermediate Frame Buffer Object, to store the texture.
	gl::BindFramebuffer(gl::READ_FRAMEBUFFER, m_MultisampledFrameBufferObject);
	gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, m_FrameBufferObject);
	gl::BlitFramebuffer(0, 0, (int)m_TextureSize.X(), (int)m_TextureSize.Y(), 0, 0, (int)m_TextureSize.X(), (int)m_TextureSize.Y(), gl::COLOR_BUFFER_BIT, gl::NEAREST);

	// Bind both the Read and Write Frame Buffer to the default Frame Buffer Object.
	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void PostProcessor::Render() {
	m_Shader->Use();

	// Set uniforms.
	m_Shader->SetBool("shake", m_Shake);
	m_Shader->SetBool("invertColours", m_InvertColours);
	m_Shader->SetBool("chaos", m_Chaos);

	// Render the textured quad.
	gl::ActiveTexture(gl::TEXTURE0);
	m_Texture.Bind();

	gl::BindVertexArray(m_VAO);
	gl::DrawArrays(gl::TRIANGLES, 0, 6);

	// Unbind the VAO.
	gl::BindVertexArray(0);
}