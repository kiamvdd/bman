#pragma once
#include "ContentLoader.h"
#include <unordered_map>
#include "..\Graphics\MeshFilter.h"

class MeshFilterLoader : public ContentLoader<MeshFilter>
{
public:
	MeshFilterLoader(void){};
	virtual ~MeshFilterLoader(void){};

protected:
	virtual MeshFilter* LoadContent(const wstring& assetFile);
	virtual void Destroy(MeshFilter* objToDestroy);

private:

	enum MeshDataType : int
	{
		END = 0,
		HEADER = 1,
		POSITIONS = 2,
		INDICES = 3,
		NORMALS = 4,
		BINORMALS = 5,
		TANGENTS = 6,		
		COLORS = 7,
		TEXCOORDS = 8,
		BLENDINDICES = 9,
		BLENDWEIGHTS = 10,
		ANIMATIONCLIPS = 11,
		SKELETON = 12
	};

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MeshFilterLoader(const MeshFilterLoader &obj);
	MeshFilterLoader& operator=(const MeshFilterLoader& obj);
};