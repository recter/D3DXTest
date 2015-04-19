//*************************************************************************
//	��������:	2015-4-19
//	�ļ�����:	CameraMgr.h
//  �� �� ��:   Rect 	
//	��Ȩ����:	MIT
//	˵    ��:	
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
	// �����ƶ� ���������X�����ƶ�
	void	Strafe(float units);
	// �����ƶ� ���������Y�����ƶ� 
	void	Fly(float units);
	// ǰ���ƶ� ���������Z�����ƶ�
	void	Walk(float units);
	// ��б Χ�������Z�᷽��ת�� Ҳ����ת
	void	Pitch(float angle);
	// ƫ�� Χ�������Y�᷽��ת�� Ҳ����ת
	void	Yaw(float angle);
	// ���� Χ�������X�᷽��ת�� Ҳ����ת
	void	Roll(float angle);
	// ��ȡ��ͼ����
	void	GetViewMatrix(D3DXMATRIX* V);
	// �������������
	void	SetCameraType(E_CAMERA_TYPE eType);
	// ��ȡ��ǰ����
	void	GetPosition(D3DXVECTOR3* pos);
	// ���õ�ǰ����
	void	SetPosition(D3DXVECTOR3* pos);
	// ��ȡRight��������
	void	GetRight(D3DXVECTOR3* pos);
	// ��ȡUp��������
	void	GetUp(D3DXVECTOR3* pos);
	// ��ȡLook��������
	void	GetLook(D3DXVECTOR3* pos);

private:

	E_CAMERA_TYPE	m_eCreameType;
	D3DXVECTOR3		m_Right;
	D3DXVECTOR3		m_Up;
	D3DXVECTOR3		m_Look;
	D3DXVECTOR3		m_Pos;
};

