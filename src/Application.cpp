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
    Body *body = new Body(CircleShape(50), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
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
                break;
            case SDL_MOUSEBUTTONDOWN:
                int x, y;
                SDL_GetMouseState(&x, &y);
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

    for (auto body: bodies) {
        Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
        body->AddForce(weight);

        float torque = 60;
        body->AddTorque(torque);
    }

    // Integrate the acceleration and velocity to estimate the new position
    for (auto body: bodies) {
        body->IntegrateLinear(timeDelta);
        body->IntegrateAngular(timeDelta);
    }

    for (auto body: bodies) {
        static float wallCollisionFlipRatio = 0.9;
        // Nasty hardcoded flip in velocity if it touches the limits of the screen window
        if (body->shape->GetType() == CIRCLE) {
            CircleShape *circleShape = dynamic_cast<CircleShape *>(body->shape);

            if (body->position.x - circleShape->radius <= 0) {
                body->position.x = circleShape->radius;
                body->velocity.x *= -wallCollisionFlipRatio;
            } else if (body->position.x + circleShape->radius >= Graphics::Width()) {
                body->position.x = Graphics::Width() - circleShape->radius;
                body->velocity.x *= -wallCollisionFlipRatio;
            }
            if (body->position.y - circleShape->radius <= 0) {
                body->position.y = circleShape->radius;
                body->velocity.y *= -wallCollisionFlipRatio;
            } else if (body->position.y + circleShape->radius >= Graphics::Height()) {
                body->position.y = Graphics::Height() - circleShape->radius;
                body->velocity.y *= -wallCollisionFlipRatio;
            }
        }
    }
}

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    for (auto body: bodies) {
        if (body->shape->GetType() == CIRCLE) {
            auto circleShape = dynamic_cast<CircleShape *>(body->shape);
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFFFFFFFF);
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
