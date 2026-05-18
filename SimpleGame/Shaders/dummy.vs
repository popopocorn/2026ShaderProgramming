#version 330

in vec3 a_Position;


uniform float u_Time;

out float v_Color;

const float C_PI = 3.14159265;
void main()
{
	
	float newX = a_Position.x;
	float newTime = mod(u_Time, 3.0);
	float offset = sin(2*C_PI * (a_Position.x - newTime))/2.0 * (a_Position.x+0.5) + 0.25;
	float newY = a_Position.y + offset;


	gl_Position  = vec4(newX, newY, 1.0, 1.0);
	v_Color = offset;

}
