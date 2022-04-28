//=============================================================================
// シーン処理 [scene.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_
#include "main.h"

//マクロ定義
#define POLYGON_SIZE (70.0f)
#define PRIORITY (7)
#define CIRCLE (float(rand() % 324) / 100.0f - float(rand() % 324) / 100.0f) //円

class CCameraPanel;

class CScene
{
public:

	//オブジェクトの種類
	typedef enum
	{
		OBJECTTYPE_NONE = 0,
		OBJECTTYPE_PLAYER,
		OBJECTTYPE_UI,
		OBJECTTYPE_EFFECT,
		OBJECTTYPE_FIELD,
		OBJECTTYPE_MAX
	} OBJTYPE;

	//当たり判定
	typedef enum
	{
		COLLISION_SPHERE = 0,
		COLLISION_PALL,
		COLLISION_SQUARE,
		COLLISION_MAX
	} COLLISION;

	CScene(int nPriority = 3);
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	CScene *GetNext() { return m_pNext[m_nPriority]; }

	static CScene *GetScene(int nPriority) { return m_pTop[nPriority]; }
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	//オブジェクトタイプ
	void SetObjType(OBJTYPE obj) { m_objType = obj; }
	OBJTYPE GetObjType() const { return m_objType; }

	D3DXVECTOR3 GetPos() const { return m_pos; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	float GetRadius() { return m_fRadius; }	//自分の判定

	COLLISION GetCollision() const { return m_Collision; }	//物の判定の種類
	void SetCollision(COLLISION collision) { m_Collision = collision; }

	//オブジェクトの最小最大
	D3DXVECTOR3 GetVtxMax() const { return m_vtxMaxModel; }
	void SetVtxMax(D3DXVECTOR3 VtxMax) { m_vtxMaxModel = VtxMax; }

	D3DXVECTOR3 GetVtxMin() const { return m_vtxMinModel; }
	void SetVtxMin(D3DXVECTOR3 VtxMin) { m_vtxMinModel = VtxMin; }

	//カメラ
	D3DXVECTOR3 GetCameraPosV() const { return m_CameraPosV; }
	void SetCameraPosV(D3DXVECTOR3 posV) { m_CameraPosV = posV; }
	D3DXVECTOR3 GetCameraPosR() const { return m_CameraPosR; }
	void SetCameraPosR(D3DXVECTOR3 posR){ m_CameraPosR = posR; }

	void SetDeath(bool bDeath) { m_bDeath = bDeath; }


	void SetRot(D3DXVECTOR3 Rot) { m_CameraPosR = Rot; }
	D3DXVECTOR3 GetRot() { return m_Rot; }

private:
	static CScene *m_pTop[PRIORITY]; //先頭のオブジェクトのポインタ
	static CScene *m_pCur[PRIORITY]; //現在(一番後ろ)のオブジェクトのポインタ
	CScene *m_pPrev[PRIORITY];       //前のオブジェクトのポインタ
	CScene *m_pNext[PRIORITY];       //後ろのオブジェクトのポインタ

	static int m_nNumAll;
	static bool m_bKey;
	D3DXVECTOR3 m_pos;
	bool m_bDeath;
	float m_fRadius;
	COLLISION m_Collision;
	D3DXVECTOR3 m_vtxMinModel; //モデルの最小値
	D3DXVECTOR3 m_vtxMaxModel; //モデルの最大値
	int m_nSlack;
	D3DXVECTOR3 m_CameraPosV;
	D3DXVECTOR3 m_CameraPosR;

	int m_nPriority;//優先順位

	OBJTYPE m_objType;							//オブジェクトタイプ
	D3DXVECTOR3 m_Rot;

protected:
	void Release();
};
#endif