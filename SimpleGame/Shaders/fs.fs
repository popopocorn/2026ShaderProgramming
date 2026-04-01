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

void FractalHypnosis()
{
    // 1. 화면 좌표를 복소수 평면(-1.5 ~ 1.5)으로 매핑합니다.
    // 중앙을 (0,0)으로 맞추고 화면에 프랙탈이 꽉 차도록 확대/축소 비율을 조정합니다.
    vec2 z = (UV.xy - 0.5) * 3.0; 
    
    // 2. 시간에 따라 변하는 복소수 상수 c 생성 (최면처럼 꿈틀거리는 핵심 형태)
    // u_Time을 이용해 상숫값이 원을 그리며 변하도록 합니다.
    float timeSpeed = u_Time * 0.3;
    vec2 c = vec2(cos(timeSpeed), sin(timeSpeed * 0.7)) * 0.78;
    
    // 3. 프랙탈 반복 계산 (Mandelbrot / Julia Set)
    int maxIter = 100;
    int iter;
    for(iter = 0; iter < maxIter; iter++)
    {
        // 복소수 제곱 연산: z^2 = (x+yi)^2 = x^2 - y^2 + 2xyi
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (2.0 * z.x * z.y) + c.y;
        
        // 원점에서 거리가 2를 넘어가면(발산) 반복을 멈춥니다. (x^2 + y^2 > 4.0)
        if((x * x + y * y) > 4.0) break;
        
        z = vec2(x, y);
    }
    
    // 4. 색상 계산 (Coloring)
    vec3 color;
    if (iter == maxIter) 
    {
        // 발산하지 않고 끝까지 남은 중심부는 깊이감을 위해 검은색으로 처리
        color = vec3(0.0); 
    } 
    else 
    {
        // 반복 횟수를 0.0 ~ 1.0 사이의 비율로 변환
        float t = float(iter) / float(maxIter);
        
        // 코사인 함수 기반의 부드러운 컬러 팔레트 생성 (사이키델릭한 네온 느낌)
        // t값과 u_Time을 섞어 색상이 계속 흘러가도록 만듭니다.
        vec3 paletteOffset = vec3(0.0, 0.33, 0.67);
        color = 0.5 + 0.5 * cos(6.28318 * (t * 3.0 - u_Time * 0.5 + paletteOffset));
        
        // 몽환적인 글로우(Glow) 효과를 위해 제곱근 활용
        color = sqrt(color * t * 2.5); 
    }
    
    FragColor = vec4(color, 1.0);
}
void main()
{
	FractalHypnosis();
}