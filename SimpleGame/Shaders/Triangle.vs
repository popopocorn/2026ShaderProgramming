#version 330

in vec3 a_Position;
in float a_Mass;
in vec2 a_Velocity;
in float a_Rv;
in float a_Rv2;

uniform float u_Time;
bool u_IsUpward = false;

const float PI = 3.14159265;
const float G = -9.8;

float random(float seed)
{
    return fract(sin(seed * 12.9898) * 43758.5453) * 2.0 * PI;

}

void falling()
{
    float startTime = (a_Rv +1.0f)/2.0f;

    float randomV = random(a_Rv);
    //randomV = (randomV+1.0f) / 2;

    float newTime = u_Time - startTime;
    if( newTime > 0.0f)
    {

        float t = mod(newTime, 1.0f);
    
        //float t = u_Time;
        float tt = t*t;

        vec4 newPos = vec4(a_Position, 1);

        newPos.xy *= a_Rv2;
        newPos.x+= a_Velocity.x * t + sin(randomV * 2 * PI);
        newPos.y+= a_Velocity.y * t + 0.5 * G * tt + cos(randomV * 2 * PI);

        gl_Position = newPos;
    }
    else
    {
        gl_Position.x = -1000;
        gl_Position.y = -1000;
    }
}

void main()
{
    falling();
}