//-------------------------------------------------------------------------
#include "FontFPS.h"
//-------------------------------------------------------------------------


CFontFPS::CFontFPS(void)
{
	m_XFont = 0;
	m_FrameCut = 0;
	m_fTimeElapsed = 0;
	m_fFPS = 0;
}
//-------------------------------------------------------------------------
CFontFPS::~CFontFPS(void)
{
}
//-------------------------------------------------------------------------
bool CFontFPS::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (0 == pDevice)
	{
		return false;
	}
	m_Device = pDevice;
	m_nHeight = nHeight;
	m_nWidth = nWidth;
	
	D3DXFONT_DESCA lf;
	::ZeroMemory(&lf,sizeof(D3DXFONT_DESCA));
	
	lf.Height = 25;
	lf.Width = 14;
	lf.Weight = 500;
	lf.Italic = false;
	lf.CharSet = DEFAULT_CHARSET;
#pragma warning ( disable: 4996 )
	strcpy(lf.FaceName,"Times New Roman");
#pragma warning ( default : 4996 )
	D3DXCreateFontIndirect(m_Device,&lf,&m_XFont);


	return true;

}
//-------------------------------------------------------------------------
void CFontFPS::Cleanup()
{
	if (m_XFont)
	{
		d3d::Release<ID3DXFont*>(m_XFont);
		m_XFont = 0;
	}
	
}
//-------------------------------------------------------------------------
bool CFontFPS::Display(float timeDelta)
{
	if (0 == m_Device)
	{
		return false;
	}
	__CalcFPS(timeDelta);


	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	m_Device->BeginScene();

	RECT rect = {100, 0, m_nWidth, m_nHeight};
	char szBuff[40];
	memset(szBuff,40,0);
	int nFPS = static_cast<int>(m_fFPS);

#pragma warning ( disable: 4996 )
	sprintf(szBuff,"FPS:%d",nFPS);
#pragma warning ( default : 4996 )
	
	m_XFont->DrawText(NULL,  szBuff,  -1,  &rect, DT_TOP | DT_LEFT, 0xffff0000);  

	m_Device->EndScene();

	return true;
}
//-------------------------------------------------------------------------
void CFontFPS::__CalcFPS(float timeDelta)
{
	m_FrameCut ++;
	m_fTimeElapsed += timeDelta;
	if (m_fTimeElapsed >= 1.0f)
	{
		m_fFPS = static_cast<float>(m_FrameCut) / m_fTimeElapsed;
		m_FrameCut = 0;
		m_fTimeElapsed = 0.0f;
	}
}
