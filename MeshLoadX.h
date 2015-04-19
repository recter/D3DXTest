//*************************************************************************
//	创建日期:	2015-4-17
//	文件名称:	MeshLoadX.h
//  创 建 人:   Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------


#include "ISubBase.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <ostream>

class CMeshLoadX :
	public ISubBase
{
public:
	CMeshLoadX(void);
	~CMeshLoadX(void);

	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	virtual void Cleanup();
	virtual bool Display(float timeDelta);


private:
	
	void __dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh);
	void __dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh);
	void __dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
	void __dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh);
	void __dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh);
	bool __ComputeBoundingSphere(ID3DXMesh* mesh, d3d::BoundingSphere* sphere);
	bool __ComputeBoundingBox(ID3DXMesh* mesh, d3d::BoundingBox*    box);

private:
	ID3DXMesh*						m_SourceMesh;
	ID3DXPMesh*						m_ProcessMesh;
	ID3DXMesh*						m_SphereMesh;
	ID3DXMesh*						m_BoxMesh;
	std::vector<D3DMATERIAL9>		m_vecMtrls;
	std::vector<IDirect3DTexture9*>	m_vecTextures;
	std::ofstream					m_OutFile; 
	ID3DXFont*						m_XFont;
	DWORD							m_FaceTotal;
	DWORD							m_VertexTotal;
	DWORD							m_IndexTotal;
};

