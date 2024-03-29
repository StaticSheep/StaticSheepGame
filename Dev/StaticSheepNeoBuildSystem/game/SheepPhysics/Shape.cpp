/*****************************************************************
Filename: Shape.cpp
Project:
Author(s): Jon Sourbeer (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "precompiled.h"
#include "Shape.h"

namespace SheepFizz
{

	Shape::Shape(): shape_(Count) {}

	//gettors
	//returns the type of the shape - the shape is not stored
	Shapes Shape::GetShape(void) {return shape_;}

	float Shape::GetRadius(void) {return 0;}
	float Shape::GetHeight(void) {return 0;}
	float Shape::GetWidth(void) {return 0;}
	float Shape::GetArea(void) {return area_;}
	float Shape::GetMomentOfInertia(void) {return momentOfInertia_;}

	//settors
	void Shape::SetArea(float area) {area_ = area;}

	void Shape::ChangeShape(float, float) {}

	//primary functions - sets up shape - calculates area
	void Shape::Initialize(void) {}

	//set the moment of inertia after calculation
	void Shape::SetMomentOfInertia(float moment) {momentOfInertia_ = moment;}

	//set the shape
	void Shape::SetShape(Shapes shape) {shape_ = shape;}

	Rectangle::Rectangle(float width, float height):
			width_(width), height_(height){};

	//gettors
	float Rectangle::GetWidth(void) {return width_;}
	float Rectangle::GetHeight(void) {return height_;}
	Vec3D Rectangle::GetVertex(unsigned int i) {return vertices_[i];}
	Vec3D Rectangle::GetNormal(unsigned int i) {return normals_[i];}
  unsigned int Rectangle::GetVertexNumber(void) { return vertexNumber_; }
	
	Shapes Rectangle::GetType(void) {return Rec;}

	//initializes area and vertices/normals used in manifold functions
	void Rectangle::Initialize(void)
	{
		SetArea(width_ * height_);
		
		//calculate partial moment - no density
    float moment = 1.0f / 2.0f * width_ * width_ * width_ * height_;
		SetMomentOfInertia(moment);

		SetShape(Rec);
		
    vertexNumber_ = MAXVERTICES;

		//vertices oriented counterclockwise
		vertices_[0] = Vec3D(width_ / 2, height_ / 2);
		vertices_[1] = Vec3D(-width_ / 2, height_ / 2);
		vertices_[2] = Vec3D(-width_ / 2, -height_ / 2);
		vertices_[3] = Vec3D(width_ / 2, -height_ / 2);

		//calculate side normals - starting with vertices 0 and 1
		for(int i = 0; i < 4; i++)
		{
			//calculate next vertices after this one
			int nextVertice = i + 1 < 4 ? i + 1 : 0;
			
			//subtract two vertices for vector and transform into normal
			normals_[i] = (vertices_[nextVertice] - vertices_[i]).CalculateNormal();

		}
	}

	//set the shape after creation
	void Rectangle::ChangeShape(float x, float y)
	{
		width_ = x;
		height_ = y;
		Initialize();
	}


	Circle::Circle(float radius): radius_(radius) {};

	void Circle::Initialize(void)
	{
		SetArea(radius_ * radius_ * (float)PI);
			
		//partial moment calculation - no density;
		SetMomentOfInertia((float)PI * radius_ * radius_
			* radius_ * 1/3);
		SetShape(Cir);
	}

	//set the shape after creation
	void Circle::ChangeShape(float r, float)
	{
		radius_ = r;
		Initialize();
	}

	//gettors
	float Circle::GetRadius(void) {return radius_;}
	Shapes Circle::GetType(void) {return Cir;}

  Vec3D Polygon::GetVertex(unsigned int i) { return vertices_[i]; }
  Vec3D Polygon::GetNormal(unsigned int i) { return normals_[i]; }
  unsigned int Polygon::GetVertexNumber(void) { return vertexNumber_; }

  Shapes Polygon::GetType(void) { return Rec; }

  //initializes area and vertices/normals used in manifold functions
  void Polygon::Initialize(void)
  {
    SetShape(Poly);
  }

  void Polygon::PolygonFormation(Vec3D* vertices, unsigned int size)
  {
    SetArea(width_ * height_);

    //calculate partial moment - no density
    float moment = 1.0f / 2.0f * width_ * width_ * width_ * height_;
    SetMomentOfInertia(moment);

    SetShape(Poly);

    vertexNumber_ = size;

    //vertices oriented counterclockwise
    for (unsigned int i = 0; i < size; ++i)
      vertices_[i] = vertices[i];

    //calculate side normals - starting with vertices 0 and 1
    for (unsigned int i = 0; i < size; i++)
    {
      //calculate next vertices after this one
      unsigned int nextVertice = i + 1 < size ? i + 1 : 0;

      //subtract two vertices for vector and transform into normal
      normals_[i] = (vertices_[nextVertice] - vertices_[i]).CalculateNormal();

    }

  }

}