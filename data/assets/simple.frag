#version 330

in vec3 v_normal;
in vec3 v_camera;
in vec2 v_uv;

uniform vec3 u_light;
uniform sampler2D u_texture_diffuse;
uniform sampler2D u_texture_spec;


out vec4 fragColor;

void main(void)
{
	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light);
	vec3 E = normalize(v_camera);

	vec3 final_color = vec3(0.0f, 0.0f, 0.0f);

	vec3 ambient = vec3(0.1f,0.1f,0.1f);

	float NdotL = dot(N,L);

	if(NdotL > 0.0f){

		//imaginem un objecte blanc
		vec3 diffuse = texture2D(u_texture_diffuse, v_uv).xyz * NdotL;
		//vec3 diffuse = vec3(1.0f,0.0f,1.0f) * NdotL;

		vec3 R = reflect(L,N);
		float RdotE = dot(R,E);
		
		vec3 specular =  vec3(1.0f,1.0f,1.0f) * max(pow(RdotE, 30), 0.0);

		final_color = ambient + diffuse + specular;

		
	}else{
			final_color = ambient;
	}

	// simple red colour
	fragColor =  vec4(final_color,1.0f);
}