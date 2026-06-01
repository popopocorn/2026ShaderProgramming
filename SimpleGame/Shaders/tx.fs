#version 330

in vec2 v_Tex;

layout(location=0) out vec4 FragColor;

uniform sampler2D u_Tex;

void main()
{
	FragColor = texture(u_Tex, v_Tex);
}
