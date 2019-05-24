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
	
	uint width, height;
    gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy	
    float dx = 3.0f / width;
    float dy = 3.0f / height;
	
	int posY = input.TexCoord.y * height;
	
	int colIndex = posY % 3;
	float2 t = input.TexCoord;
	t.x *= width / 3.0f;
	t.x = floor(t.x) / (width / 3.0f);

	t.y *= height / 3.0f;
	t.y = floor(t.y) / (height / 3.0f);
	
	float4 col = gTexture.Sample(samPoint, t);
	if (colIndex == 0) {
		col = float4(col.r, col.r, col.r, 1);
	} else if (colIndex == 1) {
		col = float4(col.g, col.g, col.g, 1);
	} else {
		col = float4(col.b, col.b, col.b, 1);
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

