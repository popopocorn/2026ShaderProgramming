#version 330

in vec3 a_Position;
in float a_Mass;
in vec2 a_Velocity;
in float a_Rv;
in float a_Rv2;
in float a_LifeTime;
in vec2 a_Tex;

out float v_Gray;
out vec2 v_Tex;

uniform float u_Time;

const float PI = 3.14159265;
const float G = -9.8;



void main()
{
	gl_Position=vec4(a_Position, 1.0);
	v_Gray=1.0;
	v_Tex=a_Tex;

}