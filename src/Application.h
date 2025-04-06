//
// Created by Yomi Ndu on 31/03/2024.
//

#ifndef INC_2DPHYSICSENGINE_APPLICATION_H
#define INC_2DPHYSICSENGINE_APPLICATION_H

#include "Physics/Body.h"
#include "Physics/World.h"

class Application {
	bool debug = false;
	bool running = false;
	World *world;
	SDL_Texture *backgroundTexture;

public:
	Application() = default;
	~Application() = default;

	bool IsRunning();

	void Setup();
	void Input();
	void Update();
	void Render();
	void Destroy();
};


#endif//INC_2DPHYSICSENGINE_APPLICATION_H
