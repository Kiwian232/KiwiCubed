#include "Renderer.h"


void Renderer::ClearScreen(float redValue, float blueValue, float greenValue) {
	// Clear them buffers so they are nice and clean for next frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Paint over the whole screen with a fun color
	glClearColor(redValue, blueValue, greenValue, 1.0f);
}

void Renderer::DrawElements(VertexArrayObject& vertexArrayObject, VertexBufferObject& vertexBufferObject, IndexBufferObject& indexBufferObject, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
	std::cout << "queueing" << std::endl;
	QueueRenderCommand([&]() {
		std::cout << "Executing DrawElements" << std::endl;
	
		vertexArrayObject.Bind();
		std::cout << "e" << std::endl;
		vertexBufferObject.Bind();
		vertexBufferObject.SetBufferData(vertices.size() * sizeof(GLfloat), vertices.data());
		indexBufferObject.Bind();
		std::cout << "2" << std::endl;
		indexBufferObject.SetBufferData(indices.size() * sizeof(GLuint), indices.data());
		std::cout << "3" << std::endl;
		vertexArrayObject.LinkAttribute(vertexBufferObject, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		vertexArrayObject.LinkAttribute(vertexBufferObject, 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinate));
		vertexArrayObject.LinkAttribute(vertexBufferObject, 2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureIndex));
		std::cout << "finished buffors" << std::endl;
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
		std::cout << "wow drew" << std::endl;
	});
	std::cout << "queued" << std::endl;
}

void Renderer::QueueRenderCommand(std::function<void()> command) {
	std::cout << "queued" << std::endl;
    std::lock_guard<std::mutex> lock(QueueMutex);
    commandQueue.push(command);
}

void Renderer::RenderLoop() {
	if (glGetString(GL_VERSION) != nullptr) {
		std::cout << "context good" << std::endl;
	} else {
		std::cout << "context bad" << std::endl;
	}
	std::cout << "changing to good" << std::endl;
	glfwMakeContextCurrent(globalWindow.GetWindowInstance());
	if (glGetString(GL_VERSION) != nullptr) {
		std::cout << "context good" << std::endl;
	} else {
		std::cout << "context bad" << std::endl;
	}
    while (running) {
		std::cout << "ran " << frames << " frames" << std::endl;
        {
            std::lock_guard<std::mutex> lock(QueueMutex);
            while (!commandQueue.empty()) {
				if (glGetString(GL_VERSION) != nullptr) {
					std::cout << "context good" << std::endl;
				} else {
					std::cout << "context bad" << std::endl;
				}

				std::cout << "not empty" << std::endl;
                commandQueue.front()();
				std::cout << "set to front" << std::endl;
                commandQueue.pop();
				std::cout << "called func" << std::endl;
            }
        }

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2, 1.0, 0.6, 1.0f);
        glfwSwapBuffers(globalWindow.GetWindowInstance());
		frames++;
    }

    glfwDestroyWindow(globalWindow.GetWindowInstance());
}