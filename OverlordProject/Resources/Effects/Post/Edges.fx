//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepth
{
	DepthFunc = LESS_EQUAL;
};
/// Create Rasterizer State (Backface culling) 
RasterizerState BackCull
{
	CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
    // Step 1: sample the texture
	// Step 2: calculate the mean value
	// Step 3: return the color

	
	// 1280 x 720
	uint width, height;
    gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy	
    float dx = 1.0f / width;
    float dy = 1.0f / height;
	
	float4 col = float4(1, 1, 1, 1);
	float4 p0 = gTexture.Sample(samPoint, input.TexCoord);
    float4 p1 = gTexture.Sample(samPoint, input.TexCoord + (dx, 0));
	float4 p2 = gTexture.Sample(samPoint, input.TexCoord + (-dx, 0));
	float4 p3 = gTexture.Sample(samPoint, input.TexCoord + (0, dy));
	float4 p4 = gTexture.Sample(samPoint, input.TexCoord + (0, dy));
	
	float a0 = (p0.x + p0.y + p0.z) / 3.0f;
	float a1 = (p1.x + p1.y + p1.z) / 3.0f;
	float a2 = (p2.x + p2.y + p2.z) / 3.0f;
	float a3 = (p3.x + p3.y + p3.z) / 3.0f;
	float a4 = (p4.x + p4.y + p4.z) / 3.0f;
	float threshold = 0.1f;
	
	if (p0.x > 0.8f) {
		col = float4(1, 0, 0, 1);
	} else if (abs(a0 - a1) > threshold || abs(a0 - a2) > threshold || abs(a0 - a3) > threshold || abs(a0 - a4) > threshold) {
		col = float4(0, 0, 0, 1);
	}
	return col;
}


//TECHNIQUE
//---------
technique11 Grayscale
{
    pass P0
    {          
        // Set states...
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
		SetDepthStencilState(EnableDepth, 1);
		SetRasterizerState(BackCull);
    }
}

