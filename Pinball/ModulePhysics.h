#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class SDL_Texture;

class PhysBody
{
private:
	b2Body* body;
	b2Shape* bodyShape;

public:
	PhysBody(b2Body* b);

	const b2Vec2& GetPosition(float offset);
	const double& GetRotation();
	bool Contains(int x, int y) const;

	inline b2Body& GetBody()
	{
		return *body;
	}
	void ApplyForce(b2Vec2 force);
};



class ModulePhysics : public Module 
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int rad, b2BodyType bodyType, uint16 categoryBits, uint16 maskBits);
	PhysBody* CreateBox(int x, int y, int w, int h, float a, b2BodyType bodyType, bool isSensor, uint16 categoryBits, uint16 maskBits);
	PhysBody* CreateChain(int x, int y, int* chainName, const int numPoints, b2BodyType bodyType, uint16 categoryBits, uint16 maskBits);
	
	b2RevoluteJointDef CreateRevoluteJoint(b2Body* b1, b2Body* b2, float max, float min, float anchorX, float anchorY, float initAngle);

	b2World* const GetWorld()const;


private:
	SDL_Texture* bg;
	bool debug;
	b2World* world;

};