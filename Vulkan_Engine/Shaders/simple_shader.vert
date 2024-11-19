#version 450
#extension GL_KHR_vulkan_glsl : enable

//삼각형의 세점
vec2 positions[3] = vec2[](
	vec2(0.0, -0.5),
	vec2(0.5, 0.5),
	vec2(-0.5, 0.5)
);

void main() {
	//x, y, z, w
	//2차원 이기 때문에 z값은 아직 0으로 고정
	gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
}