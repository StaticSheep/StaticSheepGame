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

	void Shape::ChangeShape(float x, float y) {}

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
	
	Shapes Rectangle::GetType(void) {return Rec;}

	//initializes area and vertices/normals used in manifold functions
	void Rectangle::Initialize(void)
	{
		SetArea(width_ * height_);
		
		//calculate partial moment - no density
		SetMomentOfInertia(1/2 * width_ * width_ * height_);
		
		SetShape(Rec);
		
		//vertices oriented counterclockwise
		vertices_[0] = Vec3D(width_ / 2, height_ / 2);
		vertices_[1] = Vec3D(-width_ / 2, height_ / 2);
		vertices_[2] = Vec3D(-width_ / 2, -height_ / 2);
		vertices_[3] = Vec3D(width_ / 2, -height_ / 2);

		//calculate side normals - starting with vertice 0 and 1
		for(int i = 0; i < 4; i++)
		{
			//calculate next vertice after this one
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
		SetArea(radius_ * radius_ * PI);
			
		//partial moment calculation - no density;
		SetMomentOfInertia(PI * radius_ * radius_
			* radius_ * 1/3);
		SetShape(Cir);
	}

	//set the shape after creation
	void Circle::ChangeShape(float r, float y)
	{
		radius_ = r;
		Initialize();
	}

	//gettors
	float Circle::GetRadius(void) {return radius_;}
	Shapes Circle::GetType(void) {return Cir;}

}