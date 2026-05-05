#version 330

uniform sampler2D uTexSampler;
uniform sampler2D u_CurNumTex;
layout(location=0) out vec4 FragColor;
uniform float u_Time;

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

void brick()
{
	float resolX = 2; 
	float resolY = 2;
	float shear = 0.5 * u_Time;
	
	float offsetX = fract(ceil(UV.y*resolY)*shear);
	float offsetY = 0;


	float tx = fract(UV.x * resolX + offsetX);
	float ty = fract(UV.y * resolY + offsetY);

	

	FragColor = texture(uTexSampler, vec2(tx, ty));


}

void brick2()
{
	float tx = UV.x * 2;
	float ty = UV.y * 2;

	if(UV.x>0.5)
	{
		ty -= 0.5;
	}

	FragColor = texture(uTexSampler, vec2(tx, ty));


}

void Num()
{
	float tx = UV.x;
	float ty = UV.y;

	
	float offsetX = 0;
	float offsetY = 0;


	FragColor = texture(uTexSampler, vec2(tx, ty));


}

void main()
{
	Num();
	
}