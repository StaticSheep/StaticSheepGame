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

	//gettors
	//returns the type of the shape - the shape is not stored



	//set the area after calculation

	//set the moment of inertia after calculation

	//set the shape

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

		//gettors


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

		//gettors
		
	private:
		float radius_;

};
