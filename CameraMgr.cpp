//-------------------------------------------------------------------------
#include "CameraMgr.h"
//-------------------------------------------------------------------------

CCameraMgr::CCameraMgr(void)
{
	m_eCreameType = eCamera_AirCraft;

	m_Pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}
//-------------------------------------------------------------------------
CCameraMgr::CCameraMgr(E_CAMERA_TYPE eType)
{
	if (eType <= eCarema_Min || eCarema_Max <= eType)
	{
		printf("Carema Type Error");
	}
	m_eCreameType = eType;

	m_Pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_Up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}
//-------------------------------------------------------------------------
CCameraMgr::~CCameraMgr(void)
{
}
//-------------------------------------------------------------------------
// �����ƶ� ���������X�����ƶ�
void CCameraMgr::Strafe(float units)
{
	if( m_eCreameType == eCamera_LandObject )
		m_Pos += D3DXVECTOR3(m_Right.x, 0.0f, m_Right.z) * units;

	if( m_eCreameType == eCamera_AirCraft )
		m_Pos += m_Right * units;
}
//-------------------------------------------------------------------------
// �����ƶ� ���������Y�����ƶ� 
void CCameraMgr::Fly(float units)
{
	if( m_eCreameType == eCamera_LandObject )
		m_Pos.y += units;

	if( m_eCreameType == eCamera_AirCraft )
		m_Pos += m_Up * units;
}
//-------------------------------------------------------------------------
// ǰ���ƶ� ���������Z�����ƶ�
void CCameraMgr::Walk(float units)
{
	if( m_eCreameType == eCamera_LandObject )
		m_Pos += D3DXVECTOR3(m_Look.x, 0.0f, m_Look.z) * units;

	if( m_eCreameType == eCamera_AirCraft )
		m_Pos += m_Look * units;
}
//-------------------------------------------------------------------------
// ��б Χ�������Z�᷽��ת�� Ҳ����ת
void CCameraMgr::Pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &m_Right,	angle);
	D3DXVec3TransformCoord(&m_Up,&m_Up, &T);
	D3DXVec3TransformCoord(&m_Look,&m_Look, &T);
}
//-------------------------------------------------------------------------
// ƫ�� Χ�������Y�᷽��ת�� Ҳ����ת
void CCameraMgr::Yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( m_eCreameType == eCamera_LandObject )
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if( m_eCreameType == eCamera_AirCraft )
		D3DXMatrixRotationAxis(&T, &m_Up, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&m_Right,&m_Right, &T);
	D3DXVec3TransformCoord(&m_Look,&m_Look, &T);
}
//-------------------------------------------------------------------------
// ���� Χ�������X�᷽��ת�� Ҳ����ת
void CCameraMgr::Roll(float angle)
{
	// only roll for aircraft type
	if( m_eCreameType == eCamera_AirCraft )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_Look,	angle);

		// rotate _up and _right around _look vector
		D3DXVec3TransformCoord(&m_Right,&m_Right, &T);
		D3DXVec3TransformCoord(&m_Up,&m_Up, &T);
	}
}
//-------------------------------------------------------------------------
// ��ȡ��ͼ����
void CCameraMgr::GetViewMatrix(D3DXMATRIX* V)
{
	// ȷ�� look up right �����������ഹֱ �໥����
	D3DXVec3Normalize(&m_Look, &m_Look);

	D3DXVec3Cross(&m_Up, &m_Look, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	D3DXVec3Cross(&m_Right, &m_Up, &m_Look);
	D3DXVec3Normalize(&m_Right, &m_Right);

	// Build the view matrix:
	// ��� ������������ľ���
	// ȷ�� ��Ʒ��Ⱦλ�õ� X��ľ���..Y��...Z��
	float x = -D3DXVec3Dot(&m_Right, &m_Pos);
	float y = -D3DXVec3Dot(&m_Up, &m_Pos);
	float z = -D3DXVec3Dot(&m_Look, &m_Pos);

	// ��ת + ƽ��֮��������ľ���
	//|---------------------------|
	//|	Rx		Ux		Lx		0 |
	//|	Ry		Uy		Ly		0 |
	//|	Rz		Uz		Lz		0 |
	//|-p.R		-p.U	-p.L	1 |
	//|---------------------------|
	// ע�� �׵���οռ��� w = 1,��ʾ ƽ�Ʊ任����
	// w = 1 �ɱ任 ��ʾһ����
	// w = 0 ���ɱ任 ��ʾһ������
	(*V)(0,0) = m_Right.x; (*V)(0, 1) = m_Up.x; (*V)(0, 2) = m_Look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = m_Right.y; (*V)(1, 1) = m_Up.y; (*V)(1, 2) = m_Look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = m_Right.z; (*V)(2, 1) = m_Up.z; (*V)(2, 2) = m_Look.z; (*V)(2, 3) = 0.0f;
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}
//-------------------------------------------------------------------------
// �������������
void CCameraMgr::SetCameraType(E_CAMERA_TYPE eType)
{
	if (eType <= eCarema_Min || eCarema_Max <= eType)
	{
		printf("Carema Type Error");
	}

	m_eCreameType = eType;
}
//-------------------------------------------------------------------------
// ��ȡ��ǰ����
void CCameraMgr::GetPosition(D3DXVECTOR3* pos)
{
	*pos = m_Pos;
}
//-------------------------------------------------------------------------
// ���õ�ǰ����
void CCameraMgr::SetPosition(D3DXVECTOR3* pos)
{
	m_Pos = *pos;
}
//-------------------------------------------------------------------------
// ��ȡRight��������
void CCameraMgr::GetRight(D3DXVECTOR3* pos)
{
	*pos = m_Right;
}
//-------------------------------------------------------------------------
// ��ȡUp��������
void CCameraMgr::GetUp(D3DXVECTOR3* pos)
{
	*pos = m_Up;
}
//-------------------------------------------------------------------------
// ��ȡLook��������
void CCameraMgr::GetLook(D3DXVECTOR3* pos)
{
	*pos = m_Look;
}
//-------------------------------------------------------------------------
