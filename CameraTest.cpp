//-------------------------------------------------------------------------
#include "CameraTest.h"
//-------------------------------------------------------------------------
const DWORD VertexCarema::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

CCameraTest::CCameraTest(void):m_CameraMgr(eCamera_AirCraft)
{
	m_Floor = 0;
	m_Texture = 0;
	m_Mesh = 0;
}
//-------------------------------------------------------------------------
CCameraTest::~CCameraTest(void)
{
}
//-------------------------------------------------------------------------
bool CCameraTest::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (0 == pDevice)
	{
		return false;
	}
	m_Device = pDevice;
	m_nHeight = nHeight;
	m_nWidth = nWidth;

	// 初始化场景纹理
	{
		m_Device->CreateVertexBuffer(
			6 * sizeof(VertexCarema),
			0, 
			VertexCarema::FVF,
			D3DPOOL_MANAGED,
			&m_Floor,
			0);

		VertexCarema* v = 0;
		m_Floor->Lock(0, 0, (void**)&v, 0);

		v[0] = VertexCarema(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[1] = VertexCarema(-20.0f, -2.5f,  20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[2] = VertexCarema( 20.0f, -2.5f,  20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

		v[3] = VertexCarema(-20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[4] = VertexCarema( 20.0f, -2.5f,  20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
		v[5] = VertexCarema( 20.0f, -2.5f, -20.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

		m_Floor->Unlock();

		//D3DXCreateCylinder(m_Device, 0.5f, 0.5f, 5.0f, 20, 20, &m_Mesh, 0);
		D3DXCreateTeapot(m_Device,&m_Mesh,0);

		D3DXCreateTextureFromFile(
			m_Device,
			s_szDesert,
			&m_Texture);
	}

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)m_nWidth / (float)m_nHeight,
		1.0f,
		1000.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}
//-------------------------------------------------------------------------
void CCameraTest::Cleanup()
{
	if (m_Floor)
	{
		d3d::Release<IDirect3DVertexBuffer9*>(m_Floor);
		m_Floor = 0;
	}

	if (m_Texture)
	{
		d3d::Release<IDirect3DTexture9*>(m_Texture);
		m_Texture = 0;
	}

	if (m_Mesh)
	{
		d3d::Release<ID3DXMesh*>(m_Mesh);
		m_Mesh = 0;
	}
	
}
//-------------------------------------------------------------------------
bool CCameraTest::Display(float timeDelta)
{
	if (0 == m_Device)
	{
		return false;
	}

	// 计算摄像机角度
	{
		if( ::GetAsyncKeyState('W') & 0x8000f )
			m_CameraMgr.Walk(4.0f * timeDelta);

		if( ::GetAsyncKeyState('S') & 0x8000f )
			m_CameraMgr.Walk(-4.0f * timeDelta);

		if( ::GetAsyncKeyState('A') & 0x8000f )
			m_CameraMgr.Strafe(-4.0f * timeDelta);

		if( ::GetAsyncKeyState('D') & 0x8000f )
			m_CameraMgr.Strafe(4.0f * timeDelta);

		if( ::GetAsyncKeyState('R') & 0x8000f )
			m_CameraMgr.Fly(4.0f * timeDelta);

		if( ::GetAsyncKeyState('F') & 0x8000f )
			m_CameraMgr.Fly(-4.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			m_CameraMgr.Pitch(1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			m_CameraMgr.Pitch(-1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			m_CameraMgr.Yaw(-1.0f * timeDelta);

		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			m_CameraMgr.Yaw(1.0f * timeDelta);

		if( ::GetAsyncKeyState('N') & 0x8000f )
			m_CameraMgr.Roll(1.0f * timeDelta);

		if( ::GetAsyncKeyState('M') & 0x8000f )
			m_CameraMgr.Roll(-1.0f * timeDelta);
		
	}

	// Update the view matrix representing the cameras 
	// new position/orientation.
	D3DXMATRIX V;
	m_CameraMgr.GetViewMatrix(&V);
	m_Device->SetTransform(D3DTS_VIEW, &V);

	
	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	m_Device->BeginScene();

	__Draw(1.0f);

	m_Device->EndScene();

	return true;
}
//-------------------------------------------------------------------------
void CCameraTest::__Draw(float scale)
{
	//
	// Pre-Render Setup
	//
	m_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	D3DXVECTOR3 dir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);

	m_Device->SetLight(0, &light);
	m_Device->LightEnable(0, true);
	m_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Render
	//

	D3DXMATRIX T, R, P, S;

	D3DXMatrixScaling(&S, scale, scale, scale);

	// used to rotate cylinders to be parallel with world's y-axis
	D3DXMatrixRotationX(&R, -D3DX_PI * 0.5f);

	// draw floor
	D3DXMatrixIdentity(&T);
	T = T * S;
	m_Device->SetTransform(D3DTS_WORLD, &T);
	m_Device->SetMaterial(&d3d::WHITE_MTRL);
	m_Device->SetTexture(0, m_Texture);
	m_Device->SetStreamSource(0, m_Floor, 0, sizeof(VertexCarema));
	m_Device->SetFVF(VertexCarema::FVF);
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// draw pillars
	m_Device->SetMaterial(&d3d::BLUE_MTRL);
	m_Device->SetTexture(0, 0);
	for(int i = 0; i < 5; i++)
	{
		D3DXMatrixTranslation(&T, -5.0f, 0.0f, -15.0f + (i * 7.5f));
		P = R * T * S;
		m_Device->SetTransform(D3DTS_WORLD, &P);
		m_Mesh->DrawSubset(0);

		D3DXMatrixTranslation(&T, 5.0f, 0.0f, -15.0f + (i * 7.5f));
		P = R * T * S;
		m_Device->SetTransform(D3DTS_WORLD, &P);
		m_Mesh->DrawSubset(0);
	}
}
//-------------------------------------------------------------------------