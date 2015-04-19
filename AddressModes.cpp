//-------------------------------------------------------------------------
#include "AddressModes.h"
//-------------------------------------------------------------------------
const DWORD SVertexAddressMode::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
//-------------------------------------------------------------------------
CAddressMode::CAddressMode(void)
{
	m_Quad = 0;
	m_Tex = 0;
	m_Device = 0;
}
//-------------------------------------------------------------------------
CAddressMode::~CAddressMode(void)
{
}
//-------------------------------------------------------------------------
//
// Framework Functions
//
bool CAddressMode::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (pDevice == 0)
	{
		return false;
	}
	m_Device = pDevice;
	//
	// Create the Quad.
	//

	m_Device->CreateVertexBuffer(
		6 * sizeof(SVertexAddressMode), 
		D3DUSAGE_WRITEONLY,
		SVertexAddressMode::FVF,
		D3DPOOL_MANAGED,
		&m_Quad,
		0);

	SVertexAddressMode* v;
	m_Quad->Lock(0, 0, (void**)&v, 0);

	// quad built from two triangles:
	v[0] = SVertexAddressMode(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 4.0f);
	v[1] = SVertexAddressMode(-1.0f,  1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = SVertexAddressMode( 1.0f,  1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 4.0f, 0.0f);

	v[3] = SVertexAddressMode(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 4.0f);
	v[4] = SVertexAddressMode( 1.0f,  1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 4.0f, 0.0f);
	v[5] = SVertexAddressMode( 1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 4.0f, 4.0f);

	m_Quad->Unlock();

	//
	// Create the texture and set texture filters.
	//

	D3DXCreateTextureFromFile(
		m_Device,
		s_szBoxPath,
		&m_Tex);

	m_Device->SetTexture(0, m_Tex);

	m_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	m_Device->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,4);
	//
	// Don't use lighting for this sample.
	//
	m_Device->SetRenderState(D3DRS_LIGHTING, false);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)nWidth / (float)nHeight,
		1.0f,
		1000.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &proj);

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	return true;
}
//-------------------------------------------------------------------------
void CAddressMode::Cleanup()
{
	if (m_Quad)
	{
		d3d::Release<IDirect3DVertexBuffer9*>(m_Quad);
		m_Quad = 0;
	}
	
	if (m_Tex)
	{
		d3d::Release<IDirect3DTexture9*>(m_Tex);
		m_Tex = 0;
	}
	
}
//-------------------------------------------------------------------------
bool CAddressMode::Display(float timeDelta)
{
	//m_Device = 0;
	if( m_Device )
	{
		// 
		// Update the scene:
		//

		// set wrap address mode
		if( ::GetAsyncKeyState('W') & 0x8000f )
		{
			m_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			m_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		}

		// set border color address mode
		if( ::GetAsyncKeyState('B') & 0x8000f )
		{
			m_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
			m_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
			m_Device->SetSamplerState(0,  D3DSAMP_BORDERCOLOR, 0x000000ff);
		}

		// set clamp address mode
		if( ::GetAsyncKeyState('C') & 0x8000f )
		{
			m_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			m_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		}

		// set mirror address mode
		if( ::GetAsyncKeyState('M') & 0x8000f )
		{
			m_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
			m_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
		}	

		//
		// Draw the scene:
		//
		
		m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00ffff00, 1.0f, 0);
		m_Device->BeginScene();

		m_Device->SetStreamSource(0, m_Quad, 0, sizeof(SVertexAddressMode));
		m_Device->SetFVF(SVertexAddressMode::FVF);
		m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		m_Device->EndScene();

	}

	return true;
}
