#include "Step.h"
#include "Clock.h"
#include "glut.h"
#include <iostream>


#define ESC_KEY 27

TimeStep step( 1.0f / 60.0f);
std::vector<Material> Materials;

Clock g_Clock;
bool frameStepping = false;
bool canStep = false;


void Draw(Body& body)
	{
		const float k_segments = 100;

		 // Render a circle with a bunch of lines
		glBegin(GL_LINE_LOOP);
		float theta = body.orientation_;
		float inc = PI * 2.0f / (float)k_segments;

		//go through every segment of circle and draw it
		for(float i = 0; i < k_segments; ++i)
		{
			theta += inc;
			Vec3D p(std::cosf(theta), std::sinf(theta));
			p *= body.shape_->GetRadius();
			p += body.position_;
			glVertex2f(p.x_,p.y_);
		}
		glEnd();

		//Render line for orientation in Circle
		glBegin(GL_LINE_STRIP);
		Vec3D r(0, 1.0f);
		float c = std::cosf(body.orientation_);
		float s = std::sinf(body.orientation_);
		Vec3D temp;
		temp.x_ = r.x_ * c - r.y_ * s;
		temp.y_ = r.x_ * s + r.y_ * c;
		r.x_ = temp.x_;
		r.y_ = temp.y_;
		r *= body.shape_->GetRadius();
		r = r + body.position_;
		glVertex2f( body.position_.x_, body.position_.y_ );
		glVertex2f( r.x_, r.y_);
		glEnd( );

	}
void Render(void)
{
	//draw all shapes
	for( unsigned int i = 0; i < step.bodies_.size(); ++i)
	{
		Body* body = step.bodies_[i];
		Draw(*(step.bodies_[i]));
	}

	//draw the contact points of the collisions
	glPointSize( 4.0f );
	glBegin( GL_POINTS );
	glColor3f( 1.0f, 0.0f, 0.0f );

	glEnd();

}



inline float Clamp(float a, float min, float max)
{
	//test clamp min
	if(a < min)
		return min;
	//test clamp max
	if(a > max)
		return max;
	//otherwise return
	return a;
}

void Initialize(void)
{

	
	
	Material Wood(.8f, .5f, .8f, .4f);
	Material Iron(1.5f, .8f, .6f, .3f);
	Material Steel(1.3f, .8f, .3f, .15f);
	Material Fluff(.3f, .2f, .4f, .1f);
	Material Bounce(.8f, 2.0f, .3f, .15f);
	Material Static(.0f,.2f,.5f,.3f);

	Materials.push_back(Wood);
	Materials.push_back(Iron);
	Materials.push_back(Steel);
	Materials.push_back(Fluff);
	Materials.push_back(Bounce);
	Materials.push_back(Static);

	step.AddBody(Cir, Static, Vec3D(40.0f,40.0f), 5);
	//step.AddBody(Cir, Wood, Vec3D(0,-20), 5);
}

void Mouse( int button, int state, int x, int y )
{
  x /= 10.0f;
  y /= 10.0f;

  if(state == GLUT_DOWN)
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
      {
        /*PolygonShape poly;
        unsigned int count = (unsigned int)Random( 3, MaxPolyVertexCount );
        Vec2 *vertices = new Vec2[count];
        float e = Random( 5, 10 );
        for(unsigned int i = 0; i < count; ++i)
          vertices[i].Set( Random( -e, e ), Random( -e, e ) );
        poly.Set( vertices, count );
        Body2 *b = scene.Add( &poly, x, y );
        b->SetOrient( Random( -PI, PI ) );
        b->restitution_ = 0.2f;
        b->dynamicFriction_ = 0.2f;
        b->staticFriction_ = 0.4f;
        delete [] vertices;*/

		/*PolygonShape poly;
        unsigned int count = 3;
        Vec2 *vertices = new Vec2[count];
        float e = 5;
        for(unsigned int i = 0; i < count; ++i)
          vertices[i].Set( e , e );
        poly.Set( vertices, count );
        Body2 *b = scene.Add( &poly, x, y );
        b->SetOrient( PI / 2 );
        b->restitution_ = 0.2f;
        b->dynamicFriction_ = 0.2f;
        b->staticFriction_ = 0.4f;
        delete [] vertices;*/
      }
      break;
    case GLUT_RIGHT_BUTTON:
      {
			//Circle c( 5.0f );
			//Body2 *b = scene.Add( &c, 40, 40 );
			step.AddBody(Cir, Materials[0], Vec3D(x,y), 3);
      }
      break;
    }
}

void Keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
  case ESC_KEY:
    exit( 0 );
    break;
  case 's':
    {
      //Circle c( 25.0f );
      //scene.Add( &c, 400 + (rand( ) % 250) * ((rand( ) % 2 == 1) ? 1 : -1), 50 );
      //OBB obb;
      //float e = Random( 10.0f, 35.0f );
      //obb.extents.Set( e, e );
      //Body *b = scene.Add( &obb, 400 + (rand( ) % 250) * ((rand( ) % 2 == 1) ? 1 : -1), 50 );
      //b->SetOrient( PI / 4.0f );
      //b->restitution = 0.2f;
      //b->dynamicFriction = 0.2f;
      //b->staticFriction = 0.4f;
    }
    break;
  case 'd':
    {
      //Circle c( 25.0f );
      //scene.Add( &c, 420, 50 );
    }
      break;

  case 'f':
    frameStepping = frameStepping ? false : true;
      break;
  case ' ':
    canStep = true;
    break;
  }
}

void PhysicsLoop( void )
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  static double accumulator = 0;

  // Different time mechanisms for Linux and Windows
#ifdef WIN32
  accumulator += g_Clock.Elapsed( );
#else
  accumulator += g_Clock.Elapsed( ) / static_cast<double>(std::chrono::duration_cast<clock_freq>(std::chrono::seconds(1)).count());
#endif

  g_Clock.Start( );

  accumulator = Clamp( 0.0f, 0.1f, accumulator);
  while(accumulator >= 1.0f / 60.0f)
  {
    if(!frameStepping)
      step.Step( );
    else
    {
      if(canStep)
      {
        step.Step( );
        canStep = false;
      }
    }
    accumulator -= 1.0f / 60.0f;
  }

  g_Clock.Stop( );

  Render( );

  glutSwapBuffers( );
}


int main(int argc, char** argv)
{

	Initialize();
	
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize( 800, 600 );
	glutCreateWindow( "PhyEngine" );
	glutDisplayFunc( PhysicsLoop );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( Mouse );
	glutIdleFunc( PhysicsLoop );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );
	gluOrtho2D( 0, 80, 60, 0 );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );


	srand( 1 );
	glutMainLoop( );
	
	//Vector test



	//initialize materials
	



	
	return 0;
}