#version 450

layout (location = 0) in vec3 fragColor;

layout (location = 0) out vec4 outColor;

void main(){
	//삼각형의 색상
	//Red, Green, Blue, Alpa(투명도)
	outColor = vec4(fragColor, 0.0);
}