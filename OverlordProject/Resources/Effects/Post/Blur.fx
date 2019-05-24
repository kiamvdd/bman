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
DepthStencilState gDSS_DepthWriting
{
    DepthEnable = TRUE;
};

/// Create Rasterizer State (Backface culling) 
RasterizerState gRS_FrontCulling
{
    CullMode = NONE;
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
	// Set the Position
    output.Position = float4(input.Position, 1);
	// Set the TexCoord
    output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	// Step 1: find the dimensions of the texture (the texture has a method for that)	
    uint width, height;
    gTexture.GetDimensions(width, height);
	// Step 2: calculate dx and dy	
    float dx = 1.0f / width;
    float dy = 1.0f / height;

    float3 finalColor = float3(0,0,0);

    uint rows = 10;
    uint columns = 10;
	// Step 3: Create a double for loop (5 iterations each)
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            //Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
            float2 offset = float2((c-(columns / 2.0f)) * dx, (r-(rows/2.0f)) * dy);

			//Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
            finalColor += gTexture.Sample(samPoint, input.TexCoord + offset);
        }
    }

	// Step 4: Divide the final color by the number of passes (in this case 5*5)
    finalColor /= rows * columns;

	// Step 5: return the final color

    return float4(finalColor,1.0f);
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		// Set states...
        SetRasterizerState(gRS_FrontCulling);
        SetDepthStencilState(gDSS_DepthWriting, 0);

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}