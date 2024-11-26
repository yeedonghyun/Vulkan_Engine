#version 450

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
	vec2 offset;
	vec3 color;
} push;

void main(){
	//삼각형의 색상
	//Red, Green, Blue, Alpa(투명도)
	outColor = vec4(push.color, 0.0);
}