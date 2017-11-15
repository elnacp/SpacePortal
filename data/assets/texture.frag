#version 330

in vec2 v_uv;

out vec4 fragColor;

uniform sampler2D u_texture_diffuse;

void main(void){
	
	vec3 final_color = texture2D(u_texture_diffuse, v_uv).xyz;

	fragColor = vec4(final_color, 1.0);

}
