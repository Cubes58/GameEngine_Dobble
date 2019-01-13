#include "Texture2D.h"

Texture2D::Texture2D() : m_Width(0), m_Height(0), m_InternalFormat(gl::RGB), m_WrapS(gl::REPEAT),
m_WrapT(gl::REPEAT), m_FilterMin(gl::LINEAR), m_FilterMax(gl::LINEAR), m_ImageFormat(gl::RGB) {
	gl::GenTextures(1, &m_ID);
}

void Texture2D::Generate(GLuint p_Width, GLuint p_Height, unsigned char *p_Data) {
	m_Width = p_Width;
	m_Height = p_Height;

	// Create the Texture object.
	gl::BindTexture(gl::TEXTURE_2D, m_ID);
	gl::TexImage2D(gl::TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_ImageFormat, gl::UNSIGNED_BYTE, p_Data);
	gl::GenerateMipmap(gl::TEXTURE_2D);

	// Set the Texture's wrap and filter modes.
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, m_WrapS);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, m_WrapT);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, m_FilterMin);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, m_FilterMax);

	// Unbind the Texture.
	gl::BindTexture(gl::TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
	gl::BindTexture(gl::TEXTURE_2D, m_ID);
}

const GLuint &Texture2D::GetID() const {
	return m_ID;
}

void Texture2D::SetID(GLuint p_ID) {
	m_ID = p_ID;
}

const GLuint &Texture2D::GetWidth() const {
	return m_Width;
}

const GLuint &Texture2D::GetHeight() const {
	return m_Height;
}

const GLuint &Texture2D::GetInternalFormat() const {
	return m_InternalFormat;
}

void Texture2D::SetInternalFormat(GLuint p_InternalFormat) {
	m_InternalFormat = p_InternalFormat;
}

const GLuint &Texture2D::GetImageFormat() const {
	return m_ImageFormat;
}

void Texture2D::SetImageFormat(GLuint p_ImageFormat) {
	m_ImageFormat = p_ImageFormat;
}

const GLuint &Texture2D::GetWrapS() const {
	return m_WrapS;
}

const GLuint &Texture2D::GetWrapT() const {
	return m_WrapT;
}

const GLuint &Texture2D::GetFilterMin() const {
	return m_FilterMin;
}

const GLuint &Texture2D::GetFilterMax() const {
	return m_FilterMax;
}