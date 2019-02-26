#pragma once

#include <map>
#include <string>

#include "Vector2D.h"
#include "Shader.h"

class FontRenderer {
private:
	Vector2D<float> m_ScreenSize;	//!< Screen dimensions.

	/**
	* A structure to represent a Character.
	*/
	struct Character {
		GLuint m_TextureID;		//!< ID handle of the glyph texture.
		glm::ivec2 m_Size;		//!< Size of the glyph.
		glm::ivec2 m_Bearing;	//!< Offset from baseline to left/top of the glyph.
		GLuint m_Advance;		//!< Horizontal offset to advance to next glyph.
	};
	std::map<GLchar, Character> m_Characters;	//!< A map of the characters.
	GLuint m_Font_VBO;	//!< The font vertex buffer object.
	GLuint m_Font_VAO;	//!< The font vertex array object.

	std::string m_FontPath;	//!< Path to the font, being used.
	Shader *m_FontShader;	//!< Shader for the font.

	/*!
		\brief initialises the font.
	*/
	void InitFont();

public:

	/*!
		\brief Constructor.
		\param p_FontPath directory path to the font, that'll be used, to render the text.
		\param p_ScreenSize size of the screen.
	*/
	FontRenderer(const std::string &p_FontPath, const Vector2D<float> &p_ScreenSize);

	/*!
		\brief Used to render text.
		\param p_Text the text to be displayed.
		\param p_XPosition the screen's X position, where the text should be rendered.
		\param p_YPosition the screen's Y position, where the text should be rendered.
		\param p_Scale the text's scale.
		\param p_Colour the colour of the text.
	*/
	void RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, const glm::vec3 &p_Colour);

	void SetFont(const std::string &p_FontPath);
	void SetScreenSize(const Vector2D<float> &p_ScreenSize);
};