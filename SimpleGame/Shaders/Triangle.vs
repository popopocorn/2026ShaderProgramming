#version 330

in vec3 a_Position;
in float a_Mass;
in vec2 a_Velocity;
in float a_Rv;
in float a_Rv2;
in float a_LifeTime;

out float v_Gray;

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
        newPos.y += (range * newTime) * sin(newTime * a_Rv2 * PI * 2);
        gl_Position = newPos;
        v_Gray = 
    }
    else
    {
        gl_Position = vec4(-1000.0, -1000.0, 0.0, 1.0);
    }
}

void sin1()
{
    // 1. 파티클의 실제 생존 시간 계산
    float aliveTime = u_Time - a_LifeTime;

    // 2. 아직 태어나지 않은 파티클은 화면 밖으로 숨김
    if(aliveTime < 0.0)
    {
        gl_Position = vec4(-1000.0, -1000.0, 0.0, 1.0);
        v_Gray = 0.0;
        return;
    }

    // 3. 애니메이션 진행도 (0.0 ~ 1.0 반복)
    float newTime = mod(aliveTime, 1.0); 

    vec4 newPos = vec4(a_Position, 1.0);

    // --------------------------------------------------------------------------
    // 🎯 [효과 1] 끝으로 갈수록 크기가 작아지게 (Fade-out Size)
    // --------------------------------------------------------------------------
    // 초기 스케일 a_Rv2에 애니메이션 진행도 비율(1.0 - newTime)을 곱해 점점 작게 만듭니다.
    // 0.0f까지 완전히 줄이거나, minimal size를 남기려면 mix를 사용하세요 (예: newPos.xy *= mix(a_Rv2, a_Rv2 * 0.2, newTime);).
    float sizeScale = 1.0 - newTime; 
    newPos.xy *= (a_Rv2 * sizeScale); 


    // --------------------------------------------------------------------------
    // 🎯 [효과 2] 사출구와 제트의 분리 (Ejection Stage / Jet Stage)
    // 🎯 [효과 3] 사출 및 제트 활성화의 부드러운 전환 (Smoothstep)
    // --------------------------------------------------------------------------
    // jetActivationTime 비율 전까지는 사출 단계(직선), 그 후부터 제트 단계(물결)입니다.
    float jetActivationTime = 0.15; // 사출 단계를 15% 시간으로 설정 (조절 가능)

    // smoothstep을 이용해 0.0~0.2 전에는 0.0, 0.2~0.4 사이에서 0.0~1.0으로 부드럽게 증가, 이후 1.0 유지.
    float jetMix = smoothstep(0.0, jetActivationTime * 1.5, newTime); 

    float range = a_Rv* 0.2; // 최대 진폭

    // 최종 Y축 사인파 진폭 계산:
    // - 끝으로 갈수록 모이게 하는 기본 비율: (1.0 - newTime)
    // - 사출구에서 분리하는 비율: jetMix (사출 직후에는 0에 가까워 직선, 그 후 커짐)
    float amplitude = range * (1.0 - newTime) * jetMix;


    // --------------------------------------------------------------------------
    // 🎯 [효과 4] 다양한 물결 주파수 (Frequency Variation)
    // --------------------------------------------------------------------------
    // 기존 코드의 sin(newTime * a_Rv2 * PI * 2) -> 파티클마다 출렁임 횟수가 일정했습니다.
    // a_Rv2를 주파수 랜덤 상수로 활용하여 파티클마다 출렁임 횟수(주파수)를 다르게 합니다.
    // PI * 2.0 * (고정바퀴수 + a_Rv2 * 랜덤변화량)
    // 예: (4.0 + a_Rv2 * 2.0) -> 2.0~6.0바퀴 사이로 조절됨. (a_Rv2가 0.0~1.0 가정)
    float frequency = PI * 2.0 * (4.0 + (a_Rv2 + 1.0) / 2.0 * 2.0); // 4~6바퀴 사이

    newPos.y += amplitude * sin(newTime * frequency);


    // X축 방향으로 전진 (사출 속도를 좀 더 빠르게 하려면 newTime에 곱셈 추가)
    newPos.x += newTime; 

    gl_Position = newPos;

    v_Gray = amplitude;

    // (선택사항) Fragment Shader로 투명도를 넘겨 끝에서 부드럽게 사라지게 합니다.
    // v_Alpha = 1.0 - newTime; 
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
    sin1();
}