//*************************************************************************
//	创建日期:	2015-4-19
//	文件名称:	CameraMgr.h
//  创 建 人:   Rect 	
//	版权所有:	MIT
//	说    明:	
//*************************************************************************

#pragma once

#include "d3dUtility.h"
//-------------------------------------------------------------------------
enum E_CAMERA_TYPE
{
	eCarema_Min = 0,
	eCamera_LandObject,
	eCamera_AirCraft,
	eCarema_Max,
};
//-------------------------------------------------------------------------
class CCameraMgr
{
public:
	CCameraMgr(void);
	CCameraMgr(E_CAMERA_TYPE eType);
	~CCameraMgr(void);

public:
	// 左右移动 沿着摄像机X方向移动
	void	Strafe(float units);
	// 上下移动 沿着摄像机Y方向移动 
	void	Fly(float units);
	// 前后移动 沿着摄像机Z方向移动
	void	Walk(float units);
	// 倾斜 围绕摄像机Z轴方向转动 也叫旋转
	void	Pitch(float angle);
	// 偏航 围绕摄像机Y轴方向转动 也叫旋转
	void	Yaw(float angle);
	// 滚动 围绕摄像机X轴方向转动 也叫旋转
	void	Roll(float angle);
	// 获取视图矩阵
	void	GetViewMatrix(D3DXMATRIX* V);
	// 设置摄像机类型
	void	SetCameraType(E_CAMERA_TYPE eType);
	// 获取当前坐标
	void	GetPosition(D3DXVECTOR3* pos);
	// 设置当前坐标
	void	SetPosition(D3DXVECTOR3* pos);
	// 获取Right方向向量
	void	GetRight(D3DXVECTOR3* pos);
	// 获取Up方向向量
	void	GetUp(D3DXVECTOR3* pos);
	// 获取Look方向向量
	void	GetLook(D3DXVECTOR3* pos);

private:

	E_CAMERA_TYPE	m_eCreameType;
	D3DXVECTOR3		m_Right;
	D3DXVECTOR3		m_Up;
	D3DXVECTOR3		m_Look;
	D3DXVECTOR3		m_Pos;
};

