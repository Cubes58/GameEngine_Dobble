/**
@file Texture2D.h
@brief A Texture2D class.
*/
#pragma once

#include "GLCore.hpp"

/*! \class Texture2D
	\brief A class that stores the properties necessary for a 2D texture, and has behaviour to manipulate it.
*/

class Texture2D {
private:
	GLuint m_ID;		//!< Unique ID of the texture.

	GLuint m_Width;		//!< Width of the texture.
	GLuint m_Height;	//!< Height of the tetxure.

	// Texture format.
	GLuint m_InternalFormat;	//!< Format of the texture object.
	GLuint m_ImageFormat;		//!< Format of the loaded image.

	// Texture configuration.
	GLuint m_WrapS;				//!< Wrapping mode on the S axis.
	GLuint m_WrapT;				//!< Wrapping mode on the T axis.
	GLuint m_FilterMin;			//!< Filtering mode if texture pixels < screen pixels.
	GLuint m_FilterMax;			//!< Filtering mode if textures pixels > screen pixels.

public:
	Texture2D();	//!< Constructor.
	~Texture2D() = default;	//!< Destructor.

	/*!
		\brief Generates the 2D texture.
		\param p_Width The width of the texture.
		\param p_Height The height of the texture.
		\param p_Data A pointer to the texture's pixel data.
		\return Nothing.
	*/
	void Generate(GLuint p_Width, GLuint p_Height, unsigned char *p_Data);	// Generates a texture from image data.
	/*!
		\brief Binds the texture as the current active gl::TEXTURE_2D texture object.
		\return Nothing.
	*/
	void Bind() const;

	/*!
		\brief Gets the texture's ID.
		\return Returns the texture's ID.
	*/
	const GLuint &GetID() const;
	/*!
		\brief Sets the texture's ID.
		\param p_ID The texture's new ID.
		\return Nothing.
	*/
	void SetID(GLuint p_ID);

	/*!
		\brief Gets the texture's width.
		\return Returns the width of the texture.
	*/
	const GLuint &GetWidth() const;
	/*!
		\brief Gets the texture' height.
		\return Returns the height of the texture.
	*/
	const GLuint &GetHeight() const;

	/*!
		\brief Get the format of the texture's data.
		\return Returns the format of the texture's data.
	*/
	const GLuint &GetInternalFormat() const;
	/*!
		\brief Sets the texture's internal format.
		\param p_InternalFormat The texture new internal format.
		\return Nothing.
	*/
	void SetInternalFormat(GLuint p_InternalFormat);
	/*!
		\brief Get the image's format used for the texture.
		\return Returns the format of the loaded in image.
	*/
	const GLuint &GetImageFormat() const;
	/*!
		\brief Sets the format of the loaded in image.
		\param p_ImageFormat The format of the loaded in image.
		\return Nothing.
	*/
	void SetImageFormat(GLuint p_ImageFormat);

	/*!
		\brief Gets the textures S axis wrapping mode.
		\return Returns the textures S axis wrapping mode.
	*/
	const GLuint &GetWrapS() const;
	/*!
		\brief Gets the textures T axis wrapping mode.
		\return Returns the textures T axis wrapping mode.
	*/
	const GLuint &GetWrapT() const;
	/*!
		\brief Gets the texture's minimum filter mode.
		\return Returns the texture's minimum filter mode.
	*/
	const GLuint &GetFilterMin() const;
	/*!
		\brief Gets the texture's maximum filter mode.
		\return Returns the texture's maximum filter mode.
	*/
	const GLuint &GetFilterMax() const;
};