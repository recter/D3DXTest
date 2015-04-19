//*************************************************************************
//	创建日期:	2015-4-19
//	文件名称:	CameraTest.h
//  创 建 人:   Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------

#include "ISubBase.h"
#include "CameraMgr.h"

struct VertexCarema
{
	VertexCarema(){}
	VertexCarema(float x, float y, float z, 
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
class CCameraTest :
	public ISubBase
{
public:
	CCameraTest(void);
	~CCameraTest(void);

	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	virtual void Cleanup();
	virtual bool Display(float timeDelta);

private:

	void	__Draw(float scale);

private:
	CCameraMgr				m_CameraMgr;
	IDirect3DVertexBuffer9*	m_Floor;
	IDirect3DTexture9*		m_Texture;
	ID3DXMesh*				m_Mesh;
};

