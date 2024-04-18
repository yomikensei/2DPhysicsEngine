//
// Created by Yomi Ndu on 31/03/2024.
//

#include "Application.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"

bool Application::IsRunning() {
	return running;
}

void Application::Setup() {
	running = Graphics::OpenWindow();
	auto *smallBall = new Particle(100, 100, 1.0);
	smallBall->radius = 10;
	particles.push_back(smallBall);

	auto *bigBall = new Particle(500, 500, 20.0);
	bigBall->radius = 50;
	particles.push_back(bigBall);

//	liquid.x = 0;
//	liquid.y = Graphics::Height() / 2;
//	liquid.w = Graphics::Width();
//	liquid.h = Graphics::Height() / 2;
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
				if (event.key.keysym.sym == SDLK_UP)
					pushForce.y = -50 * PIXELS_PER_METER;
				if (event.key.keysym.sym == SDLK_RIGHT)
					pushForce.x = 50 * PIXELS_PER_METER;
				if (event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = 50 * PIXELS_PER_METER;
				if (event.key.keysym.sym == SDLK_LEFT)
					pushForce.x = -50 * PIXELS_PER_METER;
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_UP)
					pushForce.y = 0;
				if (event.key.keysym.sym == SDLK_RIGHT)
					pushForce.x = 0;
				if (event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = 0;
				if (event.key.keysym.sym == SDLK_LEFT)
					pushForce.x = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					auto *particle = new Particle(x, y, 1.0);
					particle->radius = 5;
					this->particles.push_back(particle);
				}
				break;
		}
	}
}

void Application::Update() {
	static int timePreviousFrame;
	int waitTime = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
	if (waitTime > 0) {
		SDL_Delay(waitTime);
	}

	float timeDelta = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
	if (timeDelta > 0.016) {
		timeDelta = 0.016;
	}

	timePreviousFrame = SDL_GetTicks();
	Vec2 attraction = Force::GenerateGravitationalForce(*particles[0], *particles[1], 10000000);

	particles[0]->AddForce(attraction);
	particles[1]->AddForce(-attraction);

	for (auto particle: particles) {
//		Vec2 weight = Vec2(0 * PIXELS_PER_METER, particle->mass * GRAVITY_MAGNITUDE * PIXELS_PER_METER);
//		particle->AddForce(weight);
		Vec2 frictionForce = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);

		particle->AddForce(pushForce);
		particle->AddForce(frictionForce);

		// apply a drag force if ball is inside the liquid
		if (particle->position.y >= liquid.y) {
			auto dragForce = Force::GenerateDragForce(*particle, 0.1);
			particle->AddForce(dragForce);
		}

		particle->Integrate(timeDelta);

		// flip velocity vectors if window boundary is hit
		if (particle->position.x - particle->radius <= 0) {
			particle->position.x = particle->radius;
			particle->velocity.x *= -BOUNCE_REVERSAL_RATIO;
		} else if (particle->position.x + particle->radius >= Graphics::Width()) {
			particle->position.x = Graphics::Width() - particle->radius;
			particle->velocity.x *= -BOUNCE_REVERSAL_RATIO;
		}
		if (particle->position.y - particle->radius <= 0) {
			particle->position.y = particle->radius;
			particle->velocity.y *= -BOUNCE_REVERSAL_RATIO;
		} else if (particle->position.y + particle->radius >= Graphics::Height()) {
			particle->position.y = Graphics::Height() - particle->radius;
			particle->velocity.y *= -BOUNCE_REVERSAL_RATIO;
		}
	}
}

void Application::Render() {
	Graphics::ClearScreen(0xFF056263);
	Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF6E3713);
	for (auto particle: particles) {
		Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
	}
	Graphics::RenderFrame();
}

void Application::Destroy() {
	for (auto particle: particles) {
		delete particle;
	}
	Graphics::CloseWindow();
}