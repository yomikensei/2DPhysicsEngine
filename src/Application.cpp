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

	int particleIndex = 0;
	while (particleIndex < NUM_PARTICLES) {
		auto *ball = new Particle(anchor.x, anchor.y + (particleIndex * restLength), 2.0);
		ball->radius = 10;
		particles.push_back(ball);
		particleIndex++;
	}
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
					auto lastParticle = particles[NUM_PARTICLES -1];
					Vec2 impulseDirection = (lastParticle->position - mouseCursor).UnitVector();
					float impulseMagnitude = (lastParticle->position - mouseCursor).Magnitude() * 10.0;
					lastParticle->velocity = impulseDirection * impulseMagnitude;
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

	int particleIndex = 0;
	while (particleIndex < NUM_PARTICLES) {
		auto particle = particles[particleIndex];
		Vec2 weightForce = Vec2(0 * PIXELS_PER_METER, particle->mass * GRAVITY_MAGNITUDE * PIXELS_PER_METER);
		Vec2 frictionForce = Force::GenerateFrictionForce(*particle, 10.0 * PIXELS_PER_METER);
		Vec2 dragForce = Force::GenerateDragForce(*particle, 0.002);

		particle->AddForce(pushForce);
		particle->AddForce(frictionForce);
		particle->AddForce(weightForce);
		particle->AddForce(dragForce);

		Vec2 springForce;
		if (particleIndex == 0) {
			 springForce = Force::GenerateSpringForce(*particle, anchor, restLength, k);
			particle->AddForce(springForce);
		} else {
			springForce = Force::GenerateSpringForce(*particle, *particles[particleIndex-1], restLength, k);
			particle->AddForce(springForce);
			particles[particleIndex-1]->AddForce(-springForce);
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
		particleIndex++;
	}
}

void Application::Render() {
	Graphics::ClearScreen(0xFF056263);
	if (leftMouseButtonDown) {
		auto lastParticle = particles[NUM_PARTICLES-1];
		Graphics::DrawLine(lastParticle->position.x, lastParticle->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
	}
	Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);

	int particleIndex = 0;
	while (particleIndex < NUM_PARTICLES) {
		auto particle = particles[particleIndex];
		if (particleIndex == 0) {
			Graphics::DrawLine(anchor.x, anchor.y, particle->position.x, particle->position.y, 0xFF313131);
		} else {
			auto prevParticle = particles[particleIndex-1];
			Graphics::DrawLine(prevParticle->position.x, prevParticle->position.y, particle->position.x, particle->position.y, 0xFF313131);
		}
		Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
		particleIndex++;
	}
	Graphics::RenderFrame();
}

void Application::Destroy() {
	for (auto particle: particles) {
		delete particle;
	}
	Graphics::CloseWindow();
}