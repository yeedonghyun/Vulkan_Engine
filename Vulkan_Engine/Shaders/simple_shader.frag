#version 450

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
	vec2 offset;
	vec3 color;
} push;

void main(){
	//�ﰢ���� ����
	//Red, Green, Blue, Alpa(����)
	outColor = vec4(push.color, 0.0);
}