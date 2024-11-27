#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
	mat4 transform;
	vec3 color;
} push;

void main(){
	//삼각형의 색상
	//Red, Green, Blue, Alpa(투명도)
	outColor = vec4(fragColor, 1.0);
}