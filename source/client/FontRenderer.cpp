#include "FontRenderer.h"

#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ResourceManager.h"
#include "Logger.h"

FontRenderer::FontRenderer(const std::string &p_FontPath, const Vector2Df &p_ScreenSize)
	: m_FontPath(p_FontPath), m_ScreenSize(p_ScreenSize) {

	m_FontShader = ResourceManagerInstance.LoadShader("resources/shaders/FontShader.vert", "resources/shaders/FontShader.frag");
	InitFont();
}

void FontRenderer::InitFont() {
	// FreeType.
	FT_Library ft;

	// All functions return a value different to 0, whenever an error occurred.
	if (FT_Init_FreeType(&ft))
		Log(Type::FAULT) << "FREETYPE: Could not init FreeType Library.";

	// Load font as face.
	FT_Face face;
	if (FT_New_Face(ft, m_FontPath.c_str(), 0, &face)) {
		Log(Type::FAULT) << "FREETYPE: Failed to load font.";
		return;
	}

	// Set size to load glyphs as.
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction.
	gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set.
	for (GLubyte c = 0; c < 128; c++) {
		// Load the character glyph.
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			Log(Type::FAULT) << "FREETYTPE: Failed to load Glyph.";
			continue;
		}

		// Generate a texture.
		GLuint texture;
		gl::GenTextures(1, &texture);
		gl::BindTexture(gl::TEXTURE_2D, texture);
		gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, gl::RED, gl::UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		// Set the texture options.
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);
		gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
		
		// Now store character for later use.
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		m_Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	gl::BindTexture(gl::TEXTURE_2D, 0);

	// Destroy FreeType once we're finished.
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads.
	gl::GenVertexArrays(1, &m_Font_VAO);
	gl::GenBuffers(1, &m_Font_VBO);
	gl::BindVertexArray(m_Font_VAO);
	gl::BindBuffer(gl::ARRAY_BUFFER, m_Font_VBO);

	// Dynamic draw as the text may change frequently.
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, gl::DYNAMIC_DRAW);
	gl::EnableVertexAttribArray(0);
	gl::VertexAttribPointer(0, 4, gl::FLOAT, gl::FALSE_, 4 * sizeof(GLfloat), 0);
	gl::BindBuffer(gl::ARRAY_BUFFER, 0);
	gl::BindVertexArray(0);
}

void FontRenderer::RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, const glm::vec3 &p_Colour) {
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_ScreenSize.X()), 0.0f, static_cast<float>(m_ScreenSize.Y()));

	float pixelValueX = p_XPosition * m_ScreenSize.X();
	float pixelValueY = p_YPosition * m_ScreenSize.Y();

	gl::UseProgram(m_FontShader->GetID());
	m_FontShader->SetMat4("projection", projection);

	// Activate corresponding render state.
	m_FontShader->SetVec3("textColour", p_Colour);
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindVertexArray(m_Font_VAO);

	// Iterate through all characters.
	std::string::const_iterator c;
	for (c = p_Text.begin(); c != p_Text.end(); c++) {
		Character ch = m_Characters[*c];

		GLfloat xpos = pixelValueX + ch.m_Bearing.x * p_Scale;
		GLfloat ypos = pixelValueY - (ch.m_Size.y - ch.m_Bearing.y) * p_Scale;

		GLfloat w = ch.m_Size.x * p_Scale;
		GLfloat h = ch.m_Size.y * p_Scale;

		// Update VBO for each character.
		GLfloat vertices[6][4] = {
		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }};

		// Render glyph texture over quad.
		gl::BindTexture(gl::TEXTURE_2D, ch.m_TextureID);

		// Update content of VBO memory.
		gl::BindBuffer(gl::ARRAY_BUFFER, m_Font_VBO);
		gl::BufferSubData(gl::ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		gl::BindBuffer(gl::ARRAY_BUFFER, 0);

		// Render quad.
		gl::DrawArrays(gl::TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels).
		pixelValueX += (ch.m_Advance >> 6) * p_Scale; // Bitshift by 6 to get value in pixels (2^6 = 64).
	}
	gl::BindVertexArray(0);
	gl::BindTexture(gl::TEXTURE_2D, 0);
}

void FontRenderer::RenderText(Text &p_Text) {
	p_Text.Update();
	RenderText(p_Text.m_Text, p_Text.m_Position.X(), p_Text.m_Position.Y(), p_Text.m_Size, p_Text.m_Colour);
}

void FontRenderer::SetFont(const std::string &p_FontPath) {
	m_FontPath = p_FontPath;
	InitFont();
}

void FontRenderer::SetScreenSize(const Vector2Df &p_ScreenSize) {
	m_ScreenSize = p_ScreenSize;
}