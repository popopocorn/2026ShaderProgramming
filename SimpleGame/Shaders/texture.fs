#version 330

uniform sampler2D uTexSampler;
uniform sampler2D u_CurNumTex;
layout(location=0) out vec4 FragColor;
uniform float u_Time;
uniform int u_Idx;


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
	float tx = UV.x / 5;
	float ty = UV.y / 2;


	
	float offsetX = fract(u_Idx/5.0);
	float offsetY = floor(u_Idx/5.0)/2.0;


	FragColor = texture(uTexSampler, vec2(tx + offsetX, ty + offsetY));


}	

void Num2()
{
    // 1. 숫자를 십의 자리와 일의 자리로 분리 (예: u_Idx가 42라면 4와 2)
    // 참고: 셰이더에서 float의 오차를 방지하기 위해 +0.1 등의 보정을 넣기도 하지만, 보통 정수값이 들어온다면 아래로 충분합니다.
    float tens = floor(u_Idx / 10.0);
    float ones = mod(u_Idx, 10.0);

    float currentDigit = 0.0;
    float localUVx = 0.0;

    // 2. 화면(Quad)을 가로로 반으로 분할
    if (UV.x < 0.5) 
    {
        // 왼쪽 영역 (십의 자리)
        // u_Idx가 10 미만인 경우(예: 숫자 '5') 앞의 '0'을 렌더링하지 않고 투명하게 만듦
        if (u_Idx < 10.0) {
            FragColor = vec4(0.0); // 투명 처리 (알파 블렌딩 활성화 필요)
            return; 
        }
        currentDigit = tens;
        localUVx = UV.x * 2.0; // 0.0 ~ 0.5 구간을 0.0 ~ 1.0으로 확장
    } 
    else 
    {
        // 오른쪽 영역 (일의 자리)
        currentDigit = ones;
        localUVx = (UV.x - 0.5) * 2.0; // 0.5 ~ 1.0 구간을 0.0 ~ 1.0으로 확장
    }

    // 3. 재조정된 UV와 현재 자릿수(currentDigit)를 바탕으로 텍스처 샘플링
    float tx = localUVx / 5.0;
    float ty = UV.y / 2.0;

    float offsetX = fract(currentDigit / 5.0);
    float offsetY = floor(currentDigit / 5.0) / 2.0;

    FragColor = texture(uTexSampler, vec2(tx + offsetX, ty + offsetY));
}

void main()
{
	Num2();
	
}