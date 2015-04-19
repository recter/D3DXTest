//*************************************************************************
//	创建日期:	2015-4-16
//	文件名称:	FontFPS.h
//  创 建 人:   Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

//-------------------------------------------------------------------------

#include "ISubBase.h"

class CFontFPS :
	public ISubBase
{
public:
	CFontFPS(void);
	~CFontFPS(void);

	virtual bool Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight);
	virtual void Cleanup();
	virtual bool Display(float timeDelta);

private:

	void		__CalcFPS(float timeDelta);

private:
	 
	 
	ID3DXFont*				m_XFont;
	DWORD					m_FrameCut;
	float					m_fTimeElapsed;
	float					m_fFPS;
};

