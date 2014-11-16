
#include "precompiled.h"
#include "Material.h"

namespace SheepFizz
{
	//default material
		Material::Material()
		{

			density_ = 0.1f;
			restitution_ = 0.5f;
			staticFriction_ = 0.5f;
			dynamicFriction_ = 0.25f;

		}

		//any other material
		Material::Material(float density, float restitution, float staticFriction, float dynamicFriction): 
			density_(density), restitution_(restitution), staticFriction_(staticFriction),
			dynamicFriction_(dynamicFriction) {};
		
		//gettors
		float Material::GetMaterialDensity(void) const {return density_;}
		float Material::GetMaterialResitution(void) const {return restitution_;}
		float Material::GetMaterialStaticFriction(void) const {return staticFriction_;}
		float Material::GetMaterialDynamicFriction(void) const {return dynamicFriction_;}

		//settors
		void Material::SetMaterialDensity(float density) {density_ = density;}
		void Material::SetMaterialResitution(float restitution) {restitution_ = restitution;}
		void Material::SetMaterialStaticFriction(float staticFriction) {staticFriction_ = staticFriction;}
		void Material::SetMaterialDynamicFriction(float dynamicFriction) {dynamicFriction_ = dynamicFriction;}

}
