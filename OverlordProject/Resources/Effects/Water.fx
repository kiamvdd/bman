//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWVP : WORLDVIEWPROJECTION;
float4x4 gMatrixWorld : WORLD;
float4 gWaterColor : COLOR = float4(0, 0.514f, 1.0, 1.0);
float gOffsetNoiseScale = 20;
float gFoamScale = 10;
float gTime;
float gFoamIntensity = 0.3f;
float gOffsetColorMultiplier = 0.6f;
float gWaveHeight = 1;

Texture2D gTexturePacked
<
	string UIName = "Packed Water Texture";
	string UIWidget = "Texture";
	string ResourceName = "BMAN_Water_Packed.png";
>;

SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
 	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

//STATES
//******
RasterizerState gRS_NoCulling { 
	FillMode = SOLID;
	CullMode = FRONT;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};


struct VS_Input
{
	float3 Position: POSITION;
	float3 Normal: NORMAL;
	float2 TexCoord: TEXCOORD0;
};

struct VS_Output
{
	float4 Position: SV_POSITION;
	float4 WorldPosition : COLOR0;
	float3 Normal: NORMAL;
	float2 TexCoord: TEXCOORD0;
	float Offset : TEXCOORD1;
};

//MAIN VERTEX SHADER
//******************
VS_Output MainVS(VS_Input input) {
	VS_Output output = (VS_Output)0;
	output.WorldPosition = mul(float4(input.Position, 1.0), gMatrixWorld);
	float y1 = gTexturePacked.SampleLevel( gTextureSampler, (output.WorldPosition.xz + float2(gTime, gTime * 0.83f)) / gOffsetNoiseScale, 0 ).y;
	float y2 = gTexturePacked.SampleLevel( gTextureSampler, (output.WorldPosition.xz + float2(-gTime, gTime * -1.03f)) / gOffsetNoiseScale, 0 ).y;

	output.Offset = (y1 * y2) / 2.0f;
	input.Position.z -= output.Offset * gWaveHeight;
	output.Position = mul(float4(input.Position, 1.0), gMatrixWVP);
	output.Normal = mul(input.Normal, (float3x3)gMatrixWorld);
	output.TexCoord = input.TexCoord;
	
	return output;
};


//MAIN PIXEL SHADER
//*****************
float4 MainPS(VS_Output input) : SV_TARGET {
	float y1 = gTexturePacked.SampleLevel( gTextureSampler, (input.WorldPosition.xz + float2(gTime, gTime * 0.83f)) / gOffsetNoiseScale, 0 ).y;
	float y2 = gTexturePacked.SampleLevel( gTextureSampler, (input.WorldPosition.xz + float2(-gTime, gTime * -1.03f)) / gOffsetNoiseScale, 0 ).y;
	float foam = gFoamIntensity * gTexturePacked.Sample(gTextureSampler, input.WorldPosition.xz / gFoamScale + float2(sin(gTime / 3.0f) * 0.05f + y2 / gOffsetNoiseScale, sin(gTime / 3.0f) * 0.05f + y1 / gOffsetNoiseScale)).x;
	float4 color = gWaterColor + input.Offset * gOffsetColorMultiplier + foam;
	return color;
};


//TECHNIQUES
//**********
technique10 DefaultTechnique {
	pass p0 {
		SetDepthStencilState(EnableDepth, 0);
		SetRasterizerState(gRS_NoCulling);
		SetGeometryShader(NULL);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}