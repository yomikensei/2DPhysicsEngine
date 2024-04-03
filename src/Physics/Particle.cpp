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

void Particle::Integrate(float timeDelta) {
	velocity += acceleration * timeDelta;
	position += velocity * timeDelta;
}