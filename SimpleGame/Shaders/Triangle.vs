#version 330

in vec3 a_Position;

uniform float u_Time;
bool u_IsUpward = false;

// 1. 가짜 난수 생성기 (Pseudo-Random)
float hash(float n) {
    // 사인 함수의 결과를 크게 증폭시킨 뒤 소수점만 가져와 난수처럼 보이게 합니다.
    return fract(sin(n) * 43758.5453123);
}

// 2. 1D 노이즈 함수 (부드러운 주식 변동성 생성)
float noise(float x) {
    float i = floor(x); // 정수부
    float f = fract(x); // 소수부
    
    // 부드러운 곡선을 위한 보간 공식 (Smoothstep)
    float u = f * f * (3.0 - 2.0 * f);
    
    // 이전 정수점과 다음 정수점의 난수를 부드럽게 이어줍니다.
    return mix(hash(i), hash(i + 1.0), u);
}

void stockMovement()
{
    vec4 newPosition = vec4(a_Position, 1.0);
    float activeTime = u_Time;

    // 1. X축 이동 (시간의 흐름)
    // 화면을 벗어나면 다시 왼쪽으로 돌아오도록 mod 함수로 주기를 설정합니다.
    float moveX = min(activeTime * 0.5, 2.0) - 1.0; 
    newPosition.x += moveX;

    // 2. Y축 변동성 (주식의 잔파동)
    // 큰 파동(느린 노이즈)과 작은 파동(빠른 노이즈)을 섞어 진짜 주식처럼 불규칙하게 만듭니다.
    float volatility = noise(activeTime * 5.0) * 0.3 + noise(activeTime * 20.0) * 0.1;

    // 3. Y축 트렌드 (상승/하락 방향성)
    float trendSlope = 0.5; // 전체적인 상승/하락의 가파른 정도
    float trend = u_IsUpward ? (moveX * trendSlope) : -(moveX * trendSlope);

    // 4. 최종 Y 좌표 계산 (기본 위치 + 변동성 + 트렌드 방향)
    // 노이즈 값이 0.0 ~ 1.0 사이이므로, 화면 중앙에 맞추기 위해 임의의 오프셋(-0.2)을 줍니다.
    newPosition.y += (volatility - 0.2) + trend;

    gl_Position = newPosition;
}

void main()
{
    stockMovement();
}