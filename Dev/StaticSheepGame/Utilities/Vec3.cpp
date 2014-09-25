
#include "Vec3.h"

namespace SheepMath
{

Vec3D& Vec3D::operator+=(Vec3D& rhs)
{
	x_ = x_ + rhs.x_;
	y_ = y_ + rhs.y_;
	z_ = z_ + rhs.z_;
	
	return *this;
}

Vec3D Vec3D::operator+(Vec3D& rhs)
{
	Vec3D vector;
	vector.x_ = x_ + rhs.x_;
	vector.y_ = y_ + rhs.y_;
	vector.z_ = z_ + rhs.z_;

	return vector;
}

Vec3D& Vec3D::operator-=(Vec3D& rhs)
{
	x_ = x_ - rhs.x_;
	y_ = y_ - rhs.y_;
	z_ = z_ - rhs.z_;
	
	return *this;
}

Vec3D Vec3D::operator-(Vec3D& rhs)
{
	Vec3D vector;
	vector.x_ = x_ - rhs.x_;
	vector.y_ = y_ - rhs.y_;
	vector.z_ = z_ - rhs.z_;

	return vector;
}

//negation operator
Vec3D Vec3D::operator-(void)
{
	Vec3D vector;
	vector.x_ = -x_;
	vector.y_ = -y_;
	vector.z_ = -z_;

	return vector;
}

//this operator is used for the crossproduct of two vectors
Vec3D Vec3D::operator*(Vec3D& rhs)
{
	Vec3D crossprod(0,0,0);
	crossprod.x_ = y_ * rhs.z_ - z_ * rhs.y_;
	crossprod.y_ = z_ * rhs.x_ - x_ * rhs.z_;
	crossprod.z_ = x_ * rhs.y_ - y_ * rhs.x_;

	return crossprod;
}

Vec3D Vec3D::CrossProduct(Vec3D& rhs)
{
	Vec3D crossprod(0,0,0);
	crossprod.x_ = y_ * rhs.z_ - z_ * rhs.y_;
	crossprod.y_ = z_ * rhs.x_ - x_ * rhs.z_;
	crossprod.z_ = x_ * rhs.y_ - y_ * rhs.x_;

	return crossprod;
}


Vec3D& Vec3D::operator*=(float scalar)
{
	x_ = x_ * scalar;
	y_ = y_ * scalar;
	z_ = z_ * scalar;
	
	return *this;
}

Vec3D Vec3D::operator*(float scalar)
{
	Vec3D mult(0,0,0);
	mult.x_ = x_ * scalar;
	mult.y_ = y_ * scalar;
	mult.z_ = z_ * scalar;
	
	return mult;
}

Vec3D& Vec3D::operator/=(float scalar)
{
	x_ = x_ / scalar;
	y_ = y_ / scalar;
	z_ = z_ / scalar;

	return *this;
}

Vec3D Vec3D::operator/(float scalar)
{
	Vec3D divide(0,0,0);
	divide.x_ = x_ / scalar;
	divide.y_ = y_ / scalar;
	divide.z_ = z_ / scalar;

	return divide;
}

Vec3D& Vec3D::operator=(Vec3D& rhs)
{
	x_ = rhs.x_;
	y_ = rhs.y_;
	z_ = rhs.z_;

	return *this;
}

float Vec3D::SquareLength(void)
{
	return x_ * x_ + y_ * y_ + z_ * z_;
}

float Vec3D::Length(void)
{
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

float Vec3D::DotProduct(Vec3D& rhs)
{
	return (x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_);
}

Vec3D& Vec3D::Normalize(void)
{
	float length = Length();
	
	//check to see if length is zero
	if(length == 0)
		return *this;

	x_ /= length;
	y_ /= length;
	z_ /= length;

	return *this;
}

Vec3D& Vec3D::Rotate(float radians)
{
	x_ *= cosf(radians);
	y_ *= sinf(radians);

	return *this;
}

Vec3D Vec3D::CalculateNormal(void)
{
	//take base vector (this) and rotate it by 90 degrees
	//clockwise, then normalize it
	return Vec3D(Rotate(-PI / 2)).Normalize();
}

//other non-member vector functions
//**************
Vec3D operator*(float scalar, Vec3D& rhs)
{
	return rhs * scalar;
}

//**************


//additional math functions
//**************
float Minimum(float a, float b)
{
	if(a < b)
		return a;

	return b;
}

float Maximum(float a, float b)
{
	if(a >= b)
		return a;

	return b;
}
//**************

}