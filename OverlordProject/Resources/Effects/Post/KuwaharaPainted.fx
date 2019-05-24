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
int regionsize;

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
    float dx = 1.0f / width;
    float dy = 1.0f / height;
	
	float4 c1 = float4(0, 0, 0, 1);
	float4 c2 = float4(0, 0, 0, 1);
	float4 c3 = float4(0, 0, 0, 1);
	float4 c4 = float4(0, 0, 0, 1);
	float d1 = 0;
	float d2 = 0;
	float d3 = 0;
	float d4 = 0;
	
	float4 col = gTexture.Sample(samPoint, input.TexCoord + float2(-regionsize * dx, -regionsize * dy));
	float prevAvg = (col.x + col.y + col.z) * 0.333f;
	for	(int i = -regionsize; i < 1; ++i) {
		for	(int j = -regionsize; j < 1; ++j) {
			col = gTexture.Sample(samPoint, input.TexCoord + float2(i * dx, j * dy));
			float avg = (col.x + col.y + col.z) * 0.333f;
			d1 += abs(prevAvg - avg);
			prevAvg = avg;
			c1 += col;
		}
	}
	c1 /= ((regionsize + 1) * (regionsize + 1));

	
	col = gTexture.Sample(samPoint, input.TexCoord + float2(0 * dx, -regionsize * dy));
	prevAvg = (col.x + col.y + col.z) * 0.333f;
	for	(int i = 0; i < regionsize + 1; ++i) {
		for	(int j = -regionsize; j < 1; ++j) {
			col = gTexture.Sample(samPoint, input.TexCoord + float2(i * dx, j * dy));
			float avg = (col.x + col.y + col.z) * 0.333f;
			d2 += abs(prevAvg - avg);
			prevAvg = avg;
			c2 += col;
		}
	}
		c2 /= ((regionsize + 1) * (regionsize + 1));


		col = gTexture.Sample(samPoint, input.TexCoord + float2(-regionsize * dx, 0 * dy));
	prevAvg = (col.x + col.y + col.z) * 0.333f;
	for	(int i = -regionsize; i < 1; ++i) {
		for	(int j = 0; j < regionsize + 1; ++j) {
			col = gTexture.Sample(samPoint, input.TexCoord + float2(i * dx, j * dy));
			float avg = (col.x + col.y + col.z) * 0.333f;
			d3 += abs(prevAvg - avg);
			prevAvg = avg;
			c3 += col;
		}
	}
		c3 /= ((regionsize + 1) * (regionsize + 1));


	col = gTexture.Sample(samPoint, input.TexCoord + float2(0 * dx, 0 * dy));
	prevAvg = (col.x + col.y + col.z) * 0.333f;
	for	(int i = 0; i < regionsize + 1; ++i) {
		for	(int j = 0; j < regionsize + 1; ++j) {
			col = gTexture.Sample(samPoint, input.TexCoord + float2(i * dx, j * dy));
			float avg = (col.x + col.y + col.z) * 0.333f;
			d4 += abs(prevAvg - avg);
			prevAvg = avg;
			c4 += col;
		}
	}
		c4 /= ((regionsize + 1) * (regionsize + 1));

	
	if (d1 < d2 && d1 < d3 && d1 < d4) { // d1 is the smallest
		return c1;
	} else if (d2 < d3 && d2 < d4) { //d2 is the smallest
		return c2;
	} else if (d3 < d4) { //d3 is the smallest
		return c3;
	} else { //d4 is the states
		return c4;
	}
	
	return float4(1, 0, 1, 1);
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

