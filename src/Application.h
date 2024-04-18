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
		std::vector<Particle*> particles;
		Vec2 pushForce = Vec2(0,0);
		Vec2 mouseCursor = Vec2(0, 0);
		bool leftMouseButtonDown = false;

		Vec2 anchor;
		float k = 100;
		float restLength = 400;

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
