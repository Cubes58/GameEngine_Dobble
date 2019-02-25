#include "ResourceManager.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image/stb_image.h"

#include "FileSystemHelper.h"
#include "Logger.h"

ResourceManager::ResourceManager() {
	LoadShadersFromFolder("resources/shaders/");
	LoadTexturesFromFolder("resources/images/");
}

ResourceManager::~ResourceManager() {
	for (auto &shader : m_Shaders)
		gl::DeleteProgram(shader.second.GetID());
	for (auto &texture : m_Textures)
		gl::DeleteTextures(1, &texture.second.GetID());
}

bool ResourceManager::LoadShaderFromFile(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile) {
	// Retrieve the vertex/fragment source code, from the file path.
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try {
		// Open files.
		std::ifstream vertexShaderFile(p_VertexShaderFile);
		std::ifstream fragmentShaderFile(p_FragmentShaderFile);
		
		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
		if (vertexShaderFile.is_open() && fragmentShaderFile.is_open()) {
			// Read the files' buffer contents into streams.
			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();
		}
		// Close file handlers.
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// Convert the stream into a string.
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// If the geometry shader path is present, also load a geometry shader.
		if (p_GeometryShaderFile != " ") {
			std::ifstream geometryShaderFile(p_GeometryShaderFile);
			if (geometryShaderFile.is_open()) {
				std::stringstream gShaderStream;
				gShaderStream << geometryShaderFile.rdbuf();
				geometryShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
	}
	catch (std::exception p_Exception) {
		Log(Type::FAULT) << "ERROR::SHADER: Failed to read shader files.";
		return false;
	}
	catch (...) {
		Log(Type::FAULT) << "SOMETHING BAD WENT WRONG WITH THE SHADER LOAD FROM FILE CODE.";
		return false;
	}

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();

	// Create the shader object from the source code.
	Shader shader;
	bool success = shader.Compile(vShaderCode, fShaderCode, p_GeometryShaderFile != " " ? gShaderCode : nullptr);
	if (success) {
		m_Shaders.emplace(FileSystemHelper::GetNameFromFile(p_VertexShaderFile), shader);
	}

	return success;
}

bool ResourceManager::LoadTextureFromFile(const std::string &p_File, GLboolean p_GammaCorrection) {
	// Create a Texture object.
	Texture2D texture;

	// Load image.
	int width = 0;
	int height = 0;
	int nrComponents = 0;

	unsigned char *image = stbi_load(p_File.c_str(), &width, &height, &nrComponents, 0);
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

	if (width == 0 && height == 0 || image == nullptr)
		return false;

	std::string name = FileSystemHelper::GetNameFromFile(p_File);
	m_Textures.emplace(name, texture);
	auto iter = m_Textures.find(name);
	m_TextureIDs.emplace_back(&iter->second);

	return true;
}

bool ResourceManager::LoadShadersFromFolder(const std::string &p_FolderName) {
	std::vector<FileInformation> shaderFiles = FileSystemHelper::GetFilesInFolder(p_FolderName);
	FileSystemHelper::RetainRemoveFilesWithExtensions(shaderFiles, { ".vert", ".VERT", ".frag", ".FRAG", ".geom", ".GEOM" });

	struct ShaderGroup {
		std::string m_VertexShaderLocation;
		std::string m_FragmentShaderLocation;
		std::string m_GeometryShaderLocation;

		ShaderGroup() : m_VertexShaderLocation(" "), m_FragmentShaderLocation(" "), m_GeometryShaderLocation(" ") {}
		ShaderGroup(const std::string &p_VertexShaderLocation, const std::string &p_FragmentShaderLocation, const std::string &p_GeometryShaderLocation = " ")
			: m_VertexShaderLocation(p_VertexShaderLocation), m_FragmentShaderLocation(p_FragmentShaderLocation), m_GeometryShaderLocation(p_GeometryShaderLocation) {}
	};

	bool allSuccessful = true;
	std::map<std::string, ShaderGroup> shaders; 
	for (auto &shaderFile : shaderFiles) {
		auto iter = shaders.find(shaderFile.m_Name);
		if (iter == shaders.end()) {
			shaders.emplace(shaderFile.m_Name, ShaderGroup());
		}
		if (shaderFile.m_Extension == ".vert" || shaderFile.m_Extension == ".VERT")
			shaders[shaderFile.m_Name].m_VertexShaderLocation = shaderFile.m_Location;
		else if (shaderFile.m_Extension == ".frag" || shaderFile.m_Extension == ".FRAG")
			shaders[shaderFile.m_Name].m_FragmentShaderLocation = shaderFile.m_Location;
		else if (shaderFile.m_Extension == ".geom" || shaderFile.m_Extension == ".GEOM")
			shaders[shaderFile.m_Name].m_GeometryShaderLocation = shaderFile.m_Location;
		else
			allSuccessful = false;
	}

	for (const auto &shader : shaders) {
		bool successful = LoadShaderFromFile(shader.second.m_VertexShaderLocation, shader.second.m_FragmentShaderLocation, shader.second.m_GeometryShaderLocation);

		if (!successful)
			allSuccessful = false;
	}

	return allSuccessful;
}

Shader *ResourceManager::LoadShader(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile) {
	std::string shaderName = FileSystemHelper::GetNameFromFile(p_VertexShaderFile);

	auto iter = m_Shaders.find(shaderName);
	if (iter != m_Shaders.end()) {
		return &iter->second;
	}

	if (LoadShaderFromFile(p_VertexShaderFile, p_FragmentShaderFile, p_GeometryShaderFile)) {
		return &m_Shaders[shaderName];
	}

	return nullptr;
}

Shader *ResourceManager::GetShader(const std::string &p_Name) {
	auto iter = m_Shaders.find(p_Name);
	if (iter != m_Shaders.end()) {
		return &iter->second;
	}

	return nullptr;
}

bool ResourceManager::LoadTexturesFromFolder(const std::string &p_FolderName) {
	std::vector<FileInformation> textureFiles = FileSystemHelper::GetFilesInFolder(p_FolderName);
	FileSystemHelper::RetainRemoveFilesWithExtensions(textureFiles, { ".png", ".PNG", ".jpeg", ".JPEG", ".tiff", ".TIFF", ".bmp", ".BMP", ".hdr", ".HDR", ".tga", ".TGA" });

	bool allSuccessful = true;
	for (auto &textureFile : textureFiles) {
		bool success = LoadTextureFromFile(textureFile.m_Location);

		if (!success)
			allSuccessful = false;
	}

	return allSuccessful;
}

Texture2D *ResourceManager::LoadTexture(const std::string &p_File, GLboolean p_GammaCorrection) {
	std::string textureName = FileSystemHelper::GetNameFromFile(p_File);

	auto iter = m_Textures.find(textureName);
	if (iter != m_Textures.end()) {
		return &iter->second;
	}

	if (LoadTextureFromFile(p_File, p_GammaCorrection)) {
		return &m_Textures[textureName];
	}

	return nullptr;
}

Texture2D *ResourceManager::GetTexture(const std::string &p_Name) {
	auto iter = m_Textures.find(p_Name);
	if (iter != m_Textures.end()) {
		return &iter->second;
	}

	return nullptr;
}

Texture2D * ResourceManager::GetTexture(unsigned int p_TextureID) {
	if (p_TextureID >= 0 && p_TextureID <= m_TextureIDs.size() - 1)
		return m_TextureIDs.at(p_TextureID);

	return nullptr;
}
