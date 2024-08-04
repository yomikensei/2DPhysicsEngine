//
// Created by Yomi Ndu on 31/03/2024.
//

#include "Application.h"

#include <iostream>
#include <ostream>

#include "Physics/Body.h"
#include "Physics/CollisionDetection.h"
#include "Physics/Constants.h"
#include "Physics/Contact.h"
#include "Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

void Application::Setup() {
    running = Graphics::OpenWindow();

    Body *bigBall = new Body(CircleShape(200), 1000, 800, 0);

    bodies.push_back(bigBall);
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
                Body *smallBall = new Body(CircleShape(40), x, y, 1.0);
                smallBall->restitution = 0.2;
                bodies.push_back(smallBall);
                break;
        }
    }
}

void Application::Update() {
    Graphics::ClearScreen(0xFF056263);

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

        Vec2 wind = Vec2(10.0 * PIXELS_PER_METER, 0.0);
        body->AddForce(wind);

        float torque = 200;
        body->AddTorque(torque);
    }

    // Integrate the acceleration and velocity to estimate the new position
    for (auto body: bodies) {
        body->Update(timeDelta);
        body->isColliding = false;
    }

    // Check all the rigidbodies with the other rigid bodies for collision
    for (int i = 0; i <= bodies.size() - 1; i++) {
        for (int j = i + 1; j < bodies.size(); j++) {
            // TODO: check bodies[i] with bodies[j]
            Body *a = bodies[i];
            Body *b = bodies[j];
            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact)) {
                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * contact.depth,
                                   contact.start.y + contact.normal.y * contact.depth, 0xFFFF00FF);

                a->isColliding = true;
                b->isColliding = true;

                contact.ResolvePenetration();
            }
        }
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
    for (auto body: bodies) {
        Uint32 outlineColor;
        if (body->isColliding)
            outlineColor = 0xFF0000FF;
        else
            outlineColor = 0xFFFFFFFF;

        switch (body->shape->GetType()) {
            case CIRCLE: {
                auto circleShape = dynamic_cast<CircleShape *>(body->shape);
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation,
                                     outlineColor);
                break;
            }
            case BOX: {
                auto boxShape = dynamic_cast<BoxShape *>(body->shape);
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, outlineColor);
                break;
            }
            default: {
                std::cout << "NO SHAPE DRAWER DEFINED FOR SHAPE YET" << std::endl;
            }
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
