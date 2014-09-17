#include "Body.h"

namespace SheepFizz
{

void Body::ComputeMass(void){

	//calculate mass based on area - if mass is 0, it is considered infinite
	massData_.mass = shape_->GetArea() * material_.GetMaterialDensity();

	//if mass is 0, set inverse mass to 0 as well
	massData_.inverseMass = massData_.mass ? 1 / massData_.mass : 0.0f;

	//calculate inertia for rotations
	massData_.inertia = shape_->GetMomentOfInertia() * massData_.mass;

	//check if inertia exists
	massData_.inverseInertia = massData_.inertia ? 1 / massData_.inertia : 0.0f;

}//end of ComputeMass

}