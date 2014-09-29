#pragma once
#include <cmath>



namespace Framework
{

#define PI 3.1415926539f

class Vec3D{
	
	public:
		Vec3D(float x = 0, float y = 0, float z = 0): x_(x), y_(y), z_(z) { }

		//common operators for vectors
		//Vec3D& operator=(Vec3D& rhs);	//equals
		Vec3D& operator+=(Vec3D& rhs);	//plus-equals
		Vec3D operator+(Vec3D& rhs);	//addition
		Vec3D operator-(Vec3D& rhs);	//subtraction
		Vec3D operator-(void);			//negation
		Vec3D& operator-=(Vec3D& rhs);	//minus-equals

		//multiply a vector by a scalar value
		Vec3D operator*(float scalar);
		Vec3D& operator*=(float scalar);

		//divide by scalars
		Vec3D Vec3D::operator/(float scalar);
		Vec3D& Vec3D::operator/=(float scalar);
	
		//Dot Product
		float DotProduct(Vec3D& rhs);
		
		//both of the following are the crossproducts of two vectors
		Vec3D CrossProduct(Vec3D& rhs);
		Vec3D operator*(Vec3D& rhs);

		//distance calculations for length
		//these equations can be used for any distance between two vecs
		float Length(void);
		float SquareLength(void);

		//Normalize vector
		Vec3D& Normalize(void);

		//Rotate vector by a set number of radians
		Vec3D& Rotate(float radians);

		//Calculate normal to vector
		Vec3D CalculateNormal(void);

	//2D
    union
    {
		  float x_;
      float X;
    };

    union 
    {
      float y_;
      float Y;
    };
		

	//3D - can be used for homogeneous coordinates
    union
    {
      float z_;
      float Z;
    };

};

//additional vector functions
Vec3D operator*(float scalar, Vec3D& rhs);

//other functions
float Minimum(float a, float b);
float Maximum(float a, float b);

}