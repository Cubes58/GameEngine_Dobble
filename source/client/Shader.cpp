#include "Shader.h"

#include "Logger.h"

Shader::Shader() : m_ID(999999) {

}

bool Shader::Compile(const GLchar *p_VertexPath, const GLchar *p_FragmentPath, const GLchar *p_GeometryPath) {
	GLuint sVertex;
	GLuint sFragment;
	GLuint gShader;

	// Vertex Shader:
	CreateShader(sVertex, gl::VERTEX_SHADER, p_VertexPath, "VERTEX");
	// Fragment Shader:
	CreateShader(sFragment, gl::FRAGMENT_SHADER, p_FragmentPath, "FRAGMENT");
	// Geometry Shader:
	if (p_GeometryPath != nullptr)
		CreateShader(gShader, gl::GEOMETRY_SHADER, p_GeometryPath, "GEOMETRY");

	// Shader Program:
	m_ID = gl::CreateProgram();
	gl::AttachShader(m_ID, sVertex);
	gl::AttachShader(m_ID, sFragment);
	if (p_GeometryPath != nullptr)
		gl::AttachShader(m_ID, gShader);

	gl::LinkProgram(m_ID);
	bool successful = CheckErrors(m_ID, "PROGRAM");

	// Clean up:
	gl::DeleteShader(sVertex);
	gl::DeleteShader(sFragment);
	if (p_GeometryPath != nullptr)
		gl::DeleteShader(gShader);

	return successful;
}

bool Shader::CreateShader(GLuint &p_ShaderID, const GLenum &p_ShaderType, const GLchar *p_ShaderSource, const std::string &p_TypeInformation) {
	Log(MessageType::INFO) << "\nSHADER SOURCE CODE:" << p_ShaderType << "	" << "SHADER TYPE: " << p_TypeInformation << "\n" << p_ShaderSource;

	p_ShaderID = gl::CreateShader(p_ShaderType);

	gl::ShaderSource(p_ShaderID, 1, &p_ShaderSource, NULL);
	gl::CompileShader(p_ShaderID);
	return CheckErrors(p_ShaderID, p_TypeInformation);
}

bool Shader::CheckErrors(GLuint p_Object, const std::string &p_Type) {
	GLint success;
	GLchar infoLog[1024];
	if (p_Type != "PROGRAM") {
		gl::GetShaderiv(p_Object, gl::COMPILE_STATUS, &success);
		if (!success) {
			gl::GetShaderInfoLog(p_Object, 1024, NULL, infoLog);
			Log(MessageType::FAULT) << "| ERROR::SHADER: Compile-time error: Type: " << p_Type << "\n" << infoLog << "\n -- --------------------------------------------------- -- ";
			return false;
		}
	}
	else {
		gl::GetProgramiv(p_Object, gl::LINK_STATUS, &success);
		if (!success) {
			gl::GetProgramInfoLog(p_Object, 1024, NULL, infoLog);
			Log(MessageType::FAULT) << "| ERROR::SHADER: Compile-time error: Type: " << p_Type << "\n" << infoLog << "\n -- --------------------------------------------------- -- ";
			return false;
		}
	}
	return true;
}

Shader &Shader::Use() {
	gl::UseProgram(m_ID);
	return *this;
}

void Shader::SetBool(const std::string &p_Name, bool p_bValue) const {
	gl::Uniform1i(gl::GetUniformLocation(m_ID, p_Name.c_str()), static_cast<GLint>(p_bValue));
}

void Shader::SetInt(const std::string &p_Name, int p_iValue) const {
	gl::Uniform1i(gl::GetUniformLocation(m_ID, p_Name.c_str()), p_iValue);
}

void Shader::SetFloat(const std::string &p_Name, float p_fValue) const {
	gl::Uniform1f(gl::GetUniformLocation(m_ID, p_Name.c_str()), p_fValue);
}

void Shader::SetVec2(const std::string &p_Name, const glm::vec2 &p_Value) const {
	gl::Uniform2fv(gl::GetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}
void Shader::SetVec2(const std::string &p_Name, float p_XValue, float p_YValue) const {
	gl::Uniform2f(gl::GetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue);
}
// ------------------------------------------------------------------------
void Shader::SetVec3(const std::string &p_Name, const glm::vec3 &p_Value) const {
	gl::Uniform3fv(gl::GetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}
void Shader::SetVec3(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue) const {
	gl::Uniform3f(gl::GetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue, p_ZValue);
}
// ------------------------------------------------------------------------
void Shader::SetVec4(const std::string &p_Name, const glm::vec4 &p_Value) const {
	gl::Uniform4fv(gl::GetUniformLocation(m_ID, p_Name.c_str()), 1, &p_Value[0]);
}
void Shader::SetVec4(const std::string &p_Name, float p_XValue, float p_YValue, float p_ZValue, float p_WValue) const {
	gl::Uniform4f(gl::GetUniformLocation(m_ID, p_Name.c_str()), p_XValue, p_YValue, p_ZValue, p_WValue);
}
// ------------------------------------------------------------------------
void Shader::SetMat2(const std::string &p_Name, const glm::mat2 &p_Mat) const {
	gl::UniformMatrix2fv(gl::GetUniformLocation(m_ID, p_Name.c_str()), 1, gl::FALSE_, &p_Mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat3(const std::string &p_Name, const glm::mat3 &p_Mat) const {
	gl::UniformMatrix3fv(gl::GetUniformLocation(m_ID, p_Name.c_str()), 1, gl::FALSE_, &p_Mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::SetMat4(const std::string &p_Name, const glm::mat4 &p_Mat) const {
	gl::UniformMatrix4fv(gl::GetUniformLocation(m_ID, p_Name.c_str()), 1, gl::FALSE_, &p_Mat[0][0]);
}

unsigned int Shader::GetID() const {
	return m_ID;
}

void Shader::SetID(unsigned int p_ID) {
	m_ID = p_ID;
}