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

// 연속성을 깬 '완전 난수' 생성 함수 (펄린 노이즈 대신 사용)
float hash(float seed)
{
    float p = fract(seed * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

void falling()
{
    // 파티클 생성 시작 시간
    float startTime = (a_Rv + 1.0f) / 2.0f;
    float newTime = u_Time - startTime;

    if(newTime > 0.0f)
    {
        // 2.0초 주기로 폭발이 반복되도록 설정 (원하면 1.0f로 변경 가능)
        float lifeTime =1.0;
        float t = lifeTime * fract(newTime/lifeTime);
        float tt = t * t;

        vec4 newPos = vec4(a_Position, 1.0);

        // --- 1. 크기 (Scale) 적용 ---
        // a_Rv2를 이용해 파티클마다 약간씩 다른 크기를 가짐
        float scaleFactor = mix(0.5, 1.5, a_Rv2);
        newPos.xy *= scaleFactor; 

        // --- 2. 폭발 방향 (360도 완전 랜덤) ---
        float randomAngle = hash(a_Rv) * 2.0 * PI;
        vec2 dir = vec2(cos(randomAngle), sin(randomAngle));

        // --- 3. 공기 저항(Friction) 폭발 물리 계산 ---
        float speed = length(a_Velocity);
        if (speed < 0.001) speed = 6.0; // 속도가 안 들어왔을 때를 대비한 기본 폭발력
        
        // 공기 저항 계수 (질량 a_Mass가 클수록 저항을 덜 받아 멀리 날아감)
        float drag = 2.5 / max(a_Mass, 0.1); 
        
        // 저항을 받으며 뻗어나가는 거리 계산 (지수 함수를 이용해 처음엔 빠르고 갈수록 느려짐)
        float frictionDist = (1.0 - exp(-drag * t)) / drag;

        // --- 4. 위치 이동 (Translate) 적용 ---
        newPos.x += dir.x * speed * frictionDist;
        newPos.y += dir.y * speed * frictionDist + 0.5 * G * tt; // 중력은 그대로 적용

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
    falling();
}