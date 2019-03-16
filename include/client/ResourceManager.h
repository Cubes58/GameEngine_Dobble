/**
@file ResourceManager.h
@brief Manages the games shaders and textures.
*/
#pragma once

#include <map>
#include <vector>
#include <string>

#include "GLCore.hpp"

#include "Texture2D.h"
#include "Shader.h"

class RenderSystem;

#define ResourceManagerInstance ResourceManager::Instance()	//!< Direct access to the ResourceManager singleton instance.

/*! \class ResourceManager
	\brief Manages the games shaders and textures.
*/

class ResourceManager {
private:
	/*!
		* @brief The Render System class can access all of the ResourceManager's properties.
		* @relates RenderSystem
	*/
	friend RenderSystem;

	std::map<std::string, Texture2D> m_Textures;	//!< A map of names identifying textures, that're stored.
	std::map<std::string, Shader> m_Shaders;	//!< A map of names identifying shaders, that're stored.
	std::vector<Texture2D*> m_TextureIDs;	//!< A vector of 2D texture pointers, pointing to the map of textures that're stored.

	ResourceManager();	//!< Constructor.
	~ResourceManager();	//!< Destructor.

	/*!
		\brief Loads a shader from files.
		\param p_VertexShaderFile Relative path to the vertex shader file.
		\param p_FragmentShaderFile Relative path to the fragment shader file.
		\param p_GeometryShaderFile Relative path to the geometry shader file.
		\return Returns true if the shader was successfully loaded, otherwise false.
	*/
	bool LoadShaderFromFile(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile = " ");
	/*!
		\brief Loads a texture from a file.
		\param p_File Relative path to the image file.
		\param p_GammaCorrection True if the image is pre gamma corrected, otherwise false.
		\return Returns true if the image was successfully loaded, otherwise false.
	*/
	bool LoadTextureFromFile(const std::string &p_File, GLboolean p_GammaCorrection = false);

public:
	/*!
		\brief Gets the ResourceManager instance.
		\return Returns the Resource Manager instance.
	*/
	static ResourceManager &Instance() {
		static ResourceManager s_ResourceManager;

		return s_ResourceManager;
	}

	/*!
		\brief Loads shaders from a folder.
		\param p_FolderPath The relative path to the folder of shaders.
		\return Returns true if all of the shaders were successfully loaded, otherwise false.
	*/
	bool LoadShadersFromFolder(const std::string &p_FolderPath);
	/*!
		\brief Loads a shader from files.
		\param p_VertexShaderFile Relative path to the vertex shader file.
		\param p_FragmentShaderFile Relative path to the fragment shader file.
		\param p_GeometryShaderFile Relative path to the geometry shader file.
		\return Returns a pointer to the loaded shader instance.
	*/
	Shader *LoadShader(const std::string &p_VertexShaderFile, const std::string &p_FragmentShaderFile, const std::string &p_GeometryShaderFile = " ");
	/*!
		\brief Gets a pointer to the shader instance, being requested.
		\param p_Name The name of the shader.
		\return Returns a pointer to the shader instance, being requested, if found, otherwise returns a nullptr.
	*/
	Shader *GetShader(const std::string &p_Name);

	/*!
		\brief Loads textures from a folder.
		\param p_FolderPath The relative path to the folder.
		\return Returns true if all of the textures were successfully loaded, otherwise false.
	*/
	bool LoadTexturesFromFolder(const std::string &p_FolderPath);
	/*!
		\brief Loads s texture from a image file.
		\param p_File The relative path to the image file.
		\param p_GammaCorrection True if the image has been gamma corrected, otherwise false.
		\return Returns a pointer to the texture instance, that was loaded.
	*/
	Texture2D *LoadTexture(const std::string &p_File, GLboolean p_GammaCorrection = false);
	/*!
		\brief Gets a pointer to the texture instance, being requested,.
		\param p_Name The name of the texture being requested.
		\return Returns a pointer to the texture instance, being requested, if it's found, otherwise it returns a nullptr.
	*/
	Texture2D *GetTexture(const std::string &p_Name);
	/*!
		\brief Gets a pointer to the texture instance, being requested,.
		\param p_TextureID The texture ID, of the texture being requested.
		\return Returns a pointer to the texture instance, being requested, if it's found, otherwise it returns a nullptr.
	*/
	Texture2D *GetTexture(unsigned int p_TextureID);
};