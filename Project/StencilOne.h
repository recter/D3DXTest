//*************************************************************************
//	创建日期:	2015-4-15
//	文件名称:	StencilOne.h
//  创 建 人:   Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------

#include "ISubBase.h"

//
// Classes and Structures
//
struct VertexStencil
{
	VertexStencil(){}
	VertexStencil(float x, float y, float z, 
		float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF;
};
//-------------------------------------------------------------------------
class CStencilOne :
	public ISubBase
{
public:
	CStencilOne(void);
	~CStencilOne(void);

	

	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	virtual void Cleanup();
	virtual bool Display(float timeDelta);

private:

	void		__RenderScene();
	void		__RenderMirror();
	void		__RenderShadow();
	void		__CreateCubeBox();

private:
	 
	 

	IDirect3DVertexBuffer9* m_VB;

	IDirect3DTexture9*		m_FloorTex;
	IDirect3DTexture9*		m_WallTex;
	IDirect3DTexture9*		m_MirrorTex;

	D3DMATERIAL9			m_FloorMtrl;
	D3DMATERIAL9			m_WallMtrl;
	D3DMATERIAL9			m_MirrorMtrl;

	ID3DXMesh*				m_Teapot;
	D3DXVECTOR3				m_TeapotPosition;
	D3DMATERIAL9			m_TeapotMtrl;

	IDirect3DTexture9*		m_TexCube;
	IDirect3DVertexBuffer9* m_VBCube;
	IDirect3DIndexBuffer9*  m_IBCube;
};

