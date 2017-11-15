#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra/textparser.h"

using namespace std;
Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
    indices_vbo_id = 0;
    vao = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
    indices = m.indices;
    vao = m.vao;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &indices_vbo_id);
    if(vao)glDeleteVertexArrays(1, &vao);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
    indices.clear();
}

void Mesh::genBuffers() {
    assert(vertices.size() && "No vertices in this mesh");
    

    //delete old
    if(vertices_vbo_id) glDeleteBuffers(1, &vertices_vbo_id);
    if(normals_vbo_id) glDeleteBuffers(1, &normals_vbo_id);
    if(uvs_vbo_id) glDeleteBuffers(1, &uvs_vbo_id);
    if(colors_vbo_id) glDeleteBuffers(1, &colors_vbo_id);
    
    //bind vao to store state
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers( 1, &vertices_vbo_id ); //generate one handler (id)
    glBindBuffer( GL_ARRAY_BUFFER, vertices_vbo_id ); //bind the handler
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW ); //upload data
    
    //shader must a_vertex attribute, error if not
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_LOCATION);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );

    
    if(normals.size())
    {
        glGenBuffers( 1, &normals_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, normals_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_normal attribute
        glEnableVertexAttribArray(NORMAL_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(NORMAL_ATTRIBUTE_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );

    }
    
    if(uvs.size())
    {
        glGenBuffers( 1, &uvs_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, uvs_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW ); //upload data
        
        //shader must have a_uv attribute
        glEnableVertexAttribArray(UV_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(UV_ATTRIBUTE_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if(colors.size())
    {
        glGenBuffers( 1, &colors_vbo_id); //generate one handler (id)
        glBindBuffer( GL_ARRAY_BUFFER, colors_vbo_id ); //bind the handler
        glBufferData( GL_ARRAY_BUFFER, uvs.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW ); //upload data

        //shader must have a_color attribute
        glEnableVertexAttribArray(COLOR_ATTRIBUTE_LOCATION);
        glVertexAttribPointer(COLOR_ATTRIBUTE_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, NULL );
        assert( glGetError() == GL_NO_ERROR );
        
    }
    
    if (indices.size() == 0){
        //if there is no index buffer, make an 'emergency' serial one here
        for (size_t i = 0; i < vertices.size(); i++) {
            indices.push_back((unsigned int)i);
        }
    }
    
    //create index array
    glGenBuffers(1, &indices_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0); //unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind buffer
    
}

void Mesh::render( int primitive, Shader* sh )
{
    
    glBindVertexArray(vao);
    
    glDrawElements(primitive, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    
}



void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	indices.clear();

	//create four vertices for corner of quad

	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size, 0, size));
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(size,0,-size) );
	

	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );


	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(1,0) );
    
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);


    
    this->genBuffers();
}

//cargem model desde 3DS Max
bool Mesh::loadASE(const char* filename) {

	TextParser t;

	if (t.create(filename) == false) {
		std::cout << "Couldn't open file" << std::endl;
		return false;
	}

	t.seek("*MESH_NUMVERTEX");
	int num_vertices = t.getint();

	t.seek("*MESH_NUMFACES");
	int num_faces = t.getint();
		
	std::cout << num_vertices << std::endl;
	
	//guardem els vertex
	std::vector<Vector3> unique_verts;

	//guardem els vertex de la textura
	//guardem els vertex
	std::vector<Vector2> unique_uvs;

	//guardem les faces
	vertices.resize(num_faces * 3);
	normals.resize(num_faces * 3);
	uvs.resize(num_faces * 3);

	//fem que el vector sigui de la mida de la quantitat de vertex que hi ha al mesh
	unique_verts.resize(num_vertices);
	

	for (int i = 0; i < num_vertices; i++) {
		
		t.seek("*MESH_VERTEX");
		//guardem l'index del vertex
		int new_index = t.getint();
		//guardem vertex position
		float new_x = t.getfloat();
		float new_y = t.getfloat();
		float new_z = t.getfloat();

		//emmagatzamem els vertex en el array unique_verts
		Vector3 new_vec3(-new_x, new_z, new_y);

		unique_verts[new_index] = new_vec3;

		//std::cout << new_index << " " << new_x << " " << new_y << " " << new_z << std::endl;
	}
	

	for (int i = 0; i < num_faces; i++) {

		t.seek("*MESH_FACE");
		//busquem el primer vertex de la cara i la guardem
		t.seek("A:");
		int vert1 = t.getint();

		t.seek("B:");
		int vert2 = t.getint();

		t.seek("C:");
		int vert3 = t.getint();

		vertices[i * 3 + 0] = unique_verts[vert1];
		vertices[i * 3 + 1] = unique_verts[vert2];
		vertices[i * 3 + 2] = unique_verts[vert3];


		//std::cout << vert1 << " " << vert2 << " " << vert3 << std::endl;
	}
	
	t.seek("*MESH_NUMTVERTEX");
	int num_vertuvs = t.getint();

	std::cout << num_vertuvs << std::endl;

	unique_uvs.resize(num_vertuvs);

	for (int i = 0; i < num_vertuvs; i++) {

		t.seek("*MESH_TVERT");
		//guardem l'index del vertex
		int new_index = t.getint();
		//guardem vertex position
		float new_u = t.getfloat();
		float new_v = t.getfloat();

		unique_uvs[new_index].set(new_u, new_v);

		//std::cout << new_index << " " << new_x << " " << new_y << " " << new_z << std::endl;
	}


	t.seek("*MESH_NUMTVFACES");
	int num_uvs = t.getint();

	for (int i = 0; i < num_faces; i++) {
			
			t.seek("*MESH_TFACE");
			int text_index = t.getint();
			float x = t.getfloat();
			float y = t.getfloat();
			float z = t.getfloat();


			uvs[i * 3 + 0] = unique_uvs[x];
			uvs[i * 3 + 1] = unique_uvs[y];
			uvs[i * 3 + 2] = unique_uvs[z];


	}


	for (int i = 0; i < num_faces; i++) {
		for (int j = 0; j < 3; j++){
			t.seek("*MESH_VERTEXNORMAL");
			int normal_index = t.getint();
			float v_nx = t.getfloat();
			float v_ny = t.getfloat();
			float v_nz = t.getfloat();

			Vector3 new_norm(-v_nx, v_nz, v_ny);
			normals[i * 3 + j] = new_norm;
		}
	}


	this->genBuffers();

	return true;
}