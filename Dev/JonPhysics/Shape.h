#include "Matrix.h"

#define MAXVERTICES 4

enum Shapes
{
	Rec,
	Cir,
	Poly,
	Count
};

//base class
class Shape
{
	public:
	Shape(): shape_(Count) {}

	//gettors
	//returns the type of the shape - the shape is not stored
	Shapes GetShape(void) {return shape_;}

	virtual float GetRadius(void) {return 0;}
	virtual float GetHeight(void) {return 0;}
	virtual float GetWidth(void) {return 0;}
	virtual float GetArea(void) {return area_;}
	virtual float GetMomentOfInertia(void) {return momentOfInertia_;}

	//primary functions - sets up shape - calculates area
	virtual void Initialize(void) {}

	//set the area after calculation
	virtual void SetArea(float area) {area_ = area;}

	//set the moment of inertia after calculation
	virtual void SetMomentOfInertia(float moment) {momentOfInertia_ = moment;}

	//set the shape
	virtual void SetShape(Shapes shape) {shape_ = shape;}

	private:
		//moment of Inertia - used in inertia calculation
		float momentOfInertia_;
		float area_;	//area for mass calculation

		Shapes shape_;

};

//create a rectangle
class Rectangle: public Shape
{
	public:
		//constructor
		Rectangle(float width, float height):
			width_(width), height_(height){};

		//gettors
		float GetWidth(void) {return width_;}
		float GetHeight(void) {return height_;}
		Shapes GetType(void) {return Rec;}

		//initializes area and vertices/normals used in manifold functions
		void Initialize(void)
		{
			SetArea(width_ * height_);
			
			//calculate partial moment - no density
			SetMomentOfInertia(1/2 * width_ * width_ * height_);
			
			SetShape(Rec);

			//vertices oriented counterclockwise
			vertices_[0] = Vec3D(width_ / 2, height_ / 2);
			vertices_[1] = Vec3D(-width_ / 2, height_ / 2);
			vertices_[2] = Vec3D(-width_ / 2, -height_ / 2);
			vertices_[3] = Vec3D(-width_ / 2, -height_ / 2);

			//calculate side normals - starting with vertice 0 and 1
			for(int i = 0; i < 4; i++)
			{
				//calculate next vertice after this one
				int nextVertice = i + 1 < 4 ? i + 1 : 0;

				//subtract two vertices for vector and transform into normal
				normals_[i] = (vertices_[nextVertice] - vertices_[i]).CalculateNormal();

			}
		}

	private:
		float width_;
		float height_;

		//list of the vertices - at origin
		//vertices are oriented counterclockwise
		Vec3D vertices_[MAXVERTICES];

		//normals of every side of shape
		Vec3D normals_[MAXVERTICES];


};

//create a circle
class Circle: public Shape
{
	public:
		Circle(float radius): radius_(radius) {};

		void Initialize(void)
		{
			SetArea(radius_ * radius_ * PI);
			
			//partial moment calculation - no density;
			SetMomentOfInertia(PI * radius_ * radius_
				* radius_ * 1/3);
			SetShape(Cir);
		}

		//gettors
		float GetRadius(void) {return radius_;}
		Shapes GetType(void) {return Cir;}
		
	private:
		float radius_;

};