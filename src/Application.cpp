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
	anchor = Vec2(Graphics::Width() / 2, 100);
	auto *ball = new Particle(Graphics::Width() / 2.0, Graphics::Height() / 2.0, 2.0);
	ball->radius = 10;
	particles.push_back(ball);
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
			case SDL_MOUSEMOTION:
				mouseCursor.x = event.motion.x;
				mouseCursor.y = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
					leftMouseButtonDown = true;
					int x, y;
					SDL_GetMouseState(&x, &y);
					mouseCursor.x = x;
					mouseCursor.y = y;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
					leftMouseButtonDown = false;
					Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
					float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
					particles[0]->velocity = impulseDirection * impulseMagnitude;
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

	for (auto particle: particles) {
//		Vec2 weight = Vec2(0 * PIXELS_PER_METER, particle->mass * GRAVITY_MAGNITUDE * PIXELS_PER_METER);
//		particle->AddForce(weight);
		Vec2 frictionForce = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);

		particle->AddForce(pushForce);
		particle->AddForce(frictionForce);

		Vec2 springForce = Force::GenerateSpringForce(*particles[0], anchor, restLength, k);
		particle->AddForce(springForce);

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
	if (leftMouseButtonDown) {
		Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
	}
	Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);
	Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);
	Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFFFFFFF);
	Graphics::RenderFrame();
}

void Application::Destroy() {
	for (auto particle: particles) {
		delete particle;
	}
	Graphics::CloseWindow();
}