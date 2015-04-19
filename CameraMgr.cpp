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
// 左右移动 沿着摄像机X方向移动
void CCameraMgr::Strafe(float units)
{
	if( m_eCreameType == eCamera_LandObject )
		m_Pos += D3DXVECTOR3(m_Right.x, 0.0f, m_Right.z) * units;

	if( m_eCreameType == eCamera_AirCraft )
		m_Pos += m_Right * units;
}
//-------------------------------------------------------------------------
// 上下移动 沿着摄像机Y方向移动 
void CCameraMgr::Fly(float units)
{
	if( m_eCreameType == eCamera_LandObject )
		m_Pos.y += units;

	if( m_eCreameType == eCamera_AirCraft )
		m_Pos += m_Up * units;
}
//-------------------------------------------------------------------------
// 前后移动 沿着摄像机Z方向移动
void CCameraMgr::Walk(float units)
{
	if( m_eCreameType == eCamera_LandObject )
		m_Pos += D3DXVECTOR3(m_Look.x, 0.0f, m_Look.z) * units;

	if( m_eCreameType == eCamera_AirCraft )
		m_Pos += m_Look * units;
}
//-------------------------------------------------------------------------
// 倾斜 围绕摄像机Z轴方向转动 也叫旋转
void CCameraMgr::Pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &m_Right,	angle);
	D3DXVec3TransformCoord(&m_Up,&m_Up, &T);
	D3DXVec3TransformCoord(&m_Look,&m_Look, &T);
}
//-------------------------------------------------------------------------
// 偏航 围绕摄像机Y轴方向转动 也叫旋转
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
// 滚动 围绕摄像机X轴方向转动 也叫旋转
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
// 获取视图矩阵
void CCameraMgr::GetViewMatrix(D3DXMATRIX* V)
{
	// 确保 look up right 三个向量互相垂直 相互正交
	D3DXVec3Normalize(&m_Look, &m_Look);

	D3DXVec3Cross(&m_Up, &m_Look, &m_Right);
	D3DXVec3Normalize(&m_Up, &m_Up);

	D3DXVec3Cross(&m_Right, &m_Up, &m_Look);
	D3DXVec3Normalize(&m_Right, &m_Right);

	// Build the view matrix:
	// 点乘 算出两个向量的距离
	// 确定 物品渲染位置到 X轴的距离..Y轴...Z轴
	float x = -D3DXVec3Dot(&m_Right, &m_Pos);
	float y = -D3DXVec3Dot(&m_Up, &m_Pos);
	float z = -D3DXVec3Dot(&m_Look, &m_Pos);

	// 旋转 + 平移之后算出来的矩阵
	//|---------------------------|
	//|	Rx		Ux		Lx		0 |
	//|	Ry		Uy		Ly		0 |
	//|	Rz		Uz		Lz		0 |
	//|-p.R		-p.U	-p.L	1 |
	//|---------------------------|
	// 注意 套到齐次空间中 w = 1,表示 平移变换开关
	// w = 1 可变换 表示一个点
	// w = 0 不可变换 表示一个方向
	(*V)(0,0) = m_Right.x; (*V)(0, 1) = m_Up.x; (*V)(0, 2) = m_Look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = m_Right.y; (*V)(1, 1) = m_Up.y; (*V)(1, 2) = m_Look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = m_Right.z; (*V)(2, 1) = m_Up.z; (*V)(2, 2) = m_Look.z; (*V)(2, 3) = 0.0f;
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}
//-------------------------------------------------------------------------
// 设置摄像机类型
void CCameraMgr::SetCameraType(E_CAMERA_TYPE eType)
{
	if (eType <= eCarema_Min || eCarema_Max <= eType)
	{
		printf("Carema Type Error");
	}

	m_eCreameType = eType;
}
//-------------------------------------------------------------------------
// 获取当前坐标
void CCameraMgr::GetPosition(D3DXVECTOR3* pos)
{
	*pos = m_Pos;
}
//-------------------------------------------------------------------------
// 设置当前坐标
void CCameraMgr::SetPosition(D3DXVECTOR3* pos)
{
	m_Pos = *pos;
}
//-------------------------------------------------------------------------
// 获取Right方向向量
void CCameraMgr::GetRight(D3DXVECTOR3* pos)
{
	*pos = m_Right;
}
//-------------------------------------------------------------------------
// 获取Up方向向量
void CCameraMgr::GetUp(D3DXVECTOR3* pos)
{
	*pos = m_Up;
}
//-------------------------------------------------------------------------
// 获取Look方向向量
void CCameraMgr::GetLook(D3DXVECTOR3* pos)
{
	*pos = m_Look;
}
//-------------------------------------------------------------------------
