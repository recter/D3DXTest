//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dUtility.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides utility functions for simplifying common tasks.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include <d3dx9.h>
#include <string>

enum E_BUTTON
{
	eButton_Min = 0,
	eButton_Square,			// 三角形
	eButton_Texture,		// 纹理操作
	eButton_CubeBox,		// 箱子
	eButton_Teapot,			// 混合水壶
	eButton_StencilOne,		// 模版缓存
	eButton_FontFPS,		// 字体
	eButton_Mesh1,			// 网格
	eButton_MeshX,			// 渐进网格
	eButton_Camera,			// 摄像机操作
	eButton_Max
};

namespace d3d
{
	bool InitWindow(
		HINSTANCE hInstance,       // [in] Application instance.
		int width, int height,     // [in] Backbuffer dimensions.
		HWND &hwnd);			   // [out]The created m_Device.

	bool InitD3D(HWND hwnd,
		int width, int height,     // [in] Backbuffer dimensions.
		bool windowed,             // [in] Windowed (true)or full screen (false).
		D3DDEVTYPE m_DeviceType,     // [in] HAL or REF
		IDirect3DDevice9** m_Device);// [out]The created m_Device.

	//
	// Lights
	//

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	//
	// Materials
	//

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	int EnterMsgLoop( 
		bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg, 
		WPARAM wParam,
		LPARAM lParam);

	template<class T> void Release(T t)
	{
		if( t )
		{
			t->Release();
			t = 0;
		}
	}
		
	template<class T> void Delete(T t)
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}

	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255,255,255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0,0,0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255,0,0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0,255,0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0,0,255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255,255,0));
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0,255,255));
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255,0,255));

	const D3DMATERIAL9 WHITE_MTRL = InitMtrl(WHITE,WHITE,WHITE,BLACK,8.0f);
	const D3DMATERIAL9 RED_MTRL = InitMtrl(RED,RED,RED,BLACK,8.0f);
	const D3DMATERIAL9 GREEN_MTRL = InitMtrl(GREEN,GREEN,GREEN,BLACK,8.0f);
	const D3DMATERIAL9 BLUE_MTRL = InitMtrl(BLUE,BLUE,BLUE,BLACK,8.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW,YELLOW,YELLOW,BLACK,8.0f);

	const float INFINITY = FLT_MAX;
	const float EPSILON = 0.001f; 

	struct BoundingBox
	{
		BoundingBox();
		bool isPointInside(D3DXVECTOR3& p );
		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};

	struct BoundingSphere
	{
		BoundingSphere();
		D3DXVECTOR3 _center;
		float _radius;;
	};
}

#endif // __d3dUtilityH__