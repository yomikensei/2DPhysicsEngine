//
// Created by Yomi Ndu on 31/03/2024.
//

#include "Application.h"
#include "Constants.h"

bool Application::IsRunning() {
	return running;
}

void Application::Setup() {
	running = Graphics::OpenWindow();
	particle = new Particle(100, 100, 1.0);
	particle->radius = 25;

	// TODO: setup objects in the scene
}

void Application::Input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				break;
		}
	}
}

void Application::Update() {
	static int timePreviousFrame;
	int winHeight = Graphics::Height();

	// todo: check if we are too fast, and waste time to match the milliseconds per fram
	int waitTime = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
	if (waitTime > 0) {
		SDL_Delay(waitTime);
	}

	float timeDelta = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
	if (timeDelta > 0.016) {
		timeDelta = 0.016;
	}

	timePreviousFrame = SDL_GetTicks();

	particle->acceleration = Vec2(0 * PIXELS_PER_METER, 9.8 * PIXELS_PER_METER);
	particle->velocity += particle->acceleration * timeDelta;
	particle->position += particle->velocity  * timeDelta;

	if (particle -> position.x - particle->radius <= 0) {
		particle->position.x = particle->radius;
		particle->velocity.x *= -0.8;
	} else if (particle -> position.x + particle->radius >= Graphics::Width()) {
		particle->position.x = Graphics::Width() - particle->radius;
		particle->velocity.x *= -0.8;
	}
	if (particle -> position.y - particle->radius <= 0) {
		particle->position.y = particle->radius;
		particle->velocity.y *= -0.8;
	} else if (particle -> position.y + particle->radius >= Graphics::Height()) {
		particle->position.y = Graphics::Height() - particle->radius;
		particle->velocity.y *= -0.8;
	}
}

void Application::Render() {
	Graphics::ClearScreen(0xFF056263);
	Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
	Graphics::RenderFrame();
}

void Application::Destroy() {
	// TODO: destroy all objects in the scene
	delete particle;
	Graphics::CloseWindow();
}