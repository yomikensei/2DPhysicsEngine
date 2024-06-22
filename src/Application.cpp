//
// Created by Yomi Ndu on 31/03/2024.
//

#include "Application.h"

#include "Physics/Body.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();
    Body *body = new Body(CircleShape(50), Graphics::windowWidth / 2.0, Graphics::windowHeight / 2.0, 1.0);
    bodies.push_back(body);
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
                    auto lastParticle = bodies[0];
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
    //
    // particles[0]->AddForce(pushForce);
    //
    // // Apply forces to the particles
    // for (auto particle: particles) {
    // 	// Apply a drag force
    // 	Vec2 drag = Force::GenerateDragForce(*particle, 0.003);
    // 	particle->AddForce(drag);
    //
    // 	// Apply weight force
    // 	Vec2 weight = Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER);
    // 	particle->AddForce(weight);
    // }
    //
    // Vec2 ab = Force::GenerateSpringForce(*particles[0], *particles[1], restLength, k); // a <-> b
    // particles[0]->AddForce(ab);
    // particles[1]->AddForce(-ab);
    //
    // Vec2 bc = Force::GenerateSpringForce(*particles[1], *particles[2], restLength, k); // b <-> c
    // particles[1]->AddForce(bc);
    // particles[2]->AddForce(-bc);
    //
    // Vec2 cd = Force::GenerateSpringForce(*particles[2], *particles[3], restLength, k); // c <-> d
    // particles[2]->AddForce(cd);
    // particles[3]->AddForce(-cd);
    //
    // Vec2 da = Force::GenerateSpringForce(*particles[3], *particles[0], restLength, k); // d <-> a
    // particles[3]->AddForce(da);
    // particles[0]->AddForce(-da);
    //
    // Vec2 ac = Force::GenerateSpringForce(*particles[0], *particles[2], restLength, k); // a <-> c
    // particles[0]->AddForce(ac);
    // particles[2]->AddForce(-ac);
    //
    // Vec2 bd = Force::GenerateSpringForce(*particles[1], *particles[3], restLength, k); // b <-> d
    // particles[1]->AddForce(bd);
    // particles[3]->AddForce(-bd);
    //
    // // Integrate the acceleration and velocity to estimate the new position
    // for (auto particle: particles) {
    // 	particle->Integrate(timeDelta);
    // }
    //
    for (auto body: bodies) {
        // Nasty hardcoded flip in velocity if it touches the limits of the screen window
        if (body->shape->GetType() == CIRCLE) {
            auto *circleShape = dynamic_cast<CircleShape *>(body->shape);

            if (body->position.x - circleShape->radius <= 0) {
                body->position.x = circleShape->radius;
                body->velocity.x *= -0.9;
            } else if (body->position.x + circleShape->radius >= Graphics::Width()) {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -0.9;
            }
            if (body->position.y - circleShape->radius <= 0) {
                body->position.y = circleShape->radius;
                body->velocity.y *= -0.9;
            } else if (body->position.y + circleShape->radius >= Graphics::Height()) {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -0.9;
            }
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    for (auto body: bodies) {
        if (body->shape->GetType() == CIRCLE) {
            auto circleShape = dynamic_cast<CircleShape *>(body->shape);
            Graphics::DrawFillCircle(body->position.x, body->position.y, circleShape->radius, 0xFFFFFFFF);
        } else {
         // todo: draw other shapes
        }
    }
    Graphics::RenderFrame();
}

void Application::Destroy() {
    for (auto body: bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}
