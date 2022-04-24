struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord : TEXCOORD0;
};


cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;

	float m_angle;
};



VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//output.position = lerp(input.position, input.position1, (sin(m_angle) + 1.0f) / 2.0f);

	//world space
	output.position = mul(input.position, m_world);
	//view space
	output.position = mul(output.position, m_view);
	//screen space
	output.position = mul(output.position, m_proj);


	output.texcoord = input.texcoord;
	return output;
}