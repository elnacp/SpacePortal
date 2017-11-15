#include "utils.h"
#include "GameObject.h"
#include "GameObjectPlayer.h"
#include "mesh.h"
#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "LevelParser.h"
#include "framework.h"

#include "managers.h"

#include "BulletManager.h"

BulletManager::BulletManager(Mesh* theMesh, int max_bullets, Shader* theShader) {
	this->bullet_mesh = theMesh;
	this->bullets_vector.resize(max_bullets);
	this->shader = theShader;

	for (int i = 0; i < bullets_vector.size(); i++) {
		bullets_vector[i].valid = false;
	}
}

void BulletManager::render() {

	for (int i = 0; i < bullets_vector.size(); i++) {
		Bullet curr_bullet = bullets_vector[i];
		if (bullets_vector[i].valid) {

			//*** Drawing code starts here ***
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			//create model matrix for our plane mesh
			//m.setScale(1, 1, 1);
			//m.rotate(1.57, Vector3(1, 0, 0));
			//build a rotation matrix - angle is incremented in update()
			//m.rotate(scene->angle * DEG2RAD, Vector3(0, 1, 0));
			Camera*cam = g_scene.camera;
			Vector3 light_pos = g_scene.light_pos;

			Matrix44 bullmatrix;
			bullmatrix.setTranslation(curr_bullet.position.x,curr_bullet.position.y, curr_bullet.position.z);

			//create our mvp
			Matrix44 mvp = bullmatrix * g_scene.camera->viewprojection_matrix;

			//enable shader and set uniforms for normal position
			this->shader->enable();
			this->shader->setMatrix44("u_mvp", mvp);

			//enable shader and set unforms for camera position
			this->shader->setUniform3("u_camera_pos", cam->eye);

			//enable shader and set uniforms for light 
			this->shader->setUniform3("u_light", light_pos);

			//call render function for mesh, passing shader as param
			//mesh->render() binds the relevant attributes, and drawing code
			this->bullet_mesh->render(GL_TRIANGLES, this->shader);

			//disable the shader
			this->shader->disable();

			//disable blending to draw text on top
			glDisable(GL_BLEND);

			//*** Drawing code ends here ***
		}
	}
}

void BulletManager::update(float seconds_elpased) {

	for (int i = 0; i < bullets_vector.size(); i++) {
		if (bullets_vector[i].valid) {
			//save position in old position
			bullets_vector[i].old_position = bullets_vector[i].position;
			//move each bullet according to velocity
			bullets_vector[i].position = bullets_vector[i].position - Vector3(bullets_vector[i].velocity.x*seconds_elpased, bullets_vector[i].velocity.y*seconds_elpased, bullets_vector[i].velocity.z*seconds_elpased);
			//update ttl of each bullet from dt
			bullets_vector[i].ttl = bullets_vector[i].ttl--;
			//remove bullets where ttl < 0 
			if (bullets_vector[i].ttl < 0) {
				bullets_vector[i].valid = false;
			}
			//test for collisions
		}
	}

}
void BulletManager::createBullet(Vector3 pos, Vector3 vel, float power, float ttl, GameObject* owner) {

	Bullet b;
	
	//uploading data to bullet struct
	b.valid = 1;
	b.position = pos;
	b.velocity = vel;
	b.power = power;
	b.ttl = ttl;
	b.owner = owner;

	for (size_t i = 0; i < bullets_vector.size(); i++) {
		if (!bullets_vector[i].valid) {
			bullets_vector[i] = b;
			break;
		}
	}

}
