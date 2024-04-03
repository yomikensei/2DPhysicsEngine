//
// Created by Yomi Ndu on 31/03/2024.
//

#ifndef INC_2DPHYSICSENGINE_APPLICATION_H
#define INC_2DPHYSICSENGINE_APPLICATION_H

#include "./Graphics.h"
#include "Physics/Particle.h"

class Application {
	private:
		bool running = false;
		Particle* particle;
		int timePreviousFrame;

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
