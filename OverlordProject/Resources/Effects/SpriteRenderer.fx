float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

RasterizerState BackCulling 
{ 
	CullMode = BACK; 
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	uint TextureId: TEXCOORD0;
	float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
	float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
	float4 Color: COLOR;	
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
	if (rotation != 0)
	{
		pos.xy = pivotOffset;
		//Step 3.
		//Do rotation calculations
		pos.xy = float2(pos.x * rotCosSin.x - pos.y * rotCosSin.y, pos.y * rotCosSin.x + pos.x * rotCosSin.y);
		pos.xy += offset;
		//Transform to origin
		//Rotate
		//Retransform to initial position
	}
	else
	{
		//Step 2.
		//No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
		//Just apply the pivot offset
		pos.xy += pivotOffset;
	}

	//Geometry Vertex Output
	GS_DATA geomData = (GS_DATA) 0;
	geomData.Position = mul(float4(pos, 1.0f), gTransform);
	geomData.Color = col;
	geomData.TexCoord = texCoord;
	triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Given Data (Vertex Data)
	float3 position = vertex[0].TransformData.xyz; //Extract the position data from the VS_DATA vertex struct
	float2 offset = vertex[0].TransformData.xy; //Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
	float rotation = vertex[0].TransformData.w; //Extract the rotation data from the VS_DATA vertex struct
	float2 pivot = vertex[0].TransformData2.xy; //Extract the pivot data from the VS_DATA vertex struct
	float2 scale = vertex[0].TransformData2.zw; //Extract the scale data from the VS_DATA vertex struct
	float2 texCoord = float2(0, 0); //Initial Texture Coordinate
	float2 cosSin = float2(0, 0);
	if (rotation != 0) {
		cosSin.x = cos(rotation);
		cosSin.y = sin(rotation);
	}
	//...
	
	// LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
	// |          / |
	// |   °   /    |
	// |    /       |
	// | /          |
	// LB----------RB

	float2 pivotOffset;

	pivotOffset.x = -(pivot.x * gTextureSize.x) * scale.x;
	pivotOffset.y = -(pivot.y * gTextureSize.y) * scale.y;
	//VERTEX 1 [LT]
	CreateVertex(triStream, position, vertex[0].Color, texCoord, rotation, cosSin, offset, pivotOffset); //Change the color data too!

	
	//VERTEX 2 [RT]
	pivotOffset.x = ((1.0f - pivot.x) * gTextureSize.x) * scale.x;
	pivotOffset.y = -(pivot.y * gTextureSize.y) * scale.y;
	texCoord = float2(1, 0);
	CreateVertex(triStream, position, vertex[0].Color, texCoord, rotation, cosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 3 [LB]
	pivotOffset.x = -(pivot.x * gTextureSize.x) * scale.x;
	pivotOffset.y = ((1.0f - pivot.y) * gTextureSize.y) * scale.y;
	texCoord = float2(0, 1);
	CreateVertex(triStream, position, vertex[0].Color, texCoord, rotation, cosSin, offset, pivotOffset); //Change the color data too!

	//VERTEX 4 [RB]
	pivotOffset.x = ((1.0f - pivot.x) * gTextureSize.x) * scale.x;
	pivotOffset.y = ((1.0f - pivot.y) * gTextureSize.y) * scale.y;
	texCoord = float2(1, 1);
	CreateVertex(triStream, position, vertex[0].Color, texCoord, rotation, cosSin, offset, pivotOffset); //Change the color data too!
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET {

	return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;	
}

// Default Technique
technique11 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//SetDepthStencilState(NoDepth,0);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
