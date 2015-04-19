//*************************************************************************
//	��������:	2015-4-14
//	�ļ�����:	CubeBox.h
//  �� �� ��:   Rect 	
//	��Ȩ����:	MIT
//	˵    ��:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------

#include "ISubBase.h"

struct VertexBox
{
	VertexBox(){}
	VertexBox(
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

class CCubeBox :
	public ISubBase
{
public:
	CCubeBox(void);
	~CCubeBox(void);
	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	virtual void Cleanup();
	virtual bool Display(float timeDelta);

private:
	int m_nWidth;
	int m_nHeight;
	IDirect3DTexture9* m_Tex;
	IDirect3DVertexBuffer9* m_VB;
	IDirect3DIndexBuffer9*  m_IB;
};

