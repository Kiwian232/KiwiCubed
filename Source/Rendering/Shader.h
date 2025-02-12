#pragma once

#include <GLError.h>
#include <glad/glad.h>
#include "Klogger.hpp"
#include <debug-trap.h>

#include <iostream>
#include <future>
#include <string>
#include <sstream>
#include <fstream>
#include <winnt.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Renderer.h"


class Shader
{
	public:
		GLuint shaderProgramID;
		std::string shaderName = "";

		Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath, Renderer& renderer);

		unsigned int UniformTest(const char* uniform) const;
		void SetUniform1ui(const char* uniform, unsigned int value) const;
		void SetUniform3fv(const char* uniform, glm::vec3 value) const;
		void SetUniform4fv(const char* uniform, glm::mat4 value) const;

		void Bind() const;
		void Unbind() const;
		void Delete() const;

	private:
		Renderer& renderer;

		std::string ParseShader(const std::string& filePath);
		int CompileShader(unsigned int type, const std::string& source, const std::string& filePath);
		int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& vertexFilePath, const std::string& fragmentFilePath);
};