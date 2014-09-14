namespace PHYSICS
{

struct Material
{
	public:
		//default material
		Material(){

			density_ = 0.1f;
			restitution_ = 0.5f;
			staticFriction_ = 0.5f;
			dynamicFriction_ = 0.25f;

		}

		//any other material
		Material(float density, float restitution, float staticFriction, float dynamicFriction,
			float momentOfInertia = 0): 
			density_(density), restitution_(restitution), staticFriction_(staticFriction),
			dynamicFriction_(dynamicFriction) {};
		
		//gettors
		float GetMaterialDensity(void) const { return density_;}
		float GetMaterialResitution(void) const {return restitution_;}
		float GetMaterialStaticFriction(void) const {return staticFriction_;}
		float GetMaterialDynamicFriction(void) const {return dynamicFriction_;}

		//settors
		void SetMaterialDensity(float density){ density_ = density;}
		void SetMaterialResitution(float restitution){ restitution_ = restitution;}
		void SetMaterialStaticFriction(float staticFriction){staticFriction_ = staticFriction;}
		void SetMaterialDynamicFriction(float dynamicFriction){dynamicFriction_ = dynamicFriction;}

	private:
		float density_;
		float restitution_; //bounciness
		float staticFriction_; //static friction is used when two objects are touching at rest
		float dynamicFriction_; //dynamic friction is used when objects are moving past each other

};

}
