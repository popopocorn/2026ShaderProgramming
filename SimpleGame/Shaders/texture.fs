#version 330

uniform sampler2D uTexSampler;
layout(location=0) out vec4 FragColor;

in vec2 UV;

//r - (0~1/3, 0~1)->(0~1, 2/3~1)
//g - (1/3~ 2/3, 0~1)->(0~1, 1/3~2/3)
//b - (2/3~1, 0~1) ->(0~1, 0~1/3)
void texture01()
{
	
	float tx = fract(UV.x*3.0);
	float ty = UV.y/3.0;
	
	float offsetX = 0;
	float offsetY= (2 - floor(UV.x*3.0))/3;

	vec2 newTex = vec2(tx, ty + offsetY);
	FragColor = texture(uTexSampler, newTex);
}

void texture02()
{
	
	float tx = fract(UV.x*3.0);
	float ty = UV.y/3.0;
	
	float offsetX = 0;
	float offsetY= floor(UV.x*3.0)/3;

	vec2 newTex = vec2(tx, ty + offsetY);
	FragColor = texture(uTexSampler, newTex);
}


void main()
{
	texture02();
	
}