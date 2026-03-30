#version 330

in float v_Gray;
layout(location=0) out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
	FragColor = vec4(1);
}
