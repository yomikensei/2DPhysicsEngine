//
// Created by Yomi Ndu on 31/03/2025.
//

#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include "Body.h"
#include "MatMN.h"


class Constraint {
public:
	Body *a;
	Body *b;
	Vec2 aPoint;
	Vec2 bPoint;

	virtual ~Constraint() = default;

	MatMN GetInverseMatrix() const;
	VecN GetVelocities() const;

	virtual void PreSolve(float dt) {}
	virtual void Solve() {}
	virtual void PostSolve() {}
};

class JointConstraint : public Constraint {
private:
	MatMN jacobian;
	VecN cachedLambda;
	float bias;

public:
	JointConstraint();
	JointConstraint(Body *a, Body *b, const Vec2 &anchorPoint);
	void PreSolve(float dt) override;
	void Solve() override;
	void PostSolve() override;
};

class PenetrationConstraint : public Constraint {
private:
	MatMN jacobian;
	VecN cachedLambda;
	float bias;
	Vec2 normal;
	float friction;

public:
	PenetrationConstraint();
	PenetrationConstraint(Body *a, Body *b, const Vec2 &aCollisionPoint, const Vec2 &bCollisionPoint,
						  const Vec2 &normal);
	void PreSolve(float dt) override;
	void Solve() override;
	void PostSolve() override;
};


#endif//CONSTRAINT_H
