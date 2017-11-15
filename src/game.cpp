#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "managers.h"

#include <cmath>

//some globals
Mesh* mesh = NULL;
Texture* textures = new Texture[3];
Texture* texture = NULL;
Shader* shader = NULL;
float angle = 0;
RenderToTexture* rt = NULL;
extern ResourceManager g_resourceManager;

Game* Game::instance = NULL;

Game::Game(SDL_Window* window)
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	std::cout << " * Window size: " << window_width << " x " << window_height << std::endl;

	keystate = NULL;
	mouse_locked = false;
}

//Here we have already GL working, so we can create meshes and textures
void Game::init(void)
{
    std::cout << " * Path: " << getPath() << std::endl;
    
    //SDL_SetWindowSize(window, 50,50);

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0,50,50),Vector3(0,0,0), Vector3(0,1,0)); //position the camera and point to 0,0,0
	camera->setPerspective(70.0f,(float)window_width/(float)window_height,0.1f,10000.0f); //set the projection, we want to be perspective

	//Mesh* ase_mesh = new Mesh();
	//ase_mesh->loadASE("data/assets/box.ASE");

	
	//mesh->createPlane(10);
	//mesh->loadASE("data/assets/Umbreon_normal.ASE");
	//mesh->loadASE("data/assets/x3_fighter.ASE");

	g_resourceManager.loadMesh("x3_fighter", "data/assets/x3_fighter.ASE");

	g_resourceManager.loadTexture("x3_fighter","data/assets/x3_fighter.tga");
	//g_resourceManager.loadTexture("x3_fighter_spec", "data/assets/x3_fighter_spec.tga");
	//texture->load("data/assets/Textures/pm0197_00_Body1.tga");
	//texture->load("data/assets/Textures/pm0197_00_Eye1.tga");
	//texture->load("data/assets/Textures/pm0197_00_Mouth1.tga");


	if( !g_resourceManager.loadShader("x3_fighter","data/shaders/simple.vert","data/shaders/simple.frag") )
	{
		std::cout << "shader not found or error" << std::endl;
		//if your program quits straight after opening, the shader probably hasn't compiled,
		//if you put a breakpoint here you'll be able to read the compilation error in the console
		exit(0);
	}

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//update view and projection matrices, and push gl matrices for fixed pipeline rendering
	camera->set();

	//*** Drawing code starts here ***
   
	//create model matrix for our plane mesh
	Matrix44 m;
    m.setScale(1,1,1);
	m.rotate(1.57, Vector3(1, 0, 0));
	//build a rotation matrix - angle is incremented in update()
	m.rotate(angle * DEG2RAD, Vector3(0,1,0) ); 

	//create our mvp
	Matrix44 mvp = m * camera->viewprojection_matrix;

	//create our normal matrix
	Matrix44 inverse_m = m;
	inverse_m.inverse();
	Matrix44 normal_m = inverse_m.transpose();
	
	//enable shader and set uniforms for normal position
	g_resourceManager.getShader("x3_fighter")->enable();
	g_resourceManager.getShader("x3_fighter")->setMatrix44("u_mvp", mvp );
	g_resourceManager.getShader("x3_fighter")->setMatrix44("u_normal_matrix", normal_m);

	//enable shader and set uniforms for light position
	Vector3 light_pos(00.0f, 100.0f, 00.0f);
	g_resourceManager.getShader("x3_fighter")->setUniform3("u_light", light_pos);

	//enable shader and set unforms for camera position
	g_resourceManager.getShader("x3_fighter")->setUniform3("u_camera_pos", camera->eye);

	//enable shader ans set uniforms for texture
	g_resourceManager.getShader("x3_fighter")->setTexture("u_texture_diffuse", g_resourceManager.getTexture("x3_fighter"));
	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	g_resourceManager.getMesh("x3_fighter")->render(GL_TRIANGLES, shader);
	
	//disable the shader
	g_resourceManager.getShader("x3_fighter")->disable();
	
	//disable blending to draw text on top
    glDisable( GL_BLEND );

	//*** Drawing code ends here ***

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	double speed = seconds_elapsed * 100; //the speed is defined by the seconds_elapsed so it goes constant

	//mouse input to rotate the cam
	if ((mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(mouse_delta.x * 0.005f, Vector3(0,-1,0));
		camera->rotate(mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1,0,0)));
	}

	//async input to move the camera around
	if(keystate[SDL_SCANCODE_LSHIFT]) speed *= 10; //move faster with left shift
	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) camera->move(Vector3(0,0,1) * (float)speed);
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) camera->move(Vector3(0,0,-1) * (float)speed);
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) camera->move(Vector3(1,0,0) * (float)speed);
	if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) camera->move(Vector3(-1,0,0) * (float)speed);
    
	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
	{
		int center_x = (int)floor(window_width*0.5f);
		int center_y = (int)floor(window_height*0.5f);
        //center_x = center_y = 50;
		SDL_WarpMouseInWindow(this->window, center_x, center_y); //put the mouse back in the middle of the screen
		//SDL_WarpMouseGlobal(center_x, center_y); //put the mouse back in the middle of the screen
        
        this->mouse_position.x = (float)center_x;
        this->mouse_position.y = (float)center_y;
	}
    

	angle += (float)seconds_elapsed * 10;
}

//Keyboard event handler (sync input)
void Game::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); //ESC key, kill the app
	}
}


void Game::onMouseButton( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::setWindowSize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
    
	/*
    Uint32 flags = SDL_GetWindowFlags(window);
    if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
    {
        width *= 2;
        height *= 2;
    }
	*/

	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

