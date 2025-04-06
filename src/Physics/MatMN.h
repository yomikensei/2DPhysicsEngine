//
// Created by Yomi Ndu on 31/03/2025.
//

#ifndef MATMN_H
#define MATMN_H
#include "VecN.h"


struct MatMN {
	int M;
	int N;
	VecN *rows;

	MatMN();
	MatMN(int M, int N);
	MatMN(const MatMN &m);
	~MatMN();

	void Zero();
	MatMN Transpose() const;

	const MatMN &operator =(const MatMN &m);
	VecN operator *(const VecN &v) const;
	MatMN operator *(const MatMN &m) const;

	static VecN SolveGaussSeidel(const MatMN &A, const VecN &b);
};


#endif //MATMN_H
