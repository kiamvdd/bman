//DX10 - FLAT SHADER
//Digital Arts & Entertainment


//GLOBAL VARIABLES
//****************
float4x4 gMatrixWVP : WORLDVIEWPROJECTION;
float4x4 gMatrixWorld : WORLD;
float4 gFoamColor : COLOR = float4(1, 1, 1.0, 1.0);
float gOffsetNoiseScale = 20;
float gTime : TIME;
float gWaveHeight = 1;

Texture2D gTexturePacked
<
	string UIName = "Packed Water Texture";
	string UIWidget = "Texture";
	string ResourceName = "BMAN_Water_Packed.png";
>;

Texture2D gFoamPacked
<
	string UIName = "Packed Foam Texture";
	string UIWidget = "Texture";
	string ResourceName = "BMAN_Foam_Packed.png";
>;

SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
 	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};


DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

//STATES
//******
RasterizerState gRS_NoCulling { 
	CullMode = NONE;      
};


struct VS_Input
{
	float3 Position: POSITION;
	float2 TexCoord: TEXCOORD0;
};

struct VS_Output
{
	float4 Position: SV_POSITION;
	float4 WorldPosition : COLOR0;
	float2 TexCoord: TEXCOORD0;
};

//MAIN VERTEX SHADER
//******************
VS_Output MainVS(VS_Input input) {
	VS_Output output = (VS_Output)0;
	output.WorldPosition = mul(float4(input.Position, 1.0), gMatrixWorld);
	float y1 = gTexturePacked.SampleLevel(gTextureSampler, (output.WorldPosition.xz + float2(gTime, gTime * 0.83f)) / gOffsetNoiseScale, 0).y;
	float y2 = gTexturePacked.SampleLevel(gTextureSampler, (output.WorldPosition.xz + float2(-gTime, gTime * -1.03f)) / gOffsetNoiseScale, 0).y;

	float offset = (y1 * y2) / 2.0f;
	input.Position.z -= offset * gWaveHeight;
	output.Position = mul(float4(input.Position, 1.0), gMatrixWVP);
	output.TexCoord = input.TexCoord;

	return output;
};


//MAIN PIXEL SHADER
//*****************
float4 MainPS(VS_Output input) : SV_TARGET {
	float y1 = gTexturePacked.SampleLevel( gTextureSampler, (input.WorldPosition.xz + float2(gTime, gTime * 0.83f)) / gOffsetNoiseScale, 0 ).y;
	float y2 = gTexturePacked.SampleLevel( gTextureSampler, (input.WorldPosition.xz + float2(-gTime, gTime * -1.03f)) / gOffsetNoiseScale, 0 ).y;
	float fb1 = gFoamPacked.Sample(gTextureSampler, float2(input.TexCoord.x, saturate(input.TexCoord.y * 2) * 0.6f) + float2(gTime * 0.2f, 0)).z;
	float fb2 = gFoamPacked.Sample(gTextureSampler, float2(input.TexCoord.x, 1 - saturate(input.TexCoord.y * 2) * 0.5f) + float2(-gTime * 0.1f, 0)).z;
	float fe1 = gFoamPacked.Sample(gTextureSampler, input.TexCoord + float2(0, -gTime * 0.3f)).x;
	float fe2 = gFoamPacked.Sample(gTextureSampler, input.TexCoord + float2(gTime * 0.1f, -gTime * 0.4f)).y;
	float mask = gTexturePacked.Sample(gTextureSampler, input.TexCoord + float2(0, -gTime * 0.1f)).y;
	float foamMask = (fb1 * fb2 + fe1 * fe2) * (1 - input.TexCoord.y) + mask * 0.3f;
	float4 color = gFoamColor * foamMask;
	clip(foamMask > 0.5 ? 1 : -1);
	return gFoamColor;
};


//TECHNIQUES
//**********
technique10 DefaultTechnique {
	pass p0 {
		SetRasterizerState(gRS_NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetGeometryShader(NULL);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}