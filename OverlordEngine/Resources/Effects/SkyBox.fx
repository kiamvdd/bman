SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

TextureCube m_CubeMap : CubeMap;

cbuffer cbChangesEveryFrame
{
	matrix matWorldViewProj : WorldViewProjection;
}

struct VS_IN
{
	float3 posL : POSITION;
};

struct VS_OUT
{
	float4 posH : SV_POSITION;
	float3 texC : TEXCOORD;
};

RasterizerState NoCulling 
{ 
	CullMode = NONE; 
};

DepthStencilState LessEqual
{
	DepthFunc = LESS_EQUAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUT VS( VS_IN vIn )
{
	VS_OUT vOut = (VS_OUT)0;
	vOut.posH = mul(float4(vIn.posL, 0.0f), matWorldViewProj).xyww;
	vOut.texC = vIn.posL;
	// use local vertex position as cubemap lookup vector

	return vOut;
}
//--------------------------------------------------------------------------------------
// Pixel XMeshShader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUT pIn): SV_Target
{
	return m_CubeMap.Sample(samLinear, pIn.texC);
}

technique10 Render
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(LessEqual, 0);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}