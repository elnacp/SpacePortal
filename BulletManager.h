#pragma once

#include "utils.h"
#include "GameObject.h"
#include "mesh.h"

struct Bullet {
	bool valid; //whether it's alive or not
	Vector3 position; //current position
	Vector3 old_position; //store old pos before updating
	Vector3 velocity; //movement vector
	float ttl; //time-to-live
	float power; //how much damage bullet does
	GameObject* owner; //object that fired bullet
};

class BulletManager {
public:
	Mesh* bullet_mesh; //the mesh to render the bullet
	std::vector<Bullet> bullets_vector; //an array of all bullets
	Shader* shader; //the shader used to draw the bullet 
	//bool can_fire = true; //optional - use this to control fire rate

	BulletManager(Mesh* theMesh, int max_bullets ,Shader* theshader); //constructor

	void render(); //draw all bullets in array
	void update(float elapsed_time); //update position of all bullets
	void createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner);
};