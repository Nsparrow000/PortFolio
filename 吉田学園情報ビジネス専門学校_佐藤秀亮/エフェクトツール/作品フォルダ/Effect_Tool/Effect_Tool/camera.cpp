//---------------------------
//Author:佐藤秀亮
//カメラ(camera.cpp)
//---------------------------
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "keyboard.h"

CCamera::CCamera()
{

}

CCamera::~CCamera()
{

}

//初期化処理
HRESULT CCamera::Init(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	m_camera.posV = ref;
	m_camera.posR = pos;
	m_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camera.rot = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_camera.fLength = fDistance;		//多分距離

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_camera.mtxProjection);
	//プロジェクションマトリックス作成
	D3DXMatrixPerspectiveFovLH(&m_camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 5000.0f);
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_camera.mtxProjection);
	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_camera.mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_camera.mtxView, &m_camera.posV, &m_camera.posR, &m_camera.vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);
	m_pKeyboard = CManager::GetKeyboard();
	m_pMouse = CManager::GetMouse();
	return S_OK;
}

//終了処理
void CCamera::Uninit()
{

}

//更新処理
void CCamera::Update()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

	if (m_camera.rot.y >= D3DX_PI)
	{
		m_camera.rot.y -= D3DX_PI * 2;
	}
	if (m_camera.rot.y < -D3DX_PI)
	{
		m_camera.rot.y += D3DX_PI * 2;
	}
	m_camera.posV.x = m_camera.posR.x - sinf(m_camera.rot.y) * m_camera.fLength;
	m_camera.posV.y = m_camera.posR.y + m_camera.fLength * (m_camera.rot.x + (4.0f / 10.0f));
	m_camera.posV.z = m_camera.posR.z - cosf(m_camera.rot.y) * m_camera.fLength;

	//カメラ高さ制限
	if (m_camera.rot.x >= 1.5)
	{
		m_camera.rot.x = 1.5;
	}
	else if (m_camera.rot.x <= -1.5)
	{
		m_camera.rot.x = -1.5;
	}

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_camera.mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_camera.mtxView, &m_camera.posV, &m_camera.posR, &m_camera.vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);
}

//設定処理
CCamera *CCamera::Create(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos)
{
	CCamera *pCamera;
	pCamera = new CCamera;
	if (pCamera != NULL)
	{
		pCamera->Init(ref, fDistance, pos);
	}
	return pCamera;
}
