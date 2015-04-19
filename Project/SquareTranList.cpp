#include "SquareTranList.h"



const DWORD SVertex::FVF = D3DFVF_XYZ;

CSquareTranList::CSquareTranList(void)
{
	m_VB = 0;
	m_IB = 0;
	m_Device = 0;
}
//-------------------------------------------------------------------------
CSquareTranList::~CSquareTranList(void)
{
}
//-------------------------------------------------------------------------
bool CSquareTranList::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (pDevice == 0)
	{
		return false;
	}
	m_Device = pDevice;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_Device->CreateVertexBuffer(
		8 * sizeof(SVertex),
		D3DUSAGE_WRITEONLY,
		SVertex::FVF,
		D3DPOOL_MANAGED,
		&m_VB,
		0);

	m_Device->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IB,
		0);

	SVertex* vertexs;
	m_VB->Lock(0,0,(void**)&vertexs,0);

	vertexs[0] = SVertex(-1.0f,-1.0f,-1.0f);
	vertexs[1] = SVertex(-1.0f,1.0f,-1.0f);
	vertexs[2] = SVertex(1.0f,1.0f,-1.0f);
	vertexs[3] = SVertex(1.0f,-1.0f,-1.0f);
	vertexs[4] = SVertex(-1.0f,-1.0f,1.0f);
	vertexs[5] = SVertex(-1.0f,1.0f,1.0f);
	vertexs[6] = SVertex(1.0f,1.0f,1.0f);
	vertexs[7] = SVertex(1.0f,-1.0f,1.0f);

	m_VB->Unlock();

	WORD* indexs = 0;
	m_IB->Lock(0,0,(void**)&indexs,0);

	int i = 0;

	indexs[0 + i] = 0; indexs[1 + i] = 1; indexs[2 + i] = 2;
	indexs[3 + i] = 0; indexs[4 + i] = 2; indexs[5 + i] = 3;

	i += 6;
	indexs[0 + i] = 4; indexs[1 + i] = 6; indexs[2 + i] = 5;
	indexs[3 + i] = 4; indexs[4 + i] = 7; indexs[5 + i] = 6;

	i += 6;
	indexs[0 + i] = 4; indexs[1 + i] = 5; indexs[2 + i] = 1;
	indexs[3 + i] = 4; indexs[4 + i] = 1; indexs[5 + i] = 0;

	i += 6;
	indexs[0 + i] = 3; indexs[1 + i] = 2; indexs[2 + i] = 6;
	indexs[3 + i] = 3; indexs[4 + i] = 6; indexs[5 + i] = 7;

	i += 6;
	indexs[0 + i] = 1; indexs[1 + i] = 5; indexs[2 + i] = 6;
	indexs[3 + i] = 1; indexs[4 + i] = 6; indexs[5 + i] = 2;

	i += 6;
	indexs[0 + i] = 4; indexs[1 + i] = 0; indexs[2 + i] = 3;
	indexs[3 + i] = 4; indexs[4 + i] = 3; indexs[5 + i] = 7;

	m_IB->Unlock();

	// 设置摄影机
	D3DXVECTOR3 position(0.0f,0.0f,-5.0f);
	D3DXVECTOR3 target(0.0f,0.0f,0.0f);
	D3DXVECTOR3 up(0.0f,1.0f,0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V,&position,&target,&up);
	
	m_Device->SetTransform(D3DTS_VIEW,&V);

	// 投影
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)m_nWidth / (float)m_nHeight,
		1.0f,
		1000.0f
		);

	m_Device->SetTransform(D3DTS_PROJECTION,&proj);
	m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	return true;
}
//-------------------------------------------------------------------------
void CSquareTranList::Cleanup()
{
	if (m_VB)
	{
		d3d::Release<IDirect3DVertexBuffer9*>(m_VB);
	}
	m_VB = 0;
	
	if (m_IB)
	{
		d3d::Release<IDirect3DIndexBuffer9*>(m_IB);
	}
	m_IB = 0;
	
}
//-------------------------------------------------------------------------
bool CSquareTranList::Display(float timeDelta)
{
	if (m_Device == 0)
	{
		return false;
	}
	D3DXMATRIX Rx,Ry,Rz;
	// X轴旋转 90° 
	D3DXMatrixRotationX(&Rx,3.14f / 4.0f);
	static float y = 0.0f,z = 0.0f;

	D3DXMatrixRotationY(&Ry,y);
	y += timeDelta;
	if (y >= 6.28f)
	{
		y = 0.0f;
	}

	D3DXMatrixRotationZ(&Rz,z);
	z += timeDelta;
	if (z >= 6.28f)
	{
		z = 0.0f;
	}

	D3DXMATRIX p = Rx * Ry * Rz;
	m_Device->SetTransform(D3DTS_WORLD,&p);

	// Instruct the m_Device to set each pixel on the back buffer black -
	// D3DCLEAR_TARGET: 0x00000000 (black) - and to set each pixel on
	// the depth buffer to a value of 1.0 - D3DCLEAR_ZBUFFER: 1.0f.
	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x000fff00, 1.0f, 0);
	//-------------------------------------------------------------------------
	m_Device->BeginScene();
	{
		m_Device->SetStreamSource(0,m_VB,0,sizeof(SVertex));
		m_Device->SetIndices(m_IB);
		m_Device->SetFVF(SVertex::FVF);
		m_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,8,0,12);
	}
	m_Device->EndScene();
	//-------------------------------------------------------------------------

	return true;
}
