#version 330

in vec3 a_Position;
in float a_Mass;
in vec2 a_Velocity;
in float a_Rv;
in float a_Rv2;
in float a_LifeTime;

uniform float u_Time;

const float PI = 3.14159265;
const float G = -9.8;




void sin0()
{
    float newTime = u_Time - a_LifeTime;
    float range = (a_Rv +1.0f)/2.0f * 0.2f;
    newTime = mod(newTime, 1.0f);
    if(newTime > 0.0f){
        vec4 newPos = vec4(a_Position, 1.0f);
        newPos.xy *= a_Rv2;
        newPos.x += newTime;
        //끝에서 모이게
        //newPos.y += (range * (1-newTime)) * sin(newTime * a_Rv2 * PI * 2);
        //끝에서 퍼지게
        newPos.y += (range * (1-newTime)) * sin(newTime * a_Rv2 * PI * 2);
        gl_Position = newPos;
    }
    else
    {
        gl_Position = vec4(-1000.0, -1000.0, 0.0, 1.0);
    }
}

float random(float seed)
{
    return fract(sin(seed * 12.9898) * 43758.5453) * 2.0 * PI;
}
void falling()
{
    // 파티클 생성 시작 시간
    float startTime = (a_Rv + 1.0f) / 2.0f;
    float newTime = u_Time - startTime;

    if(newTime > 0.0f)
    {
        float lifeScale = 1.0f;
        float lifeTime = 0.5 + a_LifeTime * lifeScale;
        float t = lifeTime * fract(newTime/lifeTime);
        float tt = t * t;

        vec4 newPos = vec4(a_Position, 1.0);
        newPos.xy *= a_Rv2;
        newPos.x+= a_Velocity.x * t + sin(random(a_Rv2) * 2 * PI);
        newPos.y+= a_Velocity.y * t + 0.5 * G * tt + cos(random(a_Rv2) * 2 * PI);
      
        gl_Position = newPos;
    }
    else
    {
        // 생성 전이거나 화면 밖에 있을 때
        gl_Position = vec4(-1000.0, -1000.0, 0.0, 1.0);
    }
}

void main()
{
    sin0();
}