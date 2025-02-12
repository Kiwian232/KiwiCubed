#pragma once

#include <GLError.h>
#include <glad/glad.h>

#include <World.h>

class SingleplayerHandler {
	public:
		World singleplayerWorld;
		Window& window;

		bool isLoadedIntoSingleplayerWorld = false;

		SingleplayerHandler(Window &newWindow, Renderer& renderer) : singleplayerWorld(5, this, renderer), window(newWindow), isLoadedIntoSingleplayerWorld(false) {};

		void Setup();

		void StartSingleplayerWorld();
		void EndSingleplayerWorld();

		void Delete();
};