//-------------------------------------------------------------------------
#include "MeshLoadX.h"
//-------------------------------------------------------------------------


CMeshLoadX::CMeshLoadX(void)
{
	m_XFont = 0;
	m_SourceMesh = 0;
	m_ProcessMesh = 0;
	m_SphereMesh = 0;
	m_BoxMesh = 0;
	m_FaceTotal = 0;
	m_VertexTotal = 0;
	m_IndexTotal = 0;
	m_vecMtrls.clear();
	m_vecTextures.clear();
}
//-------------------------------------------------------------------------
CMeshLoadX::~CMeshLoadX(void)
{
}
//-------------------------------------------------------------------------
bool CMeshLoadX::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (0 == pDevice)
	{
		return false;
	}
	m_Device = pDevice;
	m_nHeight = nHeight;
	m_nWidth = nWidth;

	// 读取X文件并解析
	{
		HRESULT hr = 0;
		ID3DXBuffer* adjBuffer = 0;
		ID3DXBuffer* mtrlBuffer = 0;
		DWORD	numMtrls = 0;
		hr = D3DXLoadMeshFromX(
			s_szMeshFilePath,
			D3DXMESH_MANAGED,
			m_Device,
			&adjBuffer,
			&mtrlBuffer,
			0,
			&numMtrls,
			&m_SourceMesh);

		if (FAILED(hr))
		{
			::MessageBox(0,"CMeshLoadX::Setup hr is failed",0,0);
			return false;
		}

		if (mtrlBuffer != 0 && numMtrls != 0)
		{
			D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
			for (int nIndex = 0; nIndex < numMtrls; ++nIndex)
			{
				mtrls[nIndex].MatD3D.Ambient = mtrls[nIndex].MatD3D.Diffuse;
				m_vecMtrls.push_back(mtrls[nIndex].MatD3D);
				if (mtrls[nIndex].pTextureFilename != 0)
				{
					IDirect3DTexture9* pTex = 0;
					D3DXCreateTextureFromFile(m_Device,mtrls[nIndex].pTextureFilename,&pTex);
					m_vecTextures.push_back(pTex);
				}
				else
				{
					m_vecTextures.push_back(0);
				}
			}
		}

		d3d::Release<ID3DXBuffer*>(mtrlBuffer);

		hr = m_SourceMesh->OptimizeInplace(		
			D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_COMPACT  |
			D3DXMESHOPT_VERTEXCACHE,
			(DWORD*)adjBuffer->GetBufferPointer(),
			(DWORD*)adjBuffer->GetBufferPointer(), // new adjacency info
			0, 0);

		if(FAILED(hr))
		{
			::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
			return false;
		}

		hr = D3DXGeneratePMesh(
			m_SourceMesh,
			(DWORD*)adjBuffer->GetBufferPointer(),
			0,
			0,
			1,
			D3DXMESHSIMP_FACE,
			&m_ProcessMesh);

		
	

		if(FAILED(hr))
		{
			::MessageBox(0, "D3DXGeneratePMesh() - FAILED", 0, 0);
			return false;
		}

		m_ProcessMesh->SetNumFaces(m_ProcessMesh->GetMaxFaces());

		d3d::Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	}

	// 新建字体
	{
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
	}

	// 写Mesh数据文件
	{
		m_OutFile.open("MeshDumpX.txt");

		__dumpVertices(m_OutFile, m_SourceMesh);
		__dumpIndices(m_OutFile, m_SourceMesh);
		__dumpAttributeTable(m_OutFile, m_SourceMesh); 	
		__dumpAttributeBuffer(m_OutFile, m_SourceMesh);		
		__dumpAdjacencyBuffer(m_OutFile, m_SourceMesh);

		m_OutFile.close();
	}
	
	d3d::BoundingSphere boundingSphere;
	d3d::BoundingBox    boundingBox;

	__ComputeBoundingSphere(m_SourceMesh, &boundingSphere);
	__ComputeBoundingBox(m_SourceMesh, &boundingBox);

	D3DXCreateSphere(
		m_Device,
		boundingSphere._radius,
		20,
		20,
		&m_SphereMesh,
		0);

	D3DXCreateBox(
		m_Device,
		boundingBox._max.x - boundingBox._min.x,
		boundingBox._max.y - boundingBox._min.y,
		boundingBox._max.z - boundingBox._min.z,
		&m_BoxMesh,
		0);

	//
	// Set texture filters.
	//

	m_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	// 
	// Set Lights.
	//

	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);

	m_Device->SetLight(0, &light);
	m_Device->LightEnable(0, true);
	m_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Set camera.
	//

	D3DXVECTOR3 pos(4.0f, 4.0f, -20.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(
		&V,
		&pos,
		&target,
		&up);

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
void CMeshLoadX::Cleanup()
{
	if (m_XFont)
	{
		d3d::Release<ID3DXFont*>(m_XFont);
		m_XFont = 0;
	}

	if (m_SourceMesh)
	{
		d3d::Release<ID3DXMesh*>(m_SourceMesh);
		m_SourceMesh = 0;
	}

	if (m_ProcessMesh)
	{
		d3d::Release<ID3DXPMesh*>(m_ProcessMesh);
		m_ProcessMesh = 0;
	}

	if (m_SphereMesh)
	{
		d3d::Release<ID3DXMesh*>(m_SphereMesh);
		m_SphereMesh = 0;
	}

	if (m_SphereMesh)
	{
		d3d::Release<ID3DXMesh*>(m_BoxMesh);
		m_SphereMesh = 0;
	}
	
}
//-------------------------------------------------------------------------
bool CMeshLoadX::Display(float timeDelta)
{
	if (0 == m_Device)
	{
		return false;
	}

	D3DXMATRIX xRot;
	D3DXMatrixRotationX(&xRot, D3DX_PI * 0.2f);

	static float y = 0.0f;
	D3DXMATRIX yRot;
	D3DXMatrixRotationY(&yRot, y);
	y += timeDelta;

	if( y >= 6.28f )
		y = 0.0f;

	D3DXMATRIX World = xRot * yRot;

	m_Device->SetTransform(D3DTS_WORLD, &World);


	int nFace = m_ProcessMesh->GetNumFaces();
	if (::GetAsyncKeyState('A') & 0x8000f)
	{
		m_ProcessMesh->SetNumFaces(nFace + 1);
		if (nFace == m_ProcessMesh->GetNumFaces())
		{
			m_ProcessMesh->SetNumFaces(nFace + 2);
		}
	}
	if (::GetAsyncKeyState('S') & 0x8000f)
	{
		m_ProcessMesh->SetNumFaces(nFace - 1);
	}
	//
	// Render
	//

	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	m_Device->BeginScene();

	// 渲染网格数据
	{
		int nIndex = 0;
		std::vector<D3DMATERIAL9>::iterator itor = m_vecMtrls.begin();
		for (; itor != m_vecMtrls.end(); ++itor)
		{
			m_Device->SetMaterial(&(*itor));
			m_Device->SetTexture( 0, m_vecTextures[nIndex] );
			m_ProcessMesh->DrawSubset( nIndex );

			m_Device->SetMaterial(&d3d::YELLOW_MTRL);
			m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
			m_ProcessMesh->DrawSubset( nIndex );
			m_Device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
			nIndex++;
		}
	}
	
	
		// Draw bounding volume in blue and at 10% opacity
	D3DMATERIAL9 blue = d3d::RED_MTRL;
	blue.Diffuse.a = 0.50f; // 10% opacity

	m_Device->SetMaterial(&blue);
	m_Device->SetTexture(0, 0); // disable texture

	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	static bool RenderBoundingSphere = false;
	if (::GetAsyncKeyState('Q') & 0x8000f)
	{
		RenderBoundingSphere = !RenderBoundingSphere;
	}
		

	if( RenderBoundingSphere )
		m_SphereMesh->DrawSubset(0);
	else
		m_BoxMesh->DrawSubset(0);

	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	// 输出信息到屏幕上
	{
		RECT rect = {100, 0, m_nWidth, m_nHeight};
		char szBuff[200];
		memset(szBuff,200,0);
		m_FaceTotal = m_ProcessMesh->GetNumFaces();
		m_IndexTotal = m_ProcessMesh->GetNumFaces();
		m_VertexTotal = m_ProcessMesh->GetNumVertices();
		int nFace = static_cast<int>(m_FaceTotal);
		int nVertex = static_cast<int>(m_VertexTotal);
		int nIndices = static_cast<int>(m_IndexTotal);

#pragma warning ( disable: 4996 )
		sprintf(szBuff,"Face:%d,Vertex:%d,Indices:%d",nFace,nVertex,nIndices);
#pragma warning ( default : 4996 )

		m_XFont->DrawText(NULL,  szBuff,  -1,  &rect, DT_TOP | DT_LEFT, 0xffff0000);  
	}

	m_Device->EndScene();


	return true;
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//
// Prototype Implementations
//

void CMeshLoadX::__dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh)
{
	m_VertexTotal = mesh->GetNumVertices();
// 	outFile << "Vertices:" << std::endl;
// 	outFile << "---------" << std::endl << std::endl;
// 
// 	Vertex* v = 0;
// 	mesh->LockVertexBuffer(0, (void**)&v);
// 	for(int i = 0; i < mesh->GetNumVertices(); i++)
// 	{
// 		outFile << "Vertex " << i << ": (";
// 		outFile << v[i]._x  << ", " << v[i]._y  << ", " << v[i]._z  << ", ";
// 		outFile << v[i]._nx << ", " << v[i]._ny << ", " << v[i]._nz << ", ";
// 		outFile << v[i]._u  << ", " << v[i]._v  << ")"  << std::endl;
// 	}
// 	mesh->UnlockVertexBuffer();
// 
// 	outFile << std::endl << std::endl;
}
//-------------------------------------------------------------------------
void CMeshLoadX::__dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh)
{
	m_IndexTotal = mesh->GetNumFaces();

	outFile << "Indices:" << std::endl;
	outFile << "--------" << std::endl << std::endl;

	WORD* indices = 0;
	mesh->LockIndexBuffer(0, (void**)&indices);

	for(int i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle " << i << ": ";
		outFile << indices[i * 3    ] << " ";
		outFile << indices[i * 3 + 1] << " ";
		outFile << indices[i * 3 + 2] << std::endl;
	}
	mesh->UnlockIndexBuffer();

	outFile << std::endl << std::endl;
}
//-------------------------------------------------------------------------
void CMeshLoadX::__dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh)
{
	m_FaceTotal = mesh->GetNumFaces();
	outFile << "Attribute Buffer:" << std::endl;
	outFile << "-----------------" << std::endl << std::endl;

	DWORD* attributeBuffer = 0;
	mesh->LockAttributeBuffer(0, &attributeBuffer);

	// an attribute for each face
	for(int i = 0; i < m_FaceTotal; i++)
	{
		outFile << "Triangle lives in subset " << i << ": ";
		outFile << attributeBuffer[i] << std::endl;
	}
	mesh->UnlockAttributeBuffer();

	outFile << std::endl << std::endl;
}
//-------------------------------------------------------------------------
void CMeshLoadX::__dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Adjacency Buffer:" << std::endl;
	outFile << "-----------------" << std::endl << std::endl;

	// three enttries per face
	std::vector<DWORD> adjacencyBuffer(mesh->GetNumFaces() * 3);

	mesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);

	for(int i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle's adjacent to triangle " << i << ": ";
		outFile << adjacencyBuffer[i * 3    ] << " ";
		outFile << adjacencyBuffer[i * 3 + 1] << " ";
		outFile << adjacencyBuffer[i * 3 + 2] << std::endl;
	}

	outFile << std::endl << std::endl;
}
//-------------------------------------------------------------------------
void CMeshLoadX::__dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Attribute Table:" << std::endl;
	outFile << "----------------" << std::endl << std::endl;	

	// number of entries in the attribute table
	DWORD numEntries = 0;

	mesh->GetAttributeTable(0, &numEntries);

	std::vector<D3DXATTRIBUTERANGE> table(numEntries);

	mesh->GetAttributeTable(&table[0], &numEntries);

	for(int i = 0; i < numEntries; i++)
	{
		outFile << "Entry " << i << std::endl;
		outFile << "-----------" << std::endl;

		outFile << "Subset ID:    " << table[i].AttribId    << std::endl;
		outFile << "Face Start:   " << table[i].FaceStart   << std::endl;
		outFile << "Face Count:   " << table[i].FaceCount   << std::endl;
		outFile << "Vertex Start: " << table[i].VertexStart << std::endl;
		outFile << "Vertex Count: " << table[i].VertexCount << std::endl;
		outFile << std::endl;
	}

	outFile << std::endl << std::endl;
}
//-------------------------------------------------------------------------
bool CMeshLoadX::__ComputeBoundingSphere(ID3DXMesh* mesh, d3d::BoundingSphere* sphere)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&sphere->_center,
		&sphere->_radius);

	mesh->UnlockVertexBuffer();

	if( FAILED(hr) )
		return false;

	return true;
}
//-------------------------------------------------------------------------
bool CMeshLoadX::__ComputeBoundingBox(ID3DXMesh* mesh, d3d::BoundingBox*    box)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&box->_min,
		&box->_max);

	mesh->UnlockVertexBuffer();

	if( FAILED(hr) )
		return false;

	return true;
}
//-------------------------------------------------------------------------
