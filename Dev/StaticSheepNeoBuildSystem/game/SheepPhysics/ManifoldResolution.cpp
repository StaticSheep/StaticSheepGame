/*****************************************************************
Filename: ManifoldResolution.cpp
Project:
Author(s): Jon Sourbeer (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/

#include "precompiled.h"
#include "Manifold.h"
#include "Vec3D.h"


namespace SheepFizz
{
  //apply forces in the direction of the normals determined in
  //collision functions
  void Manifold::ApplyForces(void)
  {
    //if both objects have infinite mass, skip calculations
    if (A->massData_.mass == 0 && B->massData_.mass == 0)
      return;

    //the force must be applied across all contact points
    for (unsigned int i = 0; i < contactCount; ++i)
    {
      //this is broken into two primary sections, a separation
      //force calculation (impulse) and a friction calculation

      //*******************
      //calculate impulse
      //*******************
      //calculate the relative vectors between the center of mass
      //and each of the contact points
      Vec3D aRepulsionVec = contacts[i] - A->position_;
      Vec3D bRepulsionVec = contacts[i] - B->position_;

      //determine the relative velocity to determine opposite impulse force
      //calculate as if A is stationary and B is moving
      //equation is relveloc = velocity + Cross(angularvelocity, relative vec)
      Vec3D relativevelocity = B->velocity_ +
        (Vec3D(0, 0, B->angularVelocity_) ^ bRepulsionVec)
        - A->velocity_ - (Vec3D(0, 0, A->angularVelocity_) ^ aRepulsionVec);

      //determine the contact velocity - the relative velocity along the 
      //collision normal
      float contactVelocity = relativevelocity.DotProduct(normal);

      //determine the angular vectors - cross the aRepVec with norm
      //will give only z value - equivalent to scalar
      Vec3D aRepulsionCrossNorm = aRepulsionVec ^ normal;
      Vec3D bRepulsionCrossNorm = bRepulsionVec ^ normal;

      //determine the inverse mass sum of the two bodies
      //a combination of the total mass plus the moments of inertia
      //used in calculation of scalar to multiply along collision
      //normal for final calculation of impulse
      float inverseMassSum = A->massData_.inverseMass + B->massData_.inverseMass +
        (aRepulsionCrossNorm.z_ * aRepulsionCrossNorm.z_ * A->massData_.inverseInertia) +
        (bRepulsionCrossNorm.z_ * bRepulsionCrossNorm.z_ * B->massData_.inverseInertia);

      //calculate impulse scalar : "j"
      float j = -(1.0f + (mResitution * 0.5f)) * contactVelocity / inverseMassSum;

      float accumulatedPrev = accumulatedImpulse[i];
      accumulatedImpulse[i] = Maximum(j + accumulatedImpulse[i], 0);
      j = accumulatedImpulse[i] - accumulatedPrev;

      //calculate impulse and apply to bodies
      //A's impulse is negative because the normal is
      //from B to A;
      Vec3D impulse = j * normal;
      
      /*
      if (penetration > .08 && (A->massData_.mass == 0 || B->massData_.mass == 0))
        impulse *= 1 + penetration;

      if (penetration > .10 && (A->massData_.mass == 0 || B->massData_.mass == 0))
        impulse *= 1.5;*/

      impulse.z = 0;

      A->ApplyImpulse(-impulse, aRepulsionVec);
      B->ApplyImpulse(impulse, bRepulsionVec);

      //*******************
      //calculate friction
      //*******************
      //uses the same basic equations as impulse, but replaces normal vector
      //with tangent vector - in the direction closest to the relative velocity

      //because an angular velocity is being applied, must recalculate
      //relative velocity
      relativevelocity = B->velocity_ + (Vec3D(0, 0, B->angularVelocity_) ^ bRepulsionVec)
        - A->velocity_ - (Vec3D(0, 0, A->angularVelocity_) ^ aRepulsionVec);

      contactVelocity = relativevelocity.DotProduct(normal);

      //if(relativevelocity.SquareLength() > 36.0f)
        //return;

      //calculate the normalized tangent vector
      //by removing the relative velocity component along the normal, only
      //the tangent remains (non-normalized)
      Vec3D tangent = (relativevelocity - (normal * contactVelocity));
      tangent.Normalize();

      //calculate friction contact velocity - negative to tangentVector
      float frictionContactVelocity = relativevelocity.DotProduct(-tangent);

      //calculate friction scalar
      float jFriction = frictionContactVelocity / inverseMassSum;

      //friction force cannot exceed normal impulse
      //float accumulatedTanPrev = accumulatedTanImpulse[i];
     // accumulatedTanImpulse[i] = Maximum(jFriction + accumulatedTanImpulse[i], -j * mDynamicFriction);
     // accumulatedTanImpulse[i] = Minimum(accumulatedTanImpulse[i], j * mDynamicFriction);
     // jFriction = accumulatedTanImpulse[i] - accumulatedTanPrev;

      Vec3D frictionImpulse;
      frictionImpulse = jFriction * tangent * A->frictionMod_ * B->frictionMod_;

      //apply friction impulse
      frictionImpulse.z = 0;

      A->ApplyImpulse(-frictionImpulse, aRepulsionVec);
      B->ApplyImpulse(frictionImpulse, bRepulsionVec);
    }

  }//end of ApplyForces


  //positional correction is designed to prevent sinking of one
  //object into another
  void Manifold::PositionalCorrection(void)
  {
    //if both objects have infinite mass, skip calculations
    if (A->massData_.mass == 0 && B->massData_.mass == 0)
      return;

    if (A->massData_.mass > 0 && B->massData_.mass > 0)
      return;

    /*Vec3D correction = (Maximum(penetration - POSSLACK, 0.0f) /
      (A->massData_.inverseMass + B->massData_.inverseMass)) * POSCORRECT
      * normal;

    if (penetration > .06)
      correction *= 2.0f;

    if (penetration > .15)
      correction *= 10.0f;

    correction *= 2.0f;

    correction.z = 0;*/

    Vec3D correction = penetration * normal;

    
    int p = (int)(penetration * 100.0f);

    //float scalar =  PenetrationScaleFunction(p);
    
    if(A->massData_.mass != 0)
    {
      A->position_ -= correction * 0.9f;// * scalar;//A->massData_.inverseMass * correction;

      if(A->angularVelocity_ < 0.5 && A->angularVelocity_ > -0.5f)
        A->angularVelocity_ = 0.0f;
    }

    if(B->massData_.mass != 0)
    {
      B->position_ += correction * 0.9f;// * scalar;//B->massData_.inverseMass * correction;
      if(B->angularVelocity_ < 0.5 && B->angularVelocity_ > -0.5f)
        B->angularVelocity_ = 0.0f;
    }

  }//end of PositionalCorrection

}
