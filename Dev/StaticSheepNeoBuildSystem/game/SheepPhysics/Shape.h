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
		Shape();

		//gettors
		//returns the type of the shape - the shape is not stored
		Shapes GetShape(void);

		virtual float GetRadius(void);
		virtual float GetHeight(void);
		virtual float GetWidth(void);
		virtual float GetArea(void);
		virtual float GetMomentOfInertia(void);

		//sets area
		virtual void SetArea(float area);

		//change the shape
		virtual void Shape::ChangeShape(float x, float y);

		//primary functions - sets up shape - calculates area
		virtual void Initialize(void);

		//set the moment of inertia after calculation
		virtual void SetMomentOfInertia(float moment);

		//set the shape
		virtual void SetShape(Shapes shape);

		Handle self;

		private:
			//moment of Inertia - used in inertia calculation
			float momentOfInertia_;
			float area_;	//area for mass calculation

			Shapes shape_;
		
	};

  //create a polygon
  class Polygon : public Shape
  {
    public:
      //constructor
      Polygon(float width, float height);

      //gettors
      //float GetWidth(void);
      //float GetHeight(void);
      Vec3D GetVertex(unsigned int i);
      Vec3D GetNormal(unsigned int i);
      unsigned int GetVertexNumber(void);

      Shapes GetType(void);

      //initializes area and vertices/normals used in manifold functions
      void Initialize(void);
      void PolygonFormation(Vec3D* vertices, unsigned int size);

    private:
      float width_;
      float height_;

      unsigned int vertexNumber_;

      //list of the vertices - at origin
      //vertices are oriented counterclockwise
      Vec3D* vertices_;

      //normals of every side of shape
      Vec3D* normals_;

  };

	//create a rectangle
	class Rectangle: public Shape
	{
		public:
			//constructor
			Rectangle(float width, float height);

			//gettors
			float GetWidth(void);
			float GetHeight(void);
			Vec3D GetVertex(unsigned int i);
			Vec3D GetNormal(unsigned int i);
      unsigned int GetVertexNumber(void);

			Shapes GetType(void);

			//set the shape after creation
			void ChangeShape(float x, float y);

			//initializes area and vertices/normals used in manifold functions
			void Initialize(void);

		private:
			float width_;
			float height_;

      unsigned int vertexNumber_;

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
			Circle(float radius);

			void Initialize(void);

			//set the shape after creation
			void ChangeShape(float r, float y = 0);

			//gettors
			float GetRadius(void);
			Shapes GetType(void);
		
		private:
			float radius_;

	};

}