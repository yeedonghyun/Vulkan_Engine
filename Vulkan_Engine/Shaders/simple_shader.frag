#version 450

layout (location = 0) in vec3 fragColor;

layout (location = 0) out vec4 outColor;

void main(){
	//�ﰢ���� ����
	//Red, Green, Blue, Alpa(����)
	outColor = vec4(fragColor, 0.0);
}