//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dinit.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates how to initialize Direct3D, how to use the book's framework
//       functions, and how to clear the screen to black.  Note that the Direct3D
//       initialization code is in the d3dUtility.h/.cpp files.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include "AddressModes.h"
#include "SquareTranList.h"
#include "CubeBox.h"
#include "TeaPot.h"
#include "StencilOne.h"
#include "FontFPS.h"
#include "MeshDemo1.h"
#include "MeshLoadX.h"
#include "CameraTest.h"
#include "ISubBase.h"
#include <map>
//
// Globals
//

IDirect3DDevice9* g_m_Device = 0; 
HINSTANCE g_Inst = 0;
HWND g_hwnd = 0;
int g_nWidth = 800;
int g_nHeight = 600;
int g_nSelectID = eButton_Min;
std::map<int,ISubBase*> g_mapSubBase;

//
// Framework Functions
//
void initAllSubBase()
{
	g_mapSubBase.clear();
	ISubBase* pBase = NULL;

	pBase = new CSquareTranList();
	g_mapSubBase[eButton_Square] = pBase;

	pBase = new CAddressMode();
	g_mapSubBase[eButton_Texture] = pBase;

	pBase = new CCubeBox();
	g_mapSubBase[eButton_CubeBox] = pBase;

	pBase = new CTeaPot();
	g_mapSubBase[eButton_Teapot] = pBase;

	pBase = new CStencilOne();
	g_mapSubBase[eButton_StencilOne] = pBase;

	pBase = new CFontFPS();
	g_mapSubBase[eButton_FontFPS] = pBase;

	pBase = new CMeshDemo1();
	g_mapSubBase[eButton_Mesh1] = pBase;

	pBase = new CMeshLoadX();
	g_mapSubBase[eButton_MeshX] = pBase;

	pBase = new CCameraTest();
	g_mapSubBase[eButton_Camera] = pBase;
}

bool Setup()
{
	// Nothing to setup in this sample.

	if (g_m_Device == 0)
	{
		return false;
	}

	if (g_mapSubBase.empty())
	{
		initAllSubBase();
	}
	
	std::map<int,ISubBase*>::iterator itor = g_mapSubBase.find(g_nSelectID);
	if (itor == g_mapSubBase.end())
	{
		return false;
	}

	ISubBase* pBase = itor->second;
	if (pBase)
	{
		pBase->Setup(g_m_Device,g_nWidth,g_nHeight);
	}
	

	return true;
}

void Cleanup()
{
	// Nothing to cleanup in this sample.
	if (g_mapSubBase.empty())
	{
		return;
	}

	ISubBase* pBase = NULL;
	std::map<int,ISubBase*>::iterator itor = g_mapSubBase.begin();
	for (; itor != g_mapSubBase.end(); ++itor)
	{
		pBase = itor->second;
		if (pBase)
		{
			pBase->Cleanup();
		}
		delete pBase;
		itor->second = NULL;
	}
	g_mapSubBase.clear();
}

bool Display(float timeDelta)
{
	if (g_m_Device == 0)
	{
		return false;
	}
	if (g_nSelectID <= eButton_Min || eButton_Max <= g_nSelectID)
	{
		return false;
	}

	std::map<int,ISubBase*>::iterator itor = g_mapSubBase.find(g_nSelectID);
	if (itor == g_mapSubBase.end())
	{
		return false;
	}

	if (itor->second == NULL)
	{
		return false;
	}
	itor->second->Display(timeDelta);

	D3DRECT rect;
	rect.x1 = 100;
	rect.y1 = 0;
	rect.x2 = g_nWidth;
	rect.y2 = g_nHeight;
	g_m_Device->Present((const RECT*)&rect, (const RECT*)&rect, 0, 0);

	return true;
}

void OnButtonClick(int nButtonKey,HWND hwnd)
{
	if (g_nSelectID == nButtonKey)
	{
		return;
	}

	std::map<int,ISubBase*>::iterator itor = g_mapSubBase.find(g_nSelectID);
	if (itor != g_mapSubBase.end())
	{
		itor->second->Cleanup();
	}

	g_nSelectID = nButtonKey;

	if (g_nSelectID <= eButton_Min || eButton_Max <= g_nSelectID)
	{
		return;
	}

	if (g_m_Device)
	{
		g_m_Device->Release();
		g_m_Device = 0;
	}

	if (! d3d::InitD3D(g_hwnd,g_nWidth,g_nHeight,true, D3DDEVTYPE_HAL, &g_m_Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return;
	}
	Setup();

}
//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent; 

	switch( msg )
	{
	case WM_CREATE:  
		{  
			CreateWindow("button", "正方形",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 10, 90, 30, hwnd, (HMENU)eButton_Square, g_Inst, NULL);  
			CreateWindow("button", "纹理",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 50, 90, 30, hwnd, (HMENU)eButton_Texture, g_Inst, NULL);  
			CreateWindow("button", "木箱",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 90, 90, 30, hwnd, (HMENU)eButton_CubeBox, g_Inst, NULL); 
			CreateWindow("button", "茶壶",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 130, 90, 30, hwnd, (HMENU)eButton_Teapot, g_Inst, NULL);
			CreateWindow("button", "镜子",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 170, 90, 30, hwnd, (HMENU)eButton_StencilOne, g_Inst, NULL);
			CreateWindow("button", "字体FPS",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 210, 90, 30, hwnd, (HMENU)eButton_FontFPS, g_Inst, NULL);
			CreateWindow("button", "网格1",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 250, 90, 30, hwnd, (HMENU)eButton_Mesh1, g_Inst, NULL);
			CreateWindow("button", "网格X",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 290, 90, 30, hwnd, (HMENU)eButton_MeshX, g_Inst, NULL);
			CreateWindow("button", "摄像机",  
				WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, 330, 90, 30, hwnd, (HMENU)eButton_Camera, g_Inst, NULL);

			
		}  
		break; 
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;

	case WM_COMMAND:  
		{
			wmId    = LOWORD(wParam);   
			wmEvent = HIWORD(wParam); 
			if (g_nSelectID < eButton_Min || eButton_Max < g_nSelectID)
			{
				return DefWindowProc(hwnd, msg, wParam, lParam);  
			}
			OnButtonClick(wmId,hwnd);
		}
		break;  
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	g_Inst = hinstance;
	if(!d3d::InitWindow(hinstance,g_nWidth, g_nHeight,g_hwnd))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	
	d3d::EnterMsgLoop( Display );

	Cleanup();

	if (g_m_Device)
	{
		g_m_Device->Release();
		g_m_Device = 0;
	}
	
	

	return 0;
}