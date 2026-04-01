#version 330


layout(location=0) out vec4 FragColor;

in vec2 UV;

uniform float u_Time;

const float PI = 3.14159265;

void simple()
{
	float split = step(0.5, UV.y);

	vec3 color = mix(vec3(1.0, 0.0, 0.0),
					 vec3(0.0, 0.0, 1.0),
					 split);

	FragColor = vec4(color, 1.0);
}

void linepattern()
{
	float lineCountH = 10;
	float lineCountV = 6;
	float lineWidth = 1;
	lineCountH /= 2;
	lineCountV /= 2;
	lineWidth = 50 / lineWidth;
	
	vec2 sinUV;
	sinUV.x = pow(abs(sin((UV.x * 2 * PI + PI/2) * lineCountV)), lineWidth);
	sinUV.y = pow(abs(sin((UV.y * 2 * PI + PI/2) * lineCountH)), lineWidth);
	FragColor = vec4(sinUV.x + sinUV.y);
}
void Circle()
{

	vec2 center = vec2(0.5, 0.5);
	vec2 curpos = UV.xy;
	float d = distance(center, curpos);
	float lineWidth = 0.01;
	float radius = 0.5;

	if(d > radius || d < radius - lineWidth)
	{
		FragColor = vec4(0);
	}
	else
	{
		FragColor = vec4(UV, 0.0, 1.0);
	}
}
void Circlesin()
{
	vec2 center = vec2(0.5, 0.5);
	vec2 curpos = UV.xy;
	float d = distance(center, curpos);
	
	float split = step(0.5, UV.y);
	float start = 5;
	
	float color = pow(sin(2 * PI * d * 10 - u_Time * start * 10), 1557);
	//float color = pow(sin(2 * PI * d * u_Time), 1557);
	FragColor = vec4(color);
}
void main()
{
	Circlesin();
}