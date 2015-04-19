//-------------------------------------------------------------------------
#include "MeshDemo1.h"
//-------------------------------------------------------------------------
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

CMeshDemo1::CMeshDemo1(void):m_NumSubsets(3),m_Mesh(0)
{
	m_vecTextures.clear();
}
//-------------------------------------------------------------------------
CMeshDemo1::~CMeshDemo1(void)
{
	m_vecTextures.clear();
}
//-------------------------------------------------------------------------
bool CMeshDemo1::Setup(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	if (0 == pDevice)
	{
		return false;
	}
	m_Device = pDevice;
	m_nHeight = nHeight;
	m_nWidth = nWidth;

	{
		HRESULT hr = 0;

		//
		// We are going to fill the empty mesh with the geometry of a box,
		// so we need 12 triangles and 24 vetices.
		//

		hr = D3DXCreateMeshFVF(
			12,
			24,
			D3DXMESH_MANAGED,
			Vertex::FVF,
			m_Device,
			&m_Mesh);

		if(FAILED(hr))
		{
			::MessageBox(0, "D3DXCreateMeshFVF() - FAILED", 0, 0);
			return false;
		}

		//
		// Fill in vertices of a box
		//
		// 顶点缓存区
		Vertex* v = 0;
		m_Mesh->LockVertexBuffer(0, (void**)&v);

		// fill in the front face vertex data
		v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[1] = Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[2] = Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
		v[3] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

		// fill in the back face vertex data
		v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[5] = Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[6] = Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		v[7] = Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

		// fill in the top face vertex data
		v[8]  = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		v[9]  = Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		v[10] = Vertex( 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
		v[11] = Vertex( 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

		// fill in the bottom face vertex data
		v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
		v[13] = Vertex( 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
		v[14] = Vertex( 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
		v[15] = Vertex(-1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

		// fill in the left face vertex data
		v[16] = Vertex(-1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[17] = Vertex(-1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[18] = Vertex(-1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		// fill in the right face vertex data
		v[20] = Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[21] = Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[22] = Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[23] = Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		m_Mesh->UnlockVertexBuffer();

		//
		// Define the triangles of the box
		//
		// 定义索引缓冲区
		WORD* i = 0;
		m_Mesh->LockIndexBuffer(0, (void**)&i);

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

		m_Mesh->UnlockIndexBuffer();

		//
		// Specify the subset each triangle belongs to, in this example
		// we will use three subsets, the first two faces of the cube specified
		// will be in subset 0, the next two faces will be in subset 1 and
		// the the last two faces will be in subset 2.
		//
		// 设置子集 渲染的时候按照 0 1 2来渲染即可
		DWORD* attributeBuffer = 0;
		m_Mesh->LockAttributeBuffer(0, &attributeBuffer);

		for(int a = 0; a < 4; a++)
			attributeBuffer[a] = 0;

		for(int b = 4; b < 8; b++)
			attributeBuffer[b] = 1;

		for(int c = 8; c < 12; c++)
			attributeBuffer[c] = 2;

		m_Mesh->UnlockAttributeBuffer();

		//
		// Optimize the mesh to generate an attribute table.
		//
		// 优化网格 产生相邻三角形
		std::vector<DWORD> adjacencyBuffer(m_Mesh->GetNumFaces() * 3);
		m_Mesh->GenerateAdjacency(0.0f, &adjacencyBuffer[0]);

		hr = m_Mesh->OptimizeInplace(		
			D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_COMPACT  |
			D3DXMESHOPT_VERTEXCACHE,
			&adjacencyBuffer[0],
			0, 0, 0);

		//
		// Dump the Mesh Data to file.
		//

		m_OutFile.open("MeshDump.txt");

		__dumpVertices(m_OutFile, m_Mesh);
		__dumpIndices(m_OutFile, m_Mesh);
		__dumpAttributeTable(m_OutFile, m_Mesh); 	
		__dumpAttributeBuffer(m_OutFile, m_Mesh);		
		__dumpAdjacencyBuffer(m_OutFile, m_Mesh);

		m_OutFile.close();

		//
		// Load the textures and set filters.
		//
		IDirect3DTexture9* texTemp1;
		D3DXCreateTextureFromFile(
			m_Device,
			s_szBrick0Path,
			&texTemp1);
		m_vecTextures.push_back(texTemp1);

		IDirect3DTexture9* texTemp2;
		D3DXCreateTextureFromFile(
			m_Device,
			s_szBrick1Path,
			&texTemp2);
		m_vecTextures.push_back(texTemp2);

		IDirect3DTexture9* texTemp3;
		D3DXCreateTextureFromFile(
			m_Device,
			s_szCheckerPath,
			&texTemp3);
		m_vecTextures.push_back(texTemp3);

		m_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		// 
		// Disable lighting.
		//

		m_Device->SetRenderState(D3DRS_LIGHTING, false);

		//
		// Set camera.
		//

		D3DXVECTOR3 pos(0.0f, 0.f, -4.0f);
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
	}

	return true;
}
//-------------------------------------------------------------------------
void CMeshDemo1::Cleanup()
{
	if (m_Mesh)
	{
		d3d::Release<ID3DXMesh*>(m_Mesh);
		m_Mesh = 0;
	}
	
	if (! m_vecTextures.empty())
	{
		std::vector<IDirect3DTexture9*>::iterator itor = m_vecTextures.begin();
		for (; itor != m_vecTextures.end() ; ++itor)
		{
			d3d::Release<IDirect3DTexture9*>(*itor);
		}
		m_vecTextures.clear();
	}
}
//-------------------------------------------------------------------------
bool CMeshDemo1::Display(float timeDelta)
{
	if (0 == m_Device)
	{
		return false;
	}

	{
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

		//
		// Render
		//

		m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		m_Device->BeginScene();

		int nIndex = 0;
		std::vector<IDirect3DTexture9*>::iterator itor = m_vecTextures.begin();
		for (; itor != m_vecTextures.end(); ++itor)
		{
			m_Device->SetTexture( 0, *itor );
			m_Mesh->DrawSubset( nIndex++ );
		}

		m_Device->EndScene();
	}

	return true;
}
//-------------------------------------------------------------------------
//
// Prototype Implementations
//

void CMeshDemo1::__dumpVertices(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Vertices:" << std::endl;
	outFile << "---------" << std::endl << std::endl;

	Vertex* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);
	for(int i = 0; i < mesh->GetNumVertices(); i++)
	{
		outFile << "Vertex " << i << ": (";
		outFile << v[i]._x  << ", " << v[i]._y  << ", " << v[i]._z  << ", ";
		outFile << v[i]._nx << ", " << v[i]._ny << ", " << v[i]._nz << ", ";
		outFile << v[i]._u  << ", " << v[i]._v  << ")"  << std::endl;
	}
	mesh->UnlockVertexBuffer();

	outFile << std::endl << std::endl;
}
//-------------------------------------------------------------------------
void CMeshDemo1::__dumpIndices(std::ofstream& outFile, ID3DXMesh* mesh)
{
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
void CMeshDemo1::__dumpAttributeBuffer(std::ofstream& outFile, ID3DXMesh* mesh)
{
	outFile << "Attribute Buffer:" << std::endl;
	outFile << "-----------------" << std::endl << std::endl;

	DWORD* attributeBuffer = 0;
	mesh->LockAttributeBuffer(0, &attributeBuffer);

	// an attribute for each face
	for(int i = 0; i < mesh->GetNumFaces(); i++)
	{
		outFile << "Triangle lives in subset " << i << ": ";
		outFile << attributeBuffer[i] << std::endl;
	}
	mesh->UnlockAttributeBuffer();

	outFile << std::endl << std::endl;
}
//-------------------------------------------------------------------------
void CMeshDemo1::__dumpAdjacencyBuffer(std::ofstream& outFile, ID3DXMesh* mesh)
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
void CMeshDemo1::__dumpAttributeTable(std::ofstream& outFile, ID3DXMesh* mesh)
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
