#version 330

in float v_Color;
in vec2 v_Tex;

const float c_PI = 3.14159265;

uniform sampler2D u_sampler;

layout(location=0) out vec4 FragColor;

mat2 rotate2D(float radian)
{
	return mat2(cos(radian), -sin(radian), sin(radian), cos(radian));
}

void Frag()
{
	vec2 newTex = rotate2D(c_PI/2.0) * v_Tex;
	FragColor = texture(u_sampler, newTex) * v_Color;
}

void main()
{
//	Frag();
	FragColor = vec4(v_Color);
}
