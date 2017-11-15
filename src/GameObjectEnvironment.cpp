#include "GameObjectEnvironment.h"
#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "camera.h"



#include "managers.h"

#include <cmath>

extern Scene g_scene;

void GameObjectEnvironment::render() {

	//permet pintar l'interior de la esfera del fons
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);

	Camera* cam = g_scene.camera;
	Vector3 light_pos = g_scene.light_pos;

	
	//pintar el fons al centre de la camera
	model.setTranslation(cam->eye.x, cam->eye.y, cam->eye.z);
	//*** Drawing code starts here ***

	//create our mvp
	Matrix44 mvp = model * cam->viewprojection_matrix;


	//enable shader and set uniforms for normal position
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", mvp);

	//enable shader and set unforms for camera position
	this->shader->setUniform3("u_camera_pos", cam->eye);

	//enable shader and set uniforms for texture for diffuse
	this->shader->setTexture("u_texture_diffuse", this->texture);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->mesh->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);
	
/*
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}*/
}

void GameObjectEnvironment::update(float dt) {

}