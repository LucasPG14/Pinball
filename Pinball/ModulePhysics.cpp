#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleTextures.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	
	b2BodyDef bd;
	
	ground = world->CreateBody(&bd);

	// big static circle as "ground" in the middle of the screen
	int x = 200;
	int y = 800;
	int diameter = 100;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* bigBall = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	bigBall->CreateFixture(&fixture);


	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// On space bar press, create a circle on mouse position
	
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0 && b->IsActive())
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}
		}
	}
	
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && jointFlag == false)
	{
		// test if the current body contains mouse position

		pb = new PhysBody;

		pb = App->player->ballBody;

		int mx = App->input->GetMouseX();
		int my = App->input->GetMouseY();

		if (pb->Contains(mx, my) == true)
		{
			mx = PIXEL_TO_METERS(mx);
			my = PIXEL_TO_METERS(my);

			b2MouseJointDef def;
			def.bodyA = ground;
			def.bodyB = pb->body;
			def.target = b2Vec2(mx, my);
			def.dampingRatio = 0.5f;
			def.frequencyHz = 2.0f;
			def.maxForce = 100.0f * pb->body->GetMass();

			mouseJoint = (b2MouseJoint*)world->CreateJoint(&def);
			jointFlag = true;
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && jointFlag == true)
	{
		b2Vec2 target(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
		mouseJoint->SetTarget(target);

		b2Vec2 anchor = pb->body->GetPosition();
		anchor.x = METERS_TO_PIXELS(anchor.x);
		anchor.y = METERS_TO_PIXELS(anchor.y);

		App->renderer->DrawLine((anchor.x), (anchor.y), (App->input->GetMouseX()), (App->input->GetMouseY()), 255, 0, 0);
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && jointFlag == true)
	{
		world->DestroyJoint(mouseJoint);
		mouseJoint = nullptr;
		jointFlag = false;
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;
	world = nullptr;

	return true;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int rad, b2BodyType bodyType, uint16 categoryBits, uint16 maskBits)
{
	x /= 1.25f;
	y /= 1.25f;
	rad /= 1.25f;

	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(rad);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.5f;
	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;

	b->CreateFixture(&fixture);
	PhysBody* pBody = new PhysBody(b);

	return pBody;
}

PhysBody* ModulePhysics::CreateBox(int x, int y, int w, int h, float a, b2BodyType bodyType, bool isSensor, uint16 categoryBits, uint16 maskBits)
{
	x /= 1.25f;
	y /= 1.25f;
	w /= 1.25f;
	h /= 1.25f;
	a /= 1.25f;

	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = a;

	// Created a getter for the world to call it wherever I want
	b2Body* b = App->physics->GetWorld()->CreateBody(&body);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(PIXEL_TO_METERS(w), PIXEL_TO_METERS(h));

	b2FixtureDef fixture;
	fixture.shape = &boxShape;
	fixture.density = 1.0f;
	if (isSensor) fixture.isSensor = true;
	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;

	b->CreateFixture(&fixture);

	PhysBody* pBody = new PhysBody(b);

	return pBody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* chainName, const int n, b2BodyType bodyType, uint16 categoryBits, uint16 maskBits)
{
	b2Vec2* vs = new b2Vec2[n / 2];
	for (uint i = 0, j = 0; i < n/2 && j < n; ++i, j += 2)
	{
		vs[i] = b2Vec2(PIXEL_TO_METERS(chainName[j] / 1.25f), PIXEL_TO_METERS(chainName[j + 1] / 1.25f));
	}

	b2BodyDef bodyDef;
	bodyDef.type = bodyType;
	bodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = App->physics->GetWorld()->CreateBody(&bodyDef);
	b2ChainShape chainShape;
	chainShape.CreateLoop(vs, n/2);

	b2FixtureDef fixture;
	fixture.shape = &chainShape;
	fixture.density = 1.0f;
	fixture.filter.categoryBits = categoryBits;
	fixture.filter.maskBits = maskBits;

	b->CreateFixture(&fixture);

	PhysBody* pBody = new PhysBody(b);

	delete[] vs;
	return pBody;
}

b2World* const ModulePhysics::GetWorld() const
{
	return world;
}



// -------------------------------------------
// PhysBody Methods

PhysBody::PhysBody(b2Body *b)
{
	body = b;
}

// Return PhysBody's position in pixels.
const b2Vec2& PhysBody::GetPosition(float offset)
{
	b2Vec2 pos = body->GetPosition();
	pos.x = METERS_TO_PIXELS(pos.x + offset);
	pos.y = METERS_TO_PIXELS(pos.y + offset);
	return pos;
}

// Return PhysBody's rotation in degrees.
const double& PhysBody::GetRotation()
{
	return (double)RADTODEG * body->GetTransform().q.GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

void PhysBody::SetBody(b2Body* b)
{
	body = b;
}

void PhysBody::ApplyForce(b2Vec2 force)
{
	b2Vec2 pos = body->GetWorldCenter();
	body->ApplyForce(force, body->GetPosition(), true);
}

b2RevoluteJointDef ModulePhysics::CreateRevoluteJoint(b2Body* b1, b2Body* b2, float max, float min, float anchorX, float anchorY, float initAngle)
{
	b2RevoluteJointDef jointDef;

	jointDef.bodyA = b1;
	jointDef.bodyB = b2;
	jointDef.localAnchorA.Set(anchorX / 1.25f, anchorY / 1.25f);//the corner of the box
	jointDef.localAnchorB.Set(0, 0);//center of the circle
	jointDef.enableLimit = true;
	jointDef.referenceAngle = initAngle * DEGTORAD;
	jointDef.lowerAngle = min * DEGTORAD;
	jointDef.upperAngle = max * DEGTORAD;

	return jointDef;
}