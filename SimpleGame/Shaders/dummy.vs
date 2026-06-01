#version 330

in vec3 a_Position;


uniform float u_Time;
uniform vec4 u_Dropinfo[1000];

out float v_Color;
out vec2 v_Tex;

const float C_PI = 3.14159265;

void flag()
{
	float tx, ty;
	tx = a_Position.x + 0.5;
	ty =1.0 - (a_Position.y + 0.5);
	v_Tex = vec2(tx, ty);

	float value = a_Position.x + 0.5;
	float newX = a_Position.x;
	float newY = a_Position.y * (1.0 -value *0.7) + 
	value*0.25*sin((newX+0.5)*2.0*C_PI-u_Time);


	gl_Position  = vec4(newX, newY, 1.0, 1.0);
	v_Color = value;
}

void circles()
{

	float acu = 0;
	for(int i=0;i<1000;++i)
	{
	
		vec2 center = u_Dropinfo[i].xy;
		vec2 pos = a_Position.xy;
		float lTime = u_Dropinfo[i].z;
		float sTime = u_Dropinfo[i].w;
		float nTime = u_Time - sTime;
		if(nTime>0)
		{
			float t = fract(nTime/lTime) * lTime;
			float oneMinus = 1.0 - fract(nTime/lTime) ;

			float d = distance(center, pos);

			float range = t/5.0;

			float fade= 15 * clamp(range - d, 0, 1.0);

			float sinVal =  pow(abs(sin(d*2*C_PI* 8 - t*2)), 3);

			acu += sinVal * fade * oneMinus;
		}

		
	}
	v_Color = acu;
	gl_Position = vec4(a_Position.x, a_Position.y + acu * 0.05, a_Position.z, 1.0);
}

void main()
{
	circles();

}
