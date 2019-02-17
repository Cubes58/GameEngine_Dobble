#pragma once

#include "GLCore.hpp"

class Texture2D {
private:
	// Unique ID of the texture.
	GLuint m_ID;

	// Texture image dimensions.
	GLuint m_Width;
	GLuint m_Height;

	// Texture format.
	GLuint m_InternalFormat;	// Format of the texture object.
	GLuint m_ImageFormat;		// Format of the loaded image.

	// Texture configuration.
	GLuint m_WrapS;				// Wrapping mode on the S axis.
	GLuint m_WrapT;				// Wrapping mode on the T axis.
	GLuint m_FilterMin;			// Filtering mode if texture pixels < screen pixels.
	GLuint m_FilterMax;			// Filtering mode if textures pixels > screen pixels.

public:
	Texture2D();
	~Texture2D() = default;

	void Generate(GLuint p_Width, GLuint p_Height, unsigned char *p_Data);	// Generates a texture from image data.
	void Bind() const;	// Binds the texture as the current active gl::TEXTURE_2D texture object.

	const GLuint &GetID() const;
	void SetID(GLuint p_ID);

	const GLuint &GetWidth() const;
	const GLuint &GetHeight() const;

	const GLuint &GetInternalFormat() const;
	void SetInternalFormat(GLuint p_InternalFormat);
	const GLuint &GetImageFormat() const;
	void SetImageFormat(GLuint p_ImageFormat);

	const GLuint &GetWrapS() const;
	const GLuint &GetWrapT() const;
	const GLuint &GetFilterMin() const;
	const GLuint &GetFilterMax() const;
};