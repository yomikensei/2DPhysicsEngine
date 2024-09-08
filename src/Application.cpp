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

    Body* boxA = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    Body* boxB = new Body(BoxShape(200, 200), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);

    // boxA->angularVelocity = 0.4;
    // boxB->angularVelocity = 0.1;


    boxA->rotation = 0.3;
    boxB->rotation = 1.3;

    bodies.push_back(boxA);
    bodies.push_back(boxB);
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
            case SDL_MOUSEMOTION:
                int x, y;
                SDL_GetMouseState(&x, &y);
                bodies[0]->position.x = x;
                bodies[0]->position.y = y;
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

    // for (auto body: bodies) {
    //     Vec2 weight = Vec2(0.0, body->mass * 9.8 * PIXELS_PER_METER);
    //     body->AddForce(weight);
    //
    //     Vec2 wind = Vec2(10.0 * PIXELS_PER_METER, 0.0);
    //     body->AddForce(wind);
    //
    //     float torque = 200;
    //     body->AddTorque(torque);
    // }

    // Integrate the acceleration and velocity to estimate the new position
    for (auto body: bodies) {
        body->Update(timeDelta);
    }

    // Check all the rigid bodies with the other rigid bodies for collision
    for (int i = 0; i <= bodies.size() - 1; i++) {
        for (int j = i + 1; j < bodies.size(); j++) {
            Body* a = bodies[i];
            Body* b = bodies[j];
            a->isColliding = false;
            b->isColliding = false;

            Contact contact;

            if (CollisionDetection::IsColliding(a, b, contact)) {
                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFF00FF);
                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15,
                                   contact.start.y + contact.normal.y * 15, 0xFFFF00FF);
                a->isColliding = true;
                b->isColliding = true;
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
    // Draw all bodies
    for (auto body: bodies) {
        Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;

        if (body->shape->GetType() == CIRCLE) {
            CircleShape *circleShape = (CircleShape *) body->shape;
            Graphics::DrawFillCircle(body->position.x, body->position.y, circleShape->radius, color);
        }
        if (body->shape->GetType() == BOX) {
            BoxShape *boxShape = (BoxShape *) body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);
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

