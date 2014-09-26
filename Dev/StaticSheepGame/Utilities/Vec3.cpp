
#include "Vec3.h"

namespace Framework
{

Vec3D& Vec3D::operator+=(Vec3D& rhs)
{
	x_ = x_ + rhs.x_;
	y_ = y_ + rhs.y_;
	z_ = z_ + rhs.z_;
	
	return *this;
}

Vec3D Vec3D::operator+(Vec3D& rhs)
{
	Vec3D vector;
	vector.x_ = x_ + rhs.x_;
	vector.y_ = y_ + rhs.y_;
	vector.z_ = z_ + rhs.z_;

	return vector;
}

Vec3D& Vec3D::operator-=(Vec3D& rhs)
{
	x_ = x_ - rhs.x_;
	y_ = y_ - rhs.y_;
	z_ = z_ - rhs.z_;
	
	return *this;
}

Vec3D Vec3D::operator-(Vec3D& rhs)
{
	Vec3D vector;
	vector.x_ = x_ - rhs.x_;
	vector.y_ = y_ - rhs.y_;
	vector.z_ = z_ - rhs.z_;

	return vector;
}

//negation operator
Vec3D Vec3D::operator-(void)
{
	Vec3D vector;
	vector.x_ = -x_;
	vector.y_ = -y_;
	vector.z_ = -z_;

	return vector;
}

//this operator is used for the crossproduct of two vectors
Vec3D Vec3D::operator*(Vec3D& rhs)
{
	Vec3D crossprod(0,0,0);
	crossprod.x_ = y_ * rhs.z_ - z_ * rhs.y_;
	crossprod.y_ = z_ * rhs.x_ - x_ * rhs.z_;
	crossprod.z_ = x_ * rhs.y_ - y_ * rhs.x_;

	return crossprod;
}

Vec3D Vec3D::CrossProduct(Vec3D& rhs)
{
	Vec3D crossprod(0,0,0);
	crossprod.x_ = y_ * rhs.z_ - z_ * rhs.y_;
	crossprod.y_ = z_ * rhs.x_ - x_ * rhs.z_;
	crossprod.z_ = x_ * rhs.y_ - y_ * rhs.x_;

	return crossprod;
}


Vec3D& Vec3D::operator*=(float scalar)
{
	x_ = x_ * scalar;
	y_ = y_ * scalar;
	z_ = z_ * scalar;
	
	return *this;
}

Vec3D Vec3D::operator*(float scalar)
{
	Vec3D mult(0,0,0);
	mult.x_ = x_ * scalar;
	mult.y_ = y_ * scalar;
	mult.z_ = z_ * scalar;
	
	return mult;
}

Vec3D& Vec3D::operator/=(float scalar)
{
	x_ = x_ / scalar;
	y_ = y_ / scalar;
	z_ = z_ / scalar;

	return *this;
}

Vec3D Vec3D::operator/(float scalar)
{
	Vec3D divide(0,0,0);
	divide.x_ = x_ / scalar;
	divide.y_ = y_ / scalar;
	divide.z_ = z_ / scalar;

	return divide;
}

//Vec3D& Vec3D::operator=(Vec3D& rhs)
//{
//	x_ = rhs.x_;
//	y_ = rhs.y_;
//	z_ = rhs.z_;
//
//	return *this;
//}

float Vec3D::SquareLength(void)
{
	return x_ * x_ + y_ * y_ + z_ * z_;
}

float Vec3D::Length(void)
{
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

float Vec3D::DotProduct(Vec3D& rhs)
{
	return (x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_);
}

Vec3D& Vec3D::Normalize(void)
{
	float length = Length();
	
	//check to see if length is zero
	if(length == 0)
		return *this;

	x_ /= length;
	y_ /= length;
	z_ /= length;

	return *this;
}

Vec3D& Vec3D::Rotate(float radians)
{
	x_ *= cosf(radians);
	y_ *= sinf(radians);

	return *this;
}

Vec3D Vec3D::CalculateNormal(void)
{
	//take base vector (this) and rotate it by 90 degrees
	//clockwise, then normalize it
	return Vec3D(Rotate(-PI / 2)).Normalize();
}

//other non-member vector functions
//**************
Vec3D operator*(float scalar, Vec3D& rhs)
{
	return rhs * scalar;
}

//**************


//additional math functions
//**************
float Minimum(float a, float b)
{
	if(a < b)
		return a;

	return b;
}

float Maximum(float a, float b)
{
	if(a >= b)
		return a;

	return b;
}
//**************


#if DLL_ENGINEEXPORT
void Vec3D::ToLua(lua_State* L, Variable& var)
{
  // Establish a reference to a new set of userdata
  //Variable* ref = (Variable*)lua_newuserdata(L, sizeof(Variable));
  //new (ref) Variable(var); // copies stuff

  //Lua::StackDump(L);

  lua_pushcfunction(L, Lua::ErrorFunc); // Index 1

  //Lua::StackDump(L);

  // Create a new table
  lua_createtable(L, 2, 0); // Index 2

  // Get the meta table
  lua_getglobal(L, "_R"); // index 3
  lua_getfield(L, -1, "__Vector3_MT"); // index 4

  lua_getfield(L, -1, "new"); // Index 5

  // Push arguments
  lua_pushnumber(L, var.GetValue<Vec3D>().x_); // Index 6
  lua_pushnumber(L, var.GetValue<Vec3D>().y_); // Index 7
  lua_pushnumber(L, var.GetValue<Vec3D>().z_); // Index 8

  //Lua::StackDump(L);

  lua_pcall(L, 2, 1, -8);

  //Lua::StackDump(L);

}

void Vec3D::FromLua(lua_State* L, int index, Variable* var)
{
  //Lua::StackDump(L);

  lua_getfield(L, -1, "x");
  var->GetValue<Vec3D>().x_ = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, -1, "y");
  var->GetValue<Vec3D>().y_ = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, -1, "z");
  var->GetValue<Vec3D>().z_ = (float)lua_tonumber(L, -1);
  lua_pop(L, 1);
}

#endif

}