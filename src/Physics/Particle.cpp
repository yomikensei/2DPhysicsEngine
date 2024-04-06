//
// Created by Yomi Ndu on 31/03/2024.
//

#include <iostream>
#include "Particle.h"

Particle::Particle(float x, float y, float mass) {
	this->position = Vec2(x, y);
	this->mass = mass;
	std::cout << "Particle constructor called" << std::endl;
}

Particle::~Particle() {
	std::cout << "Particle destructor called" << std::endl;
}

void Particle::AddForce(const Vec2 &force) {
	sumForces += force;
}

void Particle::ClearForces() {
	sumForces = Vec2(0, 0);
}

void Particle::Integrate(float timeDelta) {
	acceleration = sumForces / mass; // f = ma -> a = f/m
	velocity += acceleration * timeDelta;
	position += velocity * timeDelta;
	ClearForces();
}
