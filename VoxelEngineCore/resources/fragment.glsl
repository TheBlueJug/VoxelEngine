#version 430

out vec4 fragColor;

void main(){
	vec3 col = vec3(1.0f, 0.5f, 0.2f);
	
	fragColor = vec4(col, 1.0);
}