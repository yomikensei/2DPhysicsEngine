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

	Particle *a = new Particle(100, 100, 1.0);
	Particle *b = new Particle(300, 100, 1.0);
	Particle *c = new Particle(300, 300, 1.0);
	Particle *d = new Particle(100, 300, 1.0);

	a->radius = 10;
	b->radius = 10;
	c->radius = 10;
	d->radius = 10;

	particles.push_back(a);
	particles.push_back(b);
	particles.push_back(c);
	particles.push_back(d);
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
					auto lastParticle = particles[NUM_PARTICLES - 1];
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

	particles[0]->AddForce(pushForce);

	// Apply forces to the particles
	for (auto particle: particles) {
		// Apply a drag force
		Vec2 drag = Force::GenerateDragForce(*particle, 0.003);
		particle->AddForce(drag);

		// Apply weight force
		Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
		particle->AddForce(weight);
	}

	Vec2 ab = Force::GenerateSpringForce(*particles[0], *particles[1], restLength, k); // a <-> b
	particles[0]->AddForce(ab);
	particles[1]->AddForce(-ab);

	Vec2 bc = Force::GenerateSpringForce(*particles[1], *particles[2], restLength, k); // b <-> c
	particles[1]->AddForce(bc);
	particles[2]->AddForce(-bc);

	Vec2 cd = Force::GenerateSpringForce(*particles[2], *particles[3], restLength, k); // c <-> d
	particles[2]->AddForce(cd);
	particles[3]->AddForce(-cd);

	Vec2 da = Force::GenerateSpringForce(*particles[3], *particles[0], restLength, k); // d <-> a
	particles[3]->AddForce(da);
	particles[0]->AddForce(-da);

	Vec2 ac = Force::GenerateSpringForce(*particles[0], *particles[2], restLength, k); // a <-> c
	particles[0]->AddForce(ac);
	particles[2]->AddForce(-ac);

	Vec2 bd = Force::GenerateSpringForce(*particles[1], *particles[3], restLength, k); // b <-> d
	particles[1]->AddForce(bd);
	particles[3]->AddForce(-bd);

	// Integrate the acceleration and velocity to estimate the new position
	for (auto particle: particles) {
		particle->Integrate(timeDelta);
	}

	for (auto particle: particles) {
		// Nasty hardcoded flip in velocity if it touches the limits of the screen window
		if (particle->position.x - particle->radius <= 0) {
			particle->position.x = particle->radius;
			particle->velocity.x *= -0.9;
		} else if (particle->position.x + particle->radius >= Graphics::Width()) {
			particle->position.x = Graphics::Width() - particle->radius;
			particle->velocity.x *= -0.9;
		}
		if (particle->position.y - particle->radius <= 0) {
			particle->position.y = particle->radius;
			particle->velocity.y *= -0.9;
		} else if (particle->position.y + particle->radius >= Graphics::Height()) {
			particle->position.y = Graphics::Height() - particle->radius;
			particle->velocity.y *= -0.9;
		}
	}
}

void Application::Render() {
	Graphics::ClearScreen(0xFF056263);
	if (leftMouseButtonDown) {
		auto lastParticle = particles[NUM_PARTICLES - 1];
		Graphics::DrawLine(lastParticle->position.x, lastParticle->position.y, mouseCursor.x, mouseCursor.y,
						   0xFF0000FF);
	}

	// Draw all springs
	Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, particles[1]->position.x, particles[1]->position.y, 0xFFFFFFFF);
	Graphics::DrawLine(particles[1]->position.x, particles[1]->position.y, particles[2]->position.x, particles[2]->position.y, 0xFFFFFFFF);
	Graphics::DrawLine(particles[2]->position.x, particles[2]->position.y, particles[3]->position.x, particles[3]->position.y, 0xFFFFFFFF);
	Graphics::DrawLine(particles[3]->position.x, particles[3]->position.y, particles[0]->position.x, particles[0]->position.y, 0xFFFFFFFF);
	Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, particles[2]->position.x, particles[2]->position.y, 0xFFFFFFFF);
	Graphics::DrawLine(particles[1]->position.x, particles[1]->position.y, particles[3]->position.x, particles[3]->position.y, 0xFFFFFFFF);

	// Draw all particles
	Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFFFFFFF);
	Graphics::DrawFillCircle(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFFFFFFFF);
	Graphics::DrawFillCircle(particles[2]->position.x, particles[2]->position.y, particles[2]->radius, 0xFFFFFFFF);
	Graphics::DrawFillCircle(particles[3]->position.x, particles[3]->position.y, particles[3]->radius, 0xFFFFFFFF);


	Graphics::RenderFrame();
}

void Application::Destroy() {
	for (auto particle: particles) {
		delete particle;
	}
	Graphics::CloseWindow();
}