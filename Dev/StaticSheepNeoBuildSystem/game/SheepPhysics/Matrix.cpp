#include "precompiled.h"
#include "Matrix.h"

namespace SheepFizz
{

Matrix2D& Matrix2D::operator=(Matrix2D& rhs)
{

	x0_ = rhs.x0_;
	x1_ = rhs.x1_;
	y0_ = rhs.y0_;
	y1_ = rhs.y1_;

	return *this;

}//end of operator= (Matrix2D)

Matrix2D& Matrix2D::operator*=(Matrix2D& rhs)
{

	x0_ *= rhs.x0_;
	x1_ *= rhs.y0_;
	y0_ *= rhs.x1_;
	y1_ *= rhs.y1_;

	return *this;
}//end of operator*= (Matrix2D)

Matrix2D Matrix2D::operator*(Matrix2D& rhs)
{

	Matrix2D matrix;
	matrix.x0_ = x0_ * rhs.x0_;
	matrix.x1_ = x1_ * rhs.y0_;
	matrix.y0_ = y0_ * rhs.x1_;
	matrix.y1_ = y1_ * rhs.y1_;

	return matrix;
}//end of operator* (Matrix2D)

Vec3D Matrix2D::operator*(Vec3D& rhs)
{
	Vec3D vector;

	vector.x_ = x0_ * rhs.x_ + y0_ * rhs.y_;
	vector.y_ = x1_ * rhs.x_ + y1_ * rhs.y_;
	vector.z_ = rhs.z_;

	return vector;

}//end of operator* (Vec3D)

}