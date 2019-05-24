//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

Texture2D gTexture;
float pixelsAcross;
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
	uint width, height;
    gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy	
    float dx = 1.0f / width;
    float dy = 1.0f / height;
	
	float pixelsDown = pixelsAcross * (height / float(width));
	float2 t = float2(floor(input.TexCoord.x * pixelsAcross) / pixelsAcross, floor(input.TexCoord.y * pixelsDown) / pixelsDown);
	
	float4 col;
	float pixelHalfWidth = (1.0f / pixelsAcross) / 2.0f;
	float pixelHalfHeight = (1.0f / pixelsDown) / 2.0f;
	float2 dist = (t + float2(pixelHalfWidth, pixelHalfHeight)) - (input.TexCoord);
	dist.y *= height / float(width);
	float circleSize = 0.7f;
	if (length(dist) < (pixelHalfHeight / 2) * circleSize)
	{
		col = gTexture.Sample(samPoint, t);
		col.w = 1;
	} else {
		col = float4(0.2f, 0.2f, 0.2f, 1);
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

