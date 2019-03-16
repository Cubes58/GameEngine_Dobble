/**
@file Shader.h
@brief A shader class.
*/
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GLCore.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/*! \class Shader
	\brief A class that stores the properties necessary for a shader, and has behaviour to manipulate the shader.
*/

class Shader {
private:
	unsigned int m_ID;	//!< The shader program ID.

	bool CreateShader(GLuint &p_ShaderID, const GLenum &p_ShaderType, const GLchar *p_ShaderSource, const std::string &p_TypeInformation = "Shader");
	bool CheckErrors(GLuint p_Object, const std::string &p_Type);

public:
	Shader();	//!< Constructor.

	/*!
		\brief Compiles the shaders, into a single shader program.
		\param p_VertexPath Relative file path to the vertex shader code. 
		\param p_FragmentPath Relative file path to the fragment shader code. 
		\param p_GeometryPath Relative file path to the geometry shader code. 
		\return Nothing.
	*/
	bool Compile(const GLchar *p_VertexPath, const GLchar *p_FragmentPath, const GLchar *p_GeometryPath = nullptr);

	/*!
		\brief Use/activate the shader.
		\return Returns the shader, for further use.
	*/
	Shader &Use();

	/*!
		\brief Sets a uniform bool in the shader program.
		\param p_Name The name of the uniform.
		\param p_Value The value to set the uniform as.
		\return Nothing.
	*/
	void SetBool(const std::string &p_Name, bool p_Value) const;
	/*!
		\brief Sets a uniform integer in the shader program.
		\param p_Name The name of the uniform.
		\param p_Value The value to set the uniform as.
		\return Nothing.
	*/
	void SetInt(const std::string &p_Name, int p_Value) const;
	/*!
		\brief Sets a uniform float in the shader.
		\param p_Name The name of the uniform.
		\param p_Value The value to set the uniform as.
		\return Nothing.
	*/
	void SetFloat(const std::string &p_Name, float p_Value) const;

	/*!
		\brief Sets a uniform vec2 in the shader.
		\param p_Name The name of the uniform.
		\param p_Value The value to set the uniform as.
		\return Nothing.
	*/
	void SetVec2(const std::string &p_Name, const glm::vec2 &p_Value) const;
	/*!
		\brief Sets a uniform vec2 in the shader.
		\param p_Name The name of the uniform.
		\param p_XValue The X value to set the uniform as.
		\param p_YValue The Y value to set the uniform as.
		\return Nothing.
	*/
	void SetVec2(const std::string &p_Name, float p_XValue, float p_YValue) const;

	/*!
		\brief Sets a uniform vec3 in the shader.
		\param p_Name The name of the uniform.
		\param p_Value The value to set the uniform as.
		\return Nothing.
	*/
	void SetVec3(const std::string &p_Name, const glm::vec3 &p_Value) const;
	/*!
		\brief Sets a uniform vec3 in the shader.
		\param p_Name The name of the uniform.
		\param p_XValue The X value to set the uniform as.
		\param p_YValue The Y value to set the uniform as.
		\param p_ZValue The Z value to set the uniform as.
		\return Nothing.
	*/
	void SetVec3(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue) const;

	/*!
		\brief Sets a uniform vec4 in the shader.
		\param p_Name The name of the uniform.
		\param p_Value The value to set the uniform as.
		\return Nothing.
	*/
	void SetVec4(const std::string &p_Name, const glm::vec4 &p_Value) const;
	/*!
		\brief Sets a uniform vec4 in the shader.
		\param p_Name The name of the uniform.
		\param p_XValue The vec4's X value to set, in the shader's vec4 unifrom.
		\param p_YValue The vec4's Y value to set, in the shader's vec4 unifrom.
		\param p_ZValue The vec4's Z value to set, in the shader's vec4 unifrom.
		\param p_WValue The vec4's W value to set, in the shader's vec4 unifrom.
		\return Nothing.
	*/
	void SetVec4(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue, float p_WValue) const;

	/*!
		\brief Sets a uniform mat2 in the shader.
		\param p_Name The name of the uniform.
		\param p_Mat The mat2's value to set.
		\return Nothing.
	*/
	void SetMat2(const std::string &p_Name, const glm::mat2 &p_Mat) const;
	/*!
		\brief Sets a uniform mat3 in the shader.
		\param p_Name The name of the uniform.
		\param p_Mat The mat3's value to set.
		\return Nothing.
	*/
	void SetMat3(const std::string &p_Name, const glm::mat3 &p_Mat) const;
	/*!
		\brief Sets a uniform mat4 in the shader.
		\param p_Name The name of the uniform.
		\param p_Mat The mat4's value to set.
		\return Nothing.
	*/
	void SetMat4(const std::string &p_Name, const glm::mat4 &p_Mat) const;

	// Accessor methods for the shader ID.
	/*!
		\brief Gets the shader's ID.
		\return Returns the ID of the shader.
	*/
	unsigned int GetID() const;
	/*!
		\brief Sets the shader's ID.
		\param p_ID The shader's new ID.
		\return Nothing.
	*/
	void SetID(unsigned int p_ID);
};