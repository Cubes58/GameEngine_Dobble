#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GLCore.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	// The program ID.
	unsigned int m_ID;

	bool CreateShader(GLuint &p_ShaderID, const GLenum &p_ShaderType, const GLchar *p_ShaderSource, const std::string &p_TypeInformation = "Shader");
	bool CheckErrors(GLuint p_Object, const std::string &p_Type);

public:
	// Constructor reads and builds the shader.
	Shader();

	bool Compile(const GLchar *p_VertexPath, const GLchar *p_FragmentPath, const GLchar *p_GeometryPath = nullptr);

	// Use/activate the shader.
	Shader &Use();

	// Utility uniform functions.
	void SetBool(const std::string &p_Name, bool p_bValue) const;
	void SetInt(const std::string &p_Name, int p_iValue) const;
	void SetFloat(const std::string &p_Name, float p_fValue) const;

	void SetVec2(const std::string &p_Name, const glm::vec2 &p_Value) const;
	void SetVec2(const std::string &p_Name, float p_XValue, float p_YValue) const;

	void SetVec3(const std::string &p_Name, const glm::vec3 &p_Value) const;
	void SetVec3(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue) const;

	void SetVec4(const std::string &p_Name, const glm::vec4 &p_Value) const;
	void SetVec4(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue, float p_WValue) const;

	void SetMat2(const std::string &p_Name, const glm::mat2 &p_Mat) const;
	void SetMat3(const std::string &p_Name, const glm::mat3 &p_Mat) const;
	void SetMat4(const std::string &p_Name, const glm::mat4 &p_Mat) const;

	// Accessor methods for the shader ID.
	unsigned int GetID() const;
	void SetID(unsigned int p_ID);
};