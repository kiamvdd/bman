//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
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


float2 Distort(float2 p)
{
    float theta  = atan2(p.y, p.x);
    float radius = length(p);
    radius = pow(radius, 1.5f);
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5 * (p + 1.0);
}

//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
    uint width, height;
    gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy	
    float dx = 1.0f / width;
    float dy = 1.0f / height;
	
	float2 xy = 2.0 * input.TexCoord - 1.0;
	float2 uv;
	uv = Distort(xy);

	float2 mask = 1 - pow(saturate(abs(input.TexCoord * 2 - 1)), 3);

	float2 uvFinal = mask * uv + (1 - mask) * input.TexCoord;
	float4 c = gTexture.Sample(samPoint, uvFinal);
	return c;
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

