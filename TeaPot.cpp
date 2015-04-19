#include "TeaPot.h"


const DWORD VertexTeaPot::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
CTeaPot::CTeaPot(void)
{
	m_Teapot = 0;
	m_BkGndQuad = 0;
	m_BkGndTex = 0;
	m_BackDropTex = 0;
}
//-------------------------------------------------------------------------
CTeaPot::~CTeaPot(void)
{
}
//-------------------------------------------------------------------------
bool CTeaPot::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (pDevice == 0)
	{
		return false;
	}

	m_Device = pDevice;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_TeapotMtrl = d3d::GREEN_MTRL;
	m_TeapotMtrl.Diffuse.a = 0.5f;

	m_BkGndMtrl = d3d::WHITE_MTRL;

	D3DXCreateTeapot(m_Device,&m_Teapot,0);

	//
	// Create the background quad.
	//

	m_Device->CreateVertexBuffer(
		6 * sizeof(VertexTeaPot), 
		D3DUSAGE_WRITEONLY,
		VertexTeaPot::FVF,
		D3DPOOL_MANAGED,
		&m_BkGndQuad,
		0);

	VertexTeaPot* v;
	m_BkGndQuad->Lock(0, 0, (void**)&v, 0);

	// 四方形一个
	v[0] = VertexTeaPot(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = VertexTeaPot(-10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = VertexTeaPot( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = VertexTeaPot(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = VertexTeaPot( 10.0f,  10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = VertexTeaPot( 10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_BkGndQuad->Unlock();


	//
	// Setup a directional light.
	//

	D3DLIGHT9 dir;
	::ZeroMemory(&dir, sizeof(dir));
	dir.Type      = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse   = d3d::WHITE;
	dir.Specular  = d3d::WHITE * 0.2f;
	dir.Ambient   = d3d::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(0.707f, 0.0f, 0.707f);

	m_Device->SetLight(0, &dir);
	m_Device->LightEnable(0, true);

	m_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Create texture and set texture filters.
	//

	D3DXCreateTextureFromFile(
		m_Device,
		s_szBoxPath,
		&m_BkGndTex);

	D3DXCreateTextureFromFile(
		m_Device,
		s_szDDSPath,
		&m_BackDropTex);

	m_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//
	// Set alpha blending states.
	//

	// use alpha in material's diffuse component for alpha
	m_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	// set blending factors so that alpha component determines transparency
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//
	// Set camera.
	//

	D3DXVECTOR3 pos(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);

	m_Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Set projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)m_nWidth / (float)m_nHeight,
		1.0f,
		1000.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}
//-------------------------------------------------------------------------
void CTeaPot::Cleanup()
{
	if (m_Teapot)
	{
		d3d::Release<ID3DXMesh*>(m_Teapot);
		m_Teapot = 0;
	}

	if (m_BkGndQuad)
	{
		d3d::Release<IDirect3DVertexBuffer9*>(m_BkGndQuad);
		m_BkGndQuad = 0;
	}

	if (m_BkGndTex)
	{
		d3d::Release<IDirect3DTexture9*>(m_BkGndTex);
		m_BkGndTex = 0;
	}

	if (m_BackDropTex)
	{
		d3d::Release<IDirect3DTexture9*>(m_BackDropTex);
		m_BackDropTex = 0;
	}

	
}
//-------------------------------------------------------------------------
bool CTeaPot::Display(float timeDelta)
{
	if (m_Device == 0)
	{
		return false;
	}
	//
	// Update
	//

	// increase/decrease alpha via keyboard input
	if( ::GetAsyncKeyState('A') & 0x8000f )
		m_TeapotMtrl.Diffuse.a += 0.001f;
	if( ::GetAsyncKeyState('S') & 0x8000f )
		m_TeapotMtrl.Diffuse.a -= 0.001f;

	// force alpha to [0, 1] interval
	if(m_TeapotMtrl.Diffuse.a > 1.0f)
		m_TeapotMtrl.Diffuse.a = 0.0f;
	if(m_TeapotMtrl.Diffuse.a < 0.0f)
		m_TeapotMtrl.Diffuse.a = 1.0f;

	//
	// Render
	//

	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
	m_Device->BeginScene();

	// Draw the background
	D3DXMATRIX W;
	D3DXMatrixIdentity(&W);
	m_Device->SetTransform(D3DTS_WORLD, &W);
	m_Device->SetFVF(VertexTeaPot::FVF);
	m_Device->SetStreamSource(0, m_BkGndQuad, 0, sizeof(VertexTeaPot));
	m_Device->SetMaterial(&m_BkGndMtrl);
	m_Device->SetTexture(0, m_BkGndTex);
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// Draw the teapot
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	D3DXMatrixScaling(&W, 1.5f, 1.5f, 1.5f);
	m_Device->SetTransform(D3DTS_WORLD, &W);
	m_Device->SetMaterial(&m_TeapotMtrl);
	m_Device->SetTexture(0, 0);
	m_Teapot->DrawSubset(0);  

	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	m_Device->EndScene();

	return true;
}
//-------------------------------------------------------------------------
