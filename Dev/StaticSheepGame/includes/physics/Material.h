namespace SheepFizz
{

struct Material
{
	public:

			density_ = 0.1f;
			restitution_ = 0.5f;
			staticFriction_ = 0.5f;
			dynamicFriction_ = 0.25f;

		}

		//any other material
		Material(float density, float restitution, float staticFriction, float dynamicFriction,
		
		//gettors

		//settors

	private:
		float density_;
		float restitution_; //bounciness
		float staticFriction_; //static friction is used when two objects are touching at rest
		float dynamicFriction_; //dynamic friction is used when objects are moving past each other

};

}

