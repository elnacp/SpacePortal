#include "GameObjectMeteor.h"
#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "GameObjectMesh.h"
#include "GameObjectEnvironment.h"
#include "LevelParser.h"
#include "framework.h"

#include "managers.h"


#include <cmath>

void GameObjectMeteor::render() {

	//*** Drawing code starts here ***

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//create model matrix for our plane mesh
	//m.setScale(1, 1, 1);
	//m.rotate(1.57, Vector3(1, 0, 0));
	//build a rotation matrix - angle is incremented in update()
	//m.rotate(scene->angle * DEG2RAD, Vector3(0, 1, 0));
	model.setTranslation(100, 0, 0);
	Camera*cam = g_scene.camera;
	Vector3 light_pos = g_scene.light_pos;

	//create our mvp
	Matrix44 mvp = this->getGlobalMatrix() * cam->viewprojection_matrix;

	//create our normal matrix
	Matrix44 inverse_m = this->model;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();


	//enable shader and set uniforms for normal position
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", mvp);
	this->shader->setMatrix44("u_normal_matrix", normal_m);

	//enable shader and set unforms for camera position
	this->shader->setUniform3("u_camera_pos", cam->eye);

	//enable shader and set uniforms for light 
	this->shader->setUniform3("u_light", light_pos);

	//enable shader and set uniforms for texture for diffuse
	this->shader->setTexture("u_texture_diffuse", this->texture);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	this->mesh->render(GL_TRIANGLES, this->shader);

	//disable the shader
	this->shader->disable();

	//disable blending to draw text on top
	glDisable(GL_BLEND);


	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}

	//*** Drawing code ends here ***

}

void GameObjectMeteor::update(float dt) {
}