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

    //Erin Catto madness**** twice as much as contact count
    //to handle regular velocity and tangent velocity
    for (unsigned int i = 0; i < 4; ++i)
    {
      accumulatedImpulse[i] = 0;
    }

    //accumulated impulse contact bias
    float contactBias = BIASFACTOR * 1 / dt_ * Minimum(0.0f, penetration + POSSLACKBIAS);
    float combinedBias;
    float biasDefault;
    
    //friction
    float impulseTan;
    float previousImpulseTan;

    //end of first part of Catto madness****

    //used to iterate a certain number of times for Catto madness
    for (int iterations = 0; iterations < MAXITERATIONS; ++iterations)
    {
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

        //calculate the combined bias - Erin Catto's method
        combinedBias = inverseMassSum * (-contactVelocity + contactBias);

        //calculate the accumulated impulse for this contact point
        biasDefault = accumulatedImpulse[i];
        accumulatedImpulse[i] = Maximum(biasDefault + combinedBias, 0.0f);
        combinedBias = accumulatedImpulse[i] - biasDefault;

        //calculate impulse scalar : "j"
        float j = (1.0f + mResitution) * combinedBias / inverseMassSum;

        //calculate impulse and apply to bodies
        //A's impulse is negative because the normal is
        //from B to A;
        Vec3D impulse = j * normal;

        if (impulse.x > 100 || impulse.x < -100 || impulse.y > 100 || impulse.y < -100)
          return;

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

        //calculate the normalized tangent vector
        //by removing the relative velocity component along the normal, only
        //the tangent remains (non-normalized)
        Vec3D tangent = relativevelocity - (normal * contactVelocity);
        tangent.Normalize();

        //calculate friction contact velocity - negative to tangentVector
        float frictionContactVelocity = relativevelocity.DotProduct(-tangent);

        //calculate friction scalar
        float jFriction = frictionContactVelocity / inverseMassSum;
        
        impulseTan = mStaticFriction * accumulatedImpulse[i];

        previousImpulseTan = accumulatedImpulse[i + 2];
        accumulatedImpulse[i + 2] = Maximum(previousImpulseTan + jFriction, Minimum(-impulseTan, impulseTan));
        jFriction = accumulatedImpulse[i + 2] - previousImpulseTan;
        
        Vec3D frictionImpulse = jFriction * tangent;

        if (frictionImpulse.x > 30 || frictionImpulse.x < -30 || frictionImpulse.y > 30 || frictionImpulse.y < -30)
          return;

        //apply friction impulse
        A->ApplyImpulse(-frictionImpulse, aRepulsionVec);
        B->ApplyImpulse(frictionImpulse, bRepulsionVec);
      }
    }
  }//end of ApplyForces
}
