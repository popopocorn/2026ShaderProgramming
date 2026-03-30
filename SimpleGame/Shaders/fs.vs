#version 330

in vec3 a_Position;
in vec2 a_UV;

out vec2 UV;


uniform float u_Time;

void main()
{
	vec4 NewPosition = vec4(a_Position, 1.0);
	gl_Position = NewPosition;
	UV = a_UV;

}