#include "CubeBox.h"

const DWORD VertexBox::FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);

CCubeBox::CCubeBox(void)
{
	m_Tex = 0;
	m_VB = 0;
	m_IB = 0;
}
//-------------------------------------------------------------------------
CCubeBox::~CCubeBox(void)
{
}
//-------------------------------------------------------------------------
bool CCubeBox::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (pDevice == 0)
	{
		return false;
	}
	m_Device = pDevice;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_Device->CreateVertexBuffer(
		24 * sizeof(VertexBox), 
		D3DUSAGE_WRITEONLY,
		VertexBox::FVF,
		D3DPOOL_MANAGED,
		&m_VB,
		0);

	VertexBox* v;
	m_VB->Lock(0, 0, (void**)&v, 0);

	// build box

	// fill in the front face vertex data
	v[0] = VertexBox(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = VertexBox(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[2] = VertexBox( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[3] = VertexBox( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	// fill in the back face vertex data
	v[4] = VertexBox(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[5] = VertexBox( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = VertexBox( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[7] = VertexBox(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// fill in the top face vertex data
	v[8]  = VertexBox(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[9]  = VertexBox(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = VertexBox( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[11] = VertexBox( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the bottom face vertex data
	v[12] = VertexBox(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[13] = VertexBox( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = VertexBox( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[15] = VertexBox(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the left face vertex data
	v[16] = VertexBox(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[17] = VertexBox(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[18] = VertexBox(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[19] = VertexBox(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// fill in the right face vertex data
	v[20] = VertexBox( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[21] = VertexBox( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[22] = VertexBox( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[23] = VertexBox( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	m_VB->Unlock();

	m_Device->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		0);

	WORD* i = 0;
	m_IB->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 4; i[7]  = 5; i[8]  = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// fill in the top face index data
	i[12] = 8; i[13] =  9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	m_IB->Unlock();

	//
	// Set a directional light.
	//

	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular  = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_Device->SetLight(0, &light);
	m_Device->LightEnable(0, true);

	m_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Create texture.
	//
	D3DXCreateTextureFromFile(
		m_Device,
		s_szBoxPath,
		&m_Tex);

	m_Device->SetTexture(0, m_Tex);
	// 
	// Set Texture Filter States.
	//

	m_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// Set the projection matrix.
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
void CCubeBox::Cleanup()
{
	if (m_Tex)
	{
		d3d::Release(m_Tex);
		m_Tex = 0;
	}

	if (m_VB)
	{
		d3d::Release(m_VB);
		m_VB = 0;
	}

	if (m_IB)
	{
		d3d::Release(m_IB);
		m_IB = 0;
	}
	m_Device = 0;
}
//-------------------------------------------------------------------------
bool CCubeBox::Display(float timeDelta)
{
	if (m_Device == 0)
	{
		return false;
	}

	static float angle  = (3.0f * D3DX_PI) / 2.0f;
	static float height = 2.0f;

	if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
		angle -= 0.5f * timeDelta;

	if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
		angle += 0.5f * timeDelta;

	if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
		height += 5.0f * timeDelta;

	if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
		height -= 5.0f * timeDelta;

	D3DXVECTOR3 position( cosf(angle) * 3.0f, height, sinf(angle) * 3.0f );
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	m_Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Draw the scene:
	//

	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x000fff0f, 1.0f, 0);
	m_Device->BeginScene();

	m_Device->SetMaterial(&d3d::WHITE_MTRL);
	m_Device->SetTexture(0, m_Tex);


	m_Device->SetStreamSource(0, m_VB, 0, sizeof(VertexBox));
	m_Device->SetIndices(m_IB);
	m_Device->SetFVF(VertexBox::FVF);
	m_Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, 
		0,                  
		0,                  
		24,
		0,
		12);  

	m_Device->EndScene();

	return true;
}
//-------------------------------------------------------------------------

