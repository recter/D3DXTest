#include "StencilOne.h"


const DWORD VertexStencil::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

CStencilOne::CStencilOne(void)
{
	m_VB = 0;

	m_FloorTex  = 0;
	m_WallTex   = 0;
	m_MirrorTex = 0;

	m_FloorMtrl  = d3d::WHITE_MTRL;
	m_WallMtrl   = d3d::WHITE_MTRL;
	m_MirrorMtrl = d3d::WHITE_MTRL;

	m_Teapot = 0;
	m_TeapotPosition = D3DXVECTOR3(0.0f, 3.0f, -7.5f);
	m_TeapotMtrl = d3d::WHITE_MTRL;

	m_TexCube = 0;
	m_VBCube = 0;
	m_IBCube = 0;
}
//-------------------------------------------------------------------------
CStencilOne::~CStencilOne(void)
{
}
//-------------------------------------------------------------------------
bool CStencilOne::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (pDevice == 0)
	{
		return false;
	}
	m_Device = pDevice;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	//
	// Make walls have low specular reflectance - 20%.
	//

	m_WallMtrl.Specular = d3d::WHITE * 0.2f;

	//
	// Create the teapot.
	//

	//D3DXCreateTeapot(m_Device, &m_Teapot, 0);

	//
	// Create and specify geometry.  For this sample we draw a floor
	// and a wall with a mirror on it.  We put the floor, wall, and
	// mirror geometry in one vertex buffer.
	//
	//   |----|----|----|
	//   |Wall|Mirr|Wall|
	//   |    | or |    |
	//   /--------------/
	//  /   Floor      /
	// /--------------/
	//
	m_Device->CreateVertexBuffer(
		24 * sizeof(VertexStencil),
		0, // usage
		VertexStencil::FVF,
		D3DPOOL_MANAGED,
		&m_VB,
		0);

	VertexStencil* v = 0;
	m_VB->Lock(0, 0, (void**)&v, 0);

	// floor
	v[0] = VertexStencil(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = VertexStencil(-7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = VertexStencil( 7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	v[3] = VertexStencil(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = VertexStencil( 7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = VertexStencil( 7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// wall
	v[6]  = VertexStencil(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[7]  = VertexStencil(-7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[8]  = VertexStencil(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[9]  = VertexStencil(-7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[10] = VertexStencil(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[11] = VertexStencil(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Note: We leave gap in middle of walls for mirror

	v[12] = VertexStencil(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[13] = VertexStencil(2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[14] = VertexStencil(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[15] = VertexStencil(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[16] = VertexStencil(7.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[17] = VertexStencil(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// mirror
	v[18] = VertexStencil(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[19] = VertexStencil(-2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[20] = VertexStencil( 2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[21] = VertexStencil(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[22] = VertexStencil( 2.5f, 5.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[23] = VertexStencil( 2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_VB->Unlock();

	//
	// Load Textures, set filters.
	//

	D3DXCreateTextureFromFile(m_Device, s_szCheckerPath, &m_FloorTex);
	D3DXCreateTextureFromFile(m_Device, s_szBrick0Path, &m_WallTex);
	D3DXCreateTextureFromFile(m_Device, s_szIcePath, &m_MirrorTex);
	

	__CreateCubeBox();

	m_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// Lights.
	//

	D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&lightDir, &color);

	m_Device->SetLight(0, &light);
	m_Device->LightEnable(0, true);

	m_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Set Camera.
	//

	D3DXVECTOR3		pos(-10.0f, 3.0f, -15.0f);
	D3DXVECTOR3		target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);

	m_Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Set projection matrix.
	//
	// 透视投影
	//
	//
	// 
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI / 4.0f, // 45 - degree
		(float)m_nWidth / (float)m_nHeight,
		1.0f,
		1000.0f);
	m_Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}
//-------------------------------------------------------------------------
void CStencilOne::__CreateCubeBox()
{
	m_Device->CreateVertexBuffer(
		24 * sizeof(VertexStencil), 
		D3DUSAGE_WRITEONLY, // usage
		VertexStencil::FVF,
		D3DPOOL_MANAGED,
		&m_VBCube,
		0);

	VertexStencil* v;
	m_VBCube->Lock(0, 0, (void**)&v, 0);

	// build box

	// fill in the front face vertex data
	v[0] = VertexStencil(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[1] = VertexStencil(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[2] = VertexStencil( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
	v[3] = VertexStencil( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	// fill in the back face vertex data
	v[4] = VertexStencil(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[5] = VertexStencil( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[6] = VertexStencil( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	v[7] = VertexStencil(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

	// fill in the top face vertex data
	v[8]  = VertexStencil(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[9]  = VertexStencil(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = VertexStencil( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	v[11] = VertexStencil( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the bottom face vertex data
	v[12] = VertexStencil(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
	v[13] = VertexStencil( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
	v[14] = VertexStencil( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
	v[15] = VertexStencil(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

	// fill in the left face vertex data
	v[16] = VertexStencil(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[17] = VertexStencil(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[18] = VertexStencil(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[19] = VertexStencil(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// fill in the right face vertex data
	v[20] = VertexStencil( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[21] = VertexStencil( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[22] = VertexStencil( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[23] = VertexStencil( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	m_VBCube->Unlock();

	m_Device->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY, // usage
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_IBCube,
		0);

	WORD* i = 0;
	m_IBCube->Lock(0, 0, (void**)&i, 0);

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

	m_IBCube->Unlock();

	D3DXCreateTextureFromFile(m_Device,s_szBoxPath,&m_TexCube);
}
//-------------------------------------------------------------------------
void CStencilOne::Cleanup()
{
	if (m_VB)
	{
		d3d::Release<IDirect3DVertexBuffer9*>(m_VB);
		m_VB = 0;
	}
	
	if (m_FloorTex)
	{
		d3d::Release<IDirect3DTexture9*>(m_FloorTex);
		m_FloorTex = 0;
	}
	
	if (m_WallTex)
	{
		d3d::Release<IDirect3DTexture9*>(m_WallTex);
		m_WallTex = 0;
	}

	if (m_MirrorTex)
	{
		d3d::Release<IDirect3DTexture9*>(m_MirrorTex);
		m_MirrorTex = 0;
	}

	if (m_Teapot)
	{
		d3d::Release<ID3DXMesh*>(m_Teapot);
		m_Teapot = 0;
	}

	if (m_TexCube)
	{
		d3d::Release<IDirect3DTexture9*>(m_TexCube);
		m_TexCube = 0;
	}

	if (m_VBCube)
	{
		d3d::Release<IDirect3DVertexBuffer9*>(m_VBCube);
		m_VBCube = 0;
	}

	if (m_IBCube)
	{
		d3d::Release<IDirect3DIndexBuffer9*>(m_IBCube);
		m_IBCube = 0;
	}
}
//-------------------------------------------------------------------------
bool CStencilOne::Display(float timeDelta)
{
	if (m_Device == 0)
	{
		return false;
	}
	//
	// Update the scene:
	//

	static float radius = 20.0f;

	if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
		m_TeapotPosition.x -= 3.0f * timeDelta;

	if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
		m_TeapotPosition.x += 3.0f * timeDelta;

	if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
		radius -= 2.0f * timeDelta;

	if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
		radius += 2.0f * timeDelta;


	static float angle = (3.0f * D3DX_PI) / 2.0f;

	if( ::GetAsyncKeyState('A') & 0x8000f )
		angle -= 0.5f * timeDelta;

	if( ::GetAsyncKeyState('S') & 0x8000f )
		angle += 0.5f * timeDelta;

	D3DXVECTOR3 position( cosf(angle) * radius, 3.0f, sinf(angle) * radius );
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	m_Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Draw the scene:
	//
	m_Device->Clear(0, 0, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		0xff000000, 1.0f, 0L);

	m_Device->BeginScene();

	__RenderScene();

	__RenderShadow();

	__RenderMirror();	


	m_Device->EndScene();

	return true;
}
//-------------------------------------------------------------------------
void CStencilOne::__RenderScene()
{
	// draw teapot
// 	m_Device->SetMaterial(&m_TeapotMtrl);
// 	m_Device->SetTexture(0, 0);
	// draw box
	m_Device->SetMaterial(&m_TeapotMtrl);
	m_Device->SetTexture(0, m_TexCube);
	m_Device->SetStreamSource(0, m_VBCube, 0, sizeof(VertexStencil));
	m_Device->SetIndices(m_IBCube);
	m_Device->SetFVF(VertexStencil::FVF);

	// 先设置世界坐标 再渲染,否则箱子会变位置
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W,
		m_TeapotPosition.x, 
		m_TeapotPosition.y,
		m_TeapotPosition.z);

	m_Device->SetTransform(D3DTS_WORLD, &W);

	m_Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, 
		0,                  
		0,                  
		24,
		0,
		12);  

	
	//m_Teapot->DrawSubset(0);

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	m_Device->SetTransform(D3DTS_WORLD, &I);

	m_Device->SetStreamSource(0, m_VB, 0, sizeof(VertexStencil));
	m_Device->SetFVF(VertexStencil::FVF);

	// draw the floor
	m_Device->SetMaterial(&m_FloorMtrl);
	m_Device->SetTexture(0, m_FloorTex);
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	// draw the walls
	m_Device->SetMaterial(&m_WallMtrl);
	m_Device->SetTexture(0, m_WallTex);
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 6, 4);

	// draw the mirror
	m_Device->SetMaterial(&m_MirrorMtrl);
	m_Device->SetTexture(0, m_MirrorTex);
	m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
}
//-------------------------------------------------------------------------
// 渲染镜子上的物件
void CStencilOne::__RenderMirror()
{
	//
	// Draw Mirror quad to stencil buffer ONLY.  In this way
	// only the stencil bits that correspond to the mirror will
	// be on.  Therefore, the reflected teapot can only be rendered
	// where the stencil bits are turned on, and thus on the mirror 
	// only.
	//

	// 这段的意思是 渲染镜子纹理  并且在镜子上的每个像素做了标记 标记为1
	{
		// 设置使用 模版缓冲 状态为 true
		m_Device->SetRenderState(D3DRS_STENCILENABLE,    true);
		// 设置 模版缓冲规则 D3DCMP_ALWAYS 表示全部缓冲
		m_Device->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
		// 控制模版测试 制定参考值 模版掩码 以便于运算
		// 设置 模版参考值 默认为0 设置为1 好像设置 2 3 也没什么问题
		m_Device->SetRenderState(D3DRS_STENCILREF,       0x1);
		// 设置模版掩码 用来掩饰(隐藏)在ref和value中的值
		m_Device->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
		// 设置模版写掩码 掩饰我们写进模版缓存的任何值的位
		m_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
		// 倘若 像素深度测试 失败 则保持原样 这里原样为写入(所以镜子肯定全部渲染出来)
		m_Device->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
		// 倘若 像素模版测试 失败 则保持原样 这里原样为写入(所以镜子肯定全部渲染出来)
		m_Device->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
		// 倘若 像素深度测试 和 像素模版测试 都成功,则指定用 模版参考值 来替换模版缓存入口 
		m_Device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);

		// disable writes to the depth and back buffers
		// 阻止或者 允许 写 深度缓存
		m_Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
		// 打开 alpha 混合 透明度混合
		m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		// 设置混合等式的参数 这里是 FinalPixel = sourcePixel x D3DBLEND_ZERO +	DestPixel x D3DBLEND_ONE
		m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
		m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// draw the mirror to the stencil buffer
		// 渲染镜子 这个不必多说 
		m_Device->SetStreamSource(0, m_VB, 0, sizeof(VertexStencil));
		m_Device->SetFVF(VertexStencil::FVF);
		m_Device->SetMaterial(&m_MirrorMtrl);
		m_Device->SetTexture(0, m_MirrorTex);
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		m_Device->SetTransform(D3DTS_WORLD, &I);
		m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

		// re-enable depth writes
		// 阻止或者 允许 写 深度缓存
		m_Device->SetRenderState( D3DRS_ZWRITEENABLE, true );
	}
	
	// 这两句的限定了 测试必须是 上面镜子的像素才会成功.简单来说 就是一个像素标记
	// 倘若像素点 不在 镜子上(未被标记为1) 则不会加入模版缓存
	{
		// only draw reflected teapot to the pixels where the mirror
		// was drawn to.
		// 设置 模版缓存规则 D3DCMP_EQUAL 表示相等的模版参考值,保证了只渲染在镜子上
		// 如果这里改成D3DCMP_ALWAYS 箱子的反射会一直存在 不管是否脱离镜子
		m_Device->SetRenderState(D3DRS_STENCILFUNC,  D3DCMP_EQUAL);
		// 如果像素深度测试 和 像素模版测试 都成功 则保存模版缓存的值
		m_Device->SetRenderState(D3DRS_STENCILPASS,  D3DSTENCILOP_KEEP);
	}
	

	// 计算箱子  在场景中 反射位置的矩阵 
	// position reflection
	D3DXMATRIX W, T, R;
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
	D3DXMatrixReflect(&R, &plane);

	D3DXMatrixTranslation(&T,
		m_TeapotPosition.x, 
		m_TeapotPosition.y,
		m_TeapotPosition.z); 

	W = T * R;

	// clear depth buffer and blend the reflected teapot with the mirror
	// 清理 深度缓存
	// 因为被反射的 箱子 的深度 比 镜子的深度大,因此镜子的图元 将被反射箱子的图元弄模糊(遮住?)
	// 为了避免这种情况 我们清楚 深度 缓存
	m_Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	// 设置混合等式的参数 这里是 FinalPixel = sourcePixel x D3DBLEND_DESTCOLOR +	DestPixel x D3DBLEND_ZERO
	// 上面公式代表的是 箱子在渲染在 镜子像素所在的位置的时候,
	// 该位置的像素 要混合 镜子像素 和 箱子像素 这样看起来更真实.
	m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// 下面是渲染箱子 自然不必解释
	// Finally, draw the reflected teapot
	m_Device->SetTransform(D3DTS_WORLD, &W);

	m_Device->SetMaterial(&m_TeapotMtrl);
	m_Device->SetTexture(0, m_TexCube);
	m_Device->SetStreamSource(0, m_VBCube, 0, sizeof(VertexStencil));
	m_Device->SetIndices(m_IBCube);
	m_Device->SetFVF(VertexStencil::FVF);

	// 设置 背部拣选 拣选顺时针..的三角形 如果不设置这个..则镜子里渲染出来的是箱子内部...Orz
	m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, 
		0,                  
		0,                  
		24,
		0,
		12);  

// 	m_Device->SetMaterial(&m_TeapotMtrl);
// 	m_Device->SetTexture(0, 0);

	//m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//m_Teapot->DrawSubset(0);

	// Restore render states.
	// 关闭 透明度 alpha 混合
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	// 设置使用 模版缓冲 状态为 false
	m_Device->SetRenderState( D3DRS_STENCILENABLE, false);
	// 设置 背部拣选 拣选逆时针..的三角形 默认值
	m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//-------------------------------------------------------------------------
void CStencilOne::__RenderShadow()
{
	// 防止多重渲染的混合 模版缓存算法
	// 大概意思就是 先全部设置为0 然后渲染一个+1.
	// 当某像素不为0 的时候 不算法
	// 这样防止某个像素重复渲染
	// 阻止了重影 重叠 之类的情况发生 
	{
		// 设置使用 模版缓冲 状态为 true
		m_Device->SetRenderState(D3DRS_STENCILENABLE,    true);
		// 设置 模版缓冲规则 D3DCMP_EQUAL 表示需要与 模版参考值 相等
		m_Device->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_EQUAL);
		// 控制模版测试 制定参考值 模版掩码 以便于运算
		// 设置 模版参考值 默认为0 设置为0 
		m_Device->SetRenderState(D3DRS_STENCILREF,       0x0);
		// 设置模版掩码 用来掩饰(隐藏)在ref和value中的值
		m_Device->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
		// 设置模版写掩码 掩饰我们写进模版缓存的任何值的位
		m_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
		// 倘若 像素深度测试 失败 则保持原样 这里原样为写入(所以镜子肯定全部渲染出来)
		m_Device->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
		// 倘若 像素模版测试 失败 则保持原样 这里原样为写入(所以镜子肯定全部渲染出来)
		m_Device->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
		// 倘若 像素深度测试 和 像素模版测试 都成功,则指定用 模版参考值 +1,参考值为0 所以这里为1
		m_Device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCR);
	}
	
	// 通过方向光 计算在 某平面的阴影
	{
		// 光源 方向光
		D3DXVECTOR4 lightDirection(0.707f,-0.707f,0.707f,0.0f);
		// 平面
		D3DXPLANE groundPlane(0.0f,-1.0f,0.0f,0.0f);

		D3DXMATRIX S;
		// 计算影子
		D3DXMatrixShadow(&S,&lightDirection,&groundPlane);

		D3DXMATRIX T;
		D3DXMatrixTranslation(&T,m_TeapotPosition.x,m_TeapotPosition.y,m_TeapotPosition.z);

		// 计算m_TeapotPosition位置的阴影
		D3DXMATRIX W = T * S;

		m_Device->SetTransform(D3DTS_WORLD,&W);
	}
	
	// 打开 alpha 混合 透明度混合
	m_Device->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	// 设置混合等式的参数 混合透明度规则
	m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	// 即1减去当前绘制像素的alpha值
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK,d3d::BLACK,d3d::BLACK,d3d::BLACK,0.0f);
	mtrl.Diffuse.a = 0.5f;

	m_Device->SetRenderState(D3DRS_ZENABLE,false);
	m_Device->SetMaterial(&mtrl);

	m_Device->SetTexture(0, m_TexCube);
	m_Device->SetStreamSource(0, m_VBCube, 0, sizeof(VertexStencil));
	m_Device->SetIndices(m_IBCube);
	m_Device->SetFVF(VertexStencil::FVF);


	m_Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, 
		0,                  
		0,                  
		24,
		0,
		12);  

	m_Device->SetRenderState(D3DRS_ZENABLE,true);
	m_Device->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	m_Device->SetRenderState(D3DRS_STENCILENABLE,    false);
}