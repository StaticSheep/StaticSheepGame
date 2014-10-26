#pragma once
#include "Vec3D.h"
#include "SheepMath.h"

namespace SheepFizz
{
	using namespace Framework;

	class Matrix2D
	{

		public:
		
			//default constructor creates identity matrix
			Matrix2D()
			{
				x0_ = 1;
				x1_ = 0;
				y0_ = 0;
				y1_ = 1;
			}

			//2D matrix
			Matrix2D(float x0, float y0, float x1, float y1): 
				x0_(x0), y0_(y0), x1_(x1), y1_(y1){}

			//rotation matrix
			Matrix2D(float radians)
			{
				x0_ = cosf(radians);
				y0_ = -sinf(radians);
				x1_ = sinf(radians);
				y1_ = cosf(radians);
			}

			//common operators
			Matrix2D& operator=(Matrix2D& rhs);
			Matrix2D operator*(Matrix2D& rhs);
			Matrix2D& operator*=(Matrix2D& rhs);

			Vec3D operator*(Vec3D& rhs);

			//transpose
			Matrix2D Transpose(void)
			{

				Matrix2D transpose;
				transpose.x0_ = x0_;
				transpose.x1_ = y0_;
				transpose.y0_ = x1_;
				transpose.y1_ = y1_;

				return transpose;

			}
	
			float x0_;
			float x1_;
			float y0_;
			float y1_;
	};

}