#pragma once



#include "Matrix.h"
#include "ObjectAllocator.h"
#include "Handle.h"
#include "HandleManager.h"

namespace SheepFizz
{

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

	//gettors
	//returns the type of the shape - the shape is not stored


	//primary functions - sets up shape - calculates area

	//set the area after calculation

	//set the moment of inertia after calculation

	//set the shape

	Handle self;

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

		//gettors

		//initializes area and vertices/normals used in manifold functions

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

}
