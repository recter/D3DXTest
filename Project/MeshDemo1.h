//*************************************************************************
//	创建日期:	2015-4-17
//	文件名称:	MeshDemo1.h
//  创 建 人:   Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------

#include "ISubBase.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, 
		float nx, float ny, float nz, float u, float v)
	{
		_x = x;   _y = y;   _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u;   _v = v;
	}

	float _x, _y, _z, _nx, _ny, _nz, _u, _v;

	static const DWORD FVF;
};
//-------------------------------------------------------------------------
class CMeshDemo1 :
	public ISubBase
{
public:
	CMeshDemo1(void);
	~CMeshDemo1(void);

	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	virtual void Cleanup();
	virtual bool Display(float timeDelta);

private:
	void __dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh);
	void __dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh);
	void __dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
	void __dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
	void __dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh);

public:
	const DWORD				m_NumSubsets;

private:
	 
	 
	ID3DXMesh*				m_Mesh;
	std::vector<IDirect3DTexture9*>		m_vecTextures;
	std::ofstream			m_OutFile; 
};

