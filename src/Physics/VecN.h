//
// Created by Yomi Ndu on 31/03/2025.
//

#ifndef VECN_H
#define VECN_H


struct VecN {
	int N;
	float *data;

	VecN();
	VecN(int n);
	VecN(const VecN &v);
	~VecN();

	void Zero() const;
	float Dot(const VecN &v) const;

	VecN &operator =(const VecN &v);
	VecN operator +(const VecN &v) const;
	VecN operator -(const VecN &v) const;
	VecN operator *(float n) const;
	const VecN &operator +=(const VecN &v) const;
	const VecN &operator -=(const VecN &v) const;
	const VecN &operator *=(float n) const;
	float operator [](int index) const;
	float &operator [](int index);
};


#endif //VECN_H
