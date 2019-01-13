#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image/stb_image.h"

#include "Logger.h"

// Instantiate the static variables.
std::map<std::string, Texture2D> ResourceManager::m_Textures;
std::map<std::string, Shader> ResourceManager::m_Shaders;

Shader ResourceManager::LoadShaderFromFile(const GLchar *p_vShaderFile, const GLchar *p_fShaderFile, const GLchar *p_gShaderFile) {
	// Retrieve the vertex/fragment source code, from the file path.
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try {
		// Open files.
		std::ifstream vertexShaderFile(p_vShaderFile);
		std::ifstream fragmentShaderFile(p_fShaderFile);

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		// Read the files' buffer contents into streams.
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		// Close file handlers.
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// Convert the stream into a string.
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// If the geometry shader path is present, also load a geometry shader.
		if (p_gShaderFile != nullptr) {
			std::ifstream geometryShaderFile(p_gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception p_Exception) {
		Log(MessageType::FAULT) << "ERROR::SHADER: Failed to read shader files.";
	}
	catch (...) {
		Log(MessageType::FAULT) << "SOMETHING BAD WENT WRONG WITH THE SHADER LOAD FROM FILE CODE.";
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();

	// Create the shader object from the source code.
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, p_gShaderFile != nullptr ? gShaderCode : nullptr);

	return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const GLchar *p_File, GLboolean p_GammaCorrection) {
	// Create a Texture object.
	Texture2D texture;

	// Load image.
	int width = 0;
	int height = 0;
	int nrComponents = 0;

	unsigned char *image = stbi_load(p_File, &width, &height, &nrComponents, 0);
	if (image) {
		if (nrComponents == 1) {
			texture.SetInternalFormat(gl::RED);
			texture.SetImageFormat(gl::RED);
		}
		else if (nrComponents == 3) {
			texture.SetInternalFormat(p_GammaCorrection ? gl::SRGB : gl::RGB);
			texture.SetImageFormat(gl::RGB);
		}
		else if (nrComponents == 4) {
			texture.SetInternalFormat(p_GammaCorrection ? gl::SRGB_ALPHA : gl::RGBA);
			texture.SetImageFormat(gl::RGBA);
		}
	}

	texture.Generate(width, height, image);
	stbi_image_free(image);

	return texture;
}

Shader &ResourceManager::LoadShader(const GLchar *p_vShaderFile, const GLchar *p_fShaderFile, const GLchar *p_gShaderFile, const std::string &p_Name) {
	m_Shaders.insert(std::pair<std::string, Shader>(p_Name, LoadShaderFromFile(p_vShaderFile, p_fShaderFile, p_gShaderFile)));

	return m_Shaders[p_Name];
}

Shader &ResourceManager::GetShader(const std::string &p_Name) {
	return m_Shaders[p_Name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *p_File, GLboolean p_GammaCorrection, const std::string &p_Name) {
	m_Textures[p_Name] = LoadTextureFromFile(p_File, p_GammaCorrection);

	return m_Textures[p_Name];
}

Texture2D &ResourceManager::GetTexture(const std::string &p_Name) {
	return m_Textures[p_Name];
}

void ResourceManager::Clear() {
	for (auto &iter : m_Shaders)
		gl::DeleteProgram(iter.second.GetID());
	for (auto &iter : m_Textures)
		gl::DeleteTextures(1, &iter.second.GetID());
}