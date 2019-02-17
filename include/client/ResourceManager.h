#pragma once

#include <map>
#include <vector>
#include <string>

#include "GLCore.hpp"

#include "Texture2D.h"
#include "Shader.h"

class RenderSystem;

class ResourceManager {
private:
	friend RenderSystem;

	std::map<std::string, Texture2D> m_Textures;
	std::map<std::string, Shader> m_Shaders;
	std::vector<Texture2D*> m_TextureIDs;

	ResourceManager();	
	~ResourceManager();

	bool LoadShaderFromFile(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile = " ");
	bool LoadTextureFromFile(const std::string &p_File, GLboolean p_GammaCorrection = false);

public:
	static ResourceManager &Instance() {
		static ResourceManager s_ResourceManager;

		return s_ResourceManager;
	}

	bool LoadShadersFromFolder(const std::string &p_FolderName);
	Shader *LoadShader(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile = " ");
	Shader *GetShader(const std::string &p_Name);

	bool LoadTexturesFromFolder(const std::string &p_FolderName);
	Texture2D *LoadTexture(const std::string &p_File, GLboolean p_GammaCorrection = false);
	Texture2D *GetTexture(const std::string &p_Name);
	Texture2D *GetTexture(unsigned int p_TextureID);
};