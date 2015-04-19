#pragma once
#include "d3dUtility.h"
#include "TestSource.h"

class ISubBase
{
public:
	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight) = 0;
	virtual void Cleanup() = 0;
	virtual bool Display(float timeDelta) = 0;

protected:
	IDirect3DDevice9*		m_Device; 
	int						m_nWidth;
	int						m_nHeight; 

};
