//*************************************************************************
//	创建日期:	2015-4-14
//	文件名称:	TeaPot.h
//  创 建 人:   Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------

#include "ISubBase.h"

struct VertexTeaPot
{
	VertexTeaPot(){}
	VertexTeaPot(
		float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v; // texture coordinates

	static const DWORD FVF;
};

class CTeaPot :
	public ISubBase
{
public:
	CTeaPot(void);
	~CTeaPot(void);

	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	virtual void Cleanup();
	virtual bool Display(float timeDelta);

private:
	ID3DXMesh*				m_Teapot;
	D3DMATERIAL9			m_TeapotMtrl;
	IDirect3DVertexBuffer9* m_BkGndQuad;
	IDirect3DTexture9*		m_BkGndTex;
	IDirect3DTexture9*		m_BackDropTex;
	D3DMATERIAL9			m_BkGndMtrl;
	 
	 

};

