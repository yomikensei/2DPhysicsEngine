//
// Created by Yomi Ndu on 21/07/2024.
//

#ifndef CONTACT_H
#define CONTACT_H
#include "Body.h"


struct Contact {
	Body *a;
	Body *b;

	Vec2 start;
	Vec2 end;

	Vec2 normal;
	float depth;
};


#endif//CONTACT_H
