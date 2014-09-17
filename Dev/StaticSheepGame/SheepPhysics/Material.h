namespace SheepFizz
{

struct Material
{
	public:
		//default material
		Material();

		//any other material
		Material(float density, float restitution, float staticFriction, float dynamicFriction,
			float momentOfInertia = 0);
		
		//gettors
		float GetMaterialDensity(void) const;
		float GetMaterialResitution(void) const;
		float GetMaterialStaticFriction(void) const;
		float GetMaterialDynamicFriction(void) const;

		//settors
		void SetMaterialDensity(float density);
		void SetMaterialResitution(float restitution);
		void SetMaterialStaticFriction(float staticFriction);
		void SetMaterialDynamicFriction(float dynamicFriction);

	private:
		float density_;
		float restitution_; //bounciness
		float staticFriction_; //static friction is used when two objects are touching at rest
		float dynamicFriction_; //dynamic friction is used when objects are moving past each other

};

}
