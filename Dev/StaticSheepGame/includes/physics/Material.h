#pragma once

namespace SheepFizz
{

	class Material
	{
		public:
			//default material
			PHY_API Material();

			//any other material
			PHY_API Material(float density, float restitution, float staticFriction, float dynamicFriction, float momentOfInertia = 0);
		
			//gettors
			PHY_API float GetMaterialDensity(void) const;
			PHY_API float GetMaterialResitution(void) const;
			PHY_API float GetMaterialStaticFriction(void) const;
			PHY_API float GetMaterialDynamicFriction(void) const;

			//settors
			PHY_API void SetMaterialDensity(float density);
			PHY_API void SetMaterialResitution(float restitution);
			PHY_API void SetMaterialStaticFriction(float staticFriction);
			PHY_API void SetMaterialDynamicFriction(float dynamicFriction);

		private:
			float density_;
			float restitution_; //bounciness
			float staticFriction_; //static friction is used when two objects are touching at rest
			float dynamicFriction_; //dynamic friction is used when objects are moving past each other

	};

}

