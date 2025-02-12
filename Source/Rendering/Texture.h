#pragma once

#include <GLError.h>
#include <Klogger.hpp>

#include"Shader.h"

class Texture {
	public:
		GLuint ID;
		GLenum type;
		GLuint atlasSize;
	
		Texture(const char* filepath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, Renderer& renderer);
	
		void TextureUnit(Shader& shader, const char* uniform, GLuint unit);
		void SetAtlasSize(Shader& shader, GLuint newAtlasSize);
		void Bind() const;
		void Unbind() const;
	
		void Delete() const;

	private:
		Renderer& renderer;
};