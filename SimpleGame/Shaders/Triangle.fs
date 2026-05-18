#version 330

in float v_Gray;
in vec2 v_Tex;
layout(location=0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
	FragColor = vec4(1);
}
