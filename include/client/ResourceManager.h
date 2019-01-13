#pragma once

#include <map>
#include <string>

#include "GLCore.hpp"

#include "Texture2D.h"
#include "Shader.h"

class ResourceManager {
private:
	ResourceManager() = default;

	// Loads and generates a shader, from a file.
	static Shader LoadShaderFromFile(const GLchar *p_vShaderFile, const GLchar *p_fShaderFile, const GLchar *p_gShaderFile);
	// Loads a single texture from a file.
	static Texture2D LoadTextureFromFile(const GLchar *p_File, GLboolean p_GammaCorrection = false);

public:
	// Resource storage.
	static std::map<std::string, Shader> m_Shaders;
	static std::map<std::string, Texture2D> m_Textures;

	// Loads (and generates) a shader program from file, loading vertex, fragment (and a geometry) shader's source code.
	static Shader &LoadShader(const GLchar *p_vShaderFile, const GLchar *p_fShaderFile, const GLchar *p_gShaderFile, const std::string &p_Name);
	static Shader &GetShader(const std::string &p_Name);

	// Loads (and generates) a texture from file.
	static Texture2D LoadTexture(const GLchar *p_File, GLboolean p_GammaCorrection, const std::string &p_Name);
	static Texture2D &GetTexture(const std::string &p_Name);

	// Properly de-allocates all loaded resources.
	static void Clear();
};