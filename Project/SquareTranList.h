//*************************************************************************
//	创建日期:	2015-4-14
//	文件名称:	TextureCube.h
//  创 建 人:   Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------
#include "ISubBase.h"
//-------------------------------------------------------------------------
struct SVertex
{
	SVertex(){}
	SVertex(float x, float y , float z)
	{
		_x = x;
		_y = y;
		_z = z;
	}
	float _x,_y,_z;
	static const DWORD FVF;
};

class CSquareTranList :
	public ISubBase
{
public:
	CSquareTranList(void);
	~CSquareTranList(void);

	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	virtual void Cleanup();
	virtual bool Display(float timeDelta);

private:
	IDirect3DVertexBuffer9* m_VB;
	IDirect3DIndexBuffer9*	m_IB;
	 
	 
};

