#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "IndexBufferObject.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Window.h"


struct Vertex {
	GLfloat position[3];
	GLfloat textureCoordinate[2];
	GLuint textureIndex;
};


class Renderer {
	public:
		std::atomic<bool> running = true;
		std::thread RenderThread;

		Renderer(Window& globalWindow) : globalWindow(globalWindow) {}

		void ClearScreen(float redValue, float blueValue, float greenValue);
		void DrawElements(VertexArrayObject& vertexArrayObject, VertexBufferObject& vertexBufferObject, IndexBufferObject& indexBufferObject, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices);

		void QueueRenderCommand(std::function<void()> command);
		
		void RenderLoop();

		void Delete();

	private:
		Window& globalWindow;

		std::queue<std::function<void()>> commandQueue;
		std::mutex QueueMutex;

		unsigned int frames = 0;
};