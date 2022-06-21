//=============================================
//プレイヤーのクラス
//=============================================
#include "player.h"
#include "keyboard.h"
#include "manager.h"
#include "camera.h"
#include "Renderer.h"
#include "sound.h"
#include "directinput.h"
#include "XInput.h"
#include "mouse.h"
#include "Enemy.h"
#include "layer_structure.h"
#include "motion.h"
#include "sword.h"
#include "model.h"
#include "game.h"
#include "gauge.h"
#include "gaugeber.h"
#include "rushattack.h"
#include "PresetSetEffect.h"
#include "blackhole.h"
#include "guard.h"
#include "PresetSetEffect.h"
#include "smallscore.h"
#include "missile.h"
#include "assistcrystal.h"
#include "savedata.h"

#define PLAYER_MOVE_SPEED (6.0f)//移動量
#define PLAYER_ROCK_LENGTH (500.0f)//ロックオン可能範囲
#define PLAYER_ATTACK_SPEED (15.0f)		//攻撃の移動速度
#define EFFECT_PLAYER_POS_Y (60.0f)		//エフェクト出す高さ
static const float MouseCameraLimit_X = 600.0f;//マウスの位置をカメラの回転速度に使用するための制限値
static const float MouseCameraLimit_Y = 200.0f;//マウスの位置をカメラの回転速度に使用するための制限値
static const float PlayerMoveSpeed = 10.0f;//プレイヤーの移動スピード
static const float CameraLimit = 10.0f;//慣性の制限値
static const float PlayerFacingUp = -100.0f;//プレイヤーの前に注視点になるようにする値
static const float RevertTime = 100.0f;
static const float AttackMoveSpeed = 0.0f;
static const float AttackMoveTime = 20.0f;
static const float AttackWaitTime = 35.0f;
static const float ComboWaitTime = 10.0f;
static const float PlayerPower = 5.0f;
static const float MaxHP = 1000.0f;
static const float MaxExp = 2.0f;
static const float PossibleRange = 300.0f;

static const int RushStartTime = 30;
static const int BlackHoleShotTime= 200;
static const int GuardMax = 20;
static const int MissileCntMax = 200;

static bool s_bCursor = true;

CPlayer::CPlayer(OBJTYPE nPriority) :CCharacter(nPriority)
{
	memset(m_pAssistCrystal, NULL, sizeof(m_pAssistCrystal));
	m_nMovePush = false;
	m_bNearEnemy = false;
	m_IsCharacterDraw = true;
	m_bMoveStop = false;
	m_motionType = N_NEUTRAL;
	m_motionLastType = N_NEUTRAL;
	m_bCanAttack = true;
	m_pSword = nullptr;
	m_fMoveSpeed = PlayerMoveSpeed;
	m_fPower = PlayerPower;
	m_fPowerDiameter = 1.0f;
	m_fAutoHeel = 0.0f;
	m_nLevel = 1;
	m_fMaxExpDiameter = 1.0f;
	m_fMaxExp = MaxExp;
	m_bCanAutoHeel = false;
	m_nTimer = 0;
	m_bCanRushAttack = false;
	m_nRushStartCnt = 0;
	m_nBlackHoleCnt = BlackHoleShotTime;
	m_bCanBlackHole = false;
	m_pDamegeGuard = nullptr;
	m_nNumGuard = GuardMax;
	m_bCanDamegeGuard = false;
	m_bCanDamege = true;
	m_nMissileCnt = MissileCntMax;
	m_bCanMissile = false;
	m_pMissile = nullptr;
	m_bCanAssistCrystal = false;
	m_BlackHoleLevel = -1;
	m_ATKupLevel = -1;
	m_HealLevel = -1;
	m_OverHealLevel = -1;
	m_SheildLevel = -1;
	m_BeamLevel = -1;
	m_RocketLevel = -1;
	m_RushAttackLevel = -1;

}

CPlayer::~CPlayer()
{
}
//----------------------------------------------
//インスタンス生成
//----------------------------------------------
CPlayer *CPlayer::Create()
{
	CPlayer *pPlayer = NULL;
	pPlayer = new CPlayer(OBJTYPE_PLAYER);
	pPlayer->Init();
	return pPlayer;
}
//-----------------------------------------------
//初期化
//---------------------------------------------
HRESULT CPlayer::Init()
{
	//体力の設定
	m_fHitPoint = MaxHP;

	//階層構造の設定
	m_pLayer = new CLayer_Structure;
	m_pLayer->SetLayer_Structure("data/TEXT/PlayerParts000.txt", m_pParts, CModel::TYPE_PLAYER);
	m_fRadius = m_pParts[0]->GetMaxPos().x *3.0f;
	//モーションの設定
	CMotion *Securement = {};
	m_pMotion = new CMotion;
	m_pMotion->MotionLoad("data/TEXT/PlayerMotion.txt");
	//武器の生成
	WeaponSet("data/TEXT/PlayerParts000.txt");



	return S_OK;
}
//-----------------------------------------------
//終了
//---------------------------------------------
void CPlayer::Uninit()
{
	if (m_pMissile)
	{
		m_pMissile->Uninit();
		m_pMissile = nullptr;
	}
	for (int nCnt = 0; nCnt < MAX_ASSISTCRYSTAL; nCnt++)
	{
		if (m_pAssistCrystal[nCnt])
		{
			m_pAssistCrystal[nCnt]->Uninit();
			m_pAssistCrystal[nCnt] = nullptr;
		}
	}


	CCharacter::Uninit();

}

//-----------------------------------------------
//更新
//---------------------------------------------
void CPlayer::Update()
{
	if (!m_IsDeath)
	{
		CInputKeyBoard *Key = CManager::GetInputKeyboard();

		if (Key->GetTrigger(DIK_F1) == true)
		{
			s_bCursor = s_bCursor ? false : true;
		}
		if (s_bCursor)
		{
			//画面の中心にマウスを設置
			RECT rec;
			HWND hDeskWnd = GetForegroundWindow();
			GetWindowRect(hDeskWnd, &rec); //デスクトップのハンドルからその(画面の)大きさを取得

			SetCursorPos(rec.right / 2, rec.bottom / 2);
			//マウスのカメラ操作
			MouseCameraCtrl();

		}

		//剣の処理の更新
		m_pSword->Update();

		//敵が近くにいるかを算出
		m_bNearEnemy = IsNearEnemyState();
		EachSkillManager();
		//敵との当たり判定
		CScene *pScene_Enemy = CScene::GetScene(OBJTYPE_ENEMY);
		while (pScene_Enemy != NULL)
		{
			if (pScene_Enemy != NULL)
			{
				CEnemy *pEnemy = (CEnemy*)pScene_Enemy;
				D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
				int nSize = pEnemy->GetParts().size();
				if (nSize != 0)
				{
					//当たり判定
					float fRadius = pEnemy->GetParts(0)->GetMaxPos().x;

					//敵に近いかどうかを判定する
					bool bNear = IsNear(EnemyPos, fRadius);
					pEnemy->SetIsNear(bNear);
					if (bNear)
					{

						if (pEnemy->GetRushAttack())
						{
							//追撃する
							CRushAttack::Create(EnemyPos, pEnemy->GetRot(), pEnemy);
							//追撃しないようにする
							pEnemy->SetRushAttack(false);
						}
						if (IsCollision(&m_pos, EnemyPos, fRadius, m_fMoveSpeed))
						{
							break;
						}
						else
						{
							m_fMoveSpeed = PlayerMoveSpeed;
						}

					}
				}

			}
			pScene_Enemy = pScene_Enemy->GetSceneNext(pScene_Enemy);
		}
		IsModelCollision(true);
		//キーボードの移動
		if (!m_bMoveStop)
		{
			KeyboardMove();
		}

		//攻撃操作
		AttackCtrl();

		//プレイヤーの状態で処理を変える
		switch (m_StateType)
		{
		case ATTACK:
			Attack();
			//攻撃モーションの再生が終わったら
			if (m_pMotion->IsMotionEnd() && !m_bAttackWait)
			{
				//移動可能にする
				m_bMoveStop = false;
				//コンボ数を０にする
				m_nComboType = COMBOWAIT;
				//モーションを待機状態に戻す
				m_motionType = N_NEUTRAL;

				if (m_pSword)
				{
					//武器の当たり判定をオフにする
					m_pSword->SetCanHit(false);
				}
			}

			break;
		}

		{
			int nSize = m_pParts.size();
			//モーションの再生
			m_pMotion->MotionTest(nSize, &m_pParts[0], &m_motionType, &m_motionLastType);

		}

	}
	else
	{

	}
}
//-----------------------------------------------
//描画
//---------------------------------------------
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

	D3DXMATRIX mtxRotModel, mtxTransModel;//計算用マトリックス

	//各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを設定
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	//向きを反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	//位置を設定
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	//位置を反映
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	//各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデル生成
	if (m_IsCharacterDraw)
	{
		int nSize = m_pLayer->GetMaxParts();
		for (int nCntParts = 0; nCntParts < nSize; nCntParts++)
		{
			m_pParts[nCntParts]->Draw();
		}
	}
	if (m_bCanAssistCrystal)
	{
		for (int nCnt = 0; nCnt < MAX_ASSISTCRYSTAL; nCnt++)
		{
			if (m_pAssistCrystal[nCnt])
			{
				m_pAssistCrystal[nCnt]->Draw();
			}
		}
	}

	//武器の描画
	if (m_pSword)
	{
		m_pSword->Draw();
	}

	//前回の位置情報の保存
	m_lastpos = m_pos;

	//プレイヤーの位置にカメラを追従させる
	FollowingPlayerCamera();
	//Drawtext();
}
//-----------------------------------------------
//移動の処理(キーボード)
//---------------------------------------------
void CPlayer::KeyboardMove()
{
	//キーボードの取得
	CInputKeyBoard *Key = CManager::GetInputKeyboard();
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	//移動ボタンを離したら待機モーションに戻す
	if (!IsMoveKeyCheck())
	{
		//待機モーションにする
		StateChange(IDLE);
		m_nMovePush = false;
		m_motionType = N_NEUTRAL;

	}
	else
	{
		//移動状態にする
		StateChange(MOVE);
		m_nMovePush = true;
		m_motionType = N_MOVE;
		m_bCanAttack = true;


	}
	//テストキー
	if (Key->GetTrigger(DIK_R) == true)
	{
		m_bCanMissile = true;
		m_RocketLevel++;
		if (m_RocketLevel >= CMissile::Level_MAX)
		{
			m_RocketLevel = CMissile::Level_MAX;
		}

	}
	//前に進む
	if (Key->GetPress(DIK_A) == true)
	{
		//向きを変える
		m_rot.y = pCamera->GetRot().x + (D3DX_PI / 2);
		//右に進む
		if (Key->GetPress(DIK_S) == true)
		{
			m_rot.y = pCamera->GetRot().x + (D3DX_PI / 4);
		}
		//左に進む
		else if (Key->GetPress(DIK_W) == true)
		{
			m_rot.y = pCamera->GetRot().x + ((D3DX_PI * 3) / 4);
		}
	}

	//後ろに進む
	if (Key->GetPress(DIK_D) == true)
	{
		//向きを変える
		m_rot.y = pCamera->GetRot().x + ((D3DX_PI * 3) / 2);
		//右に進む
		if (Key->GetPress(DIK_S) == true)
		{
			m_rot.y = pCamera->GetRot().x + ((D3DX_PI * 7) / 4);
		}
		//左に進む
		else if (Key->GetPress(DIK_W) == true)
		{
			m_rot.y = pCamera->GetRot().x + ((D3DX_PI * 5) / 4);
		}
	}

	//右に進む
	if (Key->GetPress(DIK_S) == true)
	{
		//向きを変える
		m_rot.y = pCamera->GetRot().x + 0.0f;
		//前に進む
		if (Key->GetPress(DIK_A) == true)
		{
			m_rot.y = pCamera->GetRot().x + (D3DX_PI / 4);
		}
		//後ろに進む
		else if (Key->GetPress(DIK_D) == true)
		{
			m_rot.y = pCamera->GetRot().x + ((D3DX_PI * 7) / 4);
		}
	}
	//左に進む
	if (Key->GetPress(DIK_W) == true)
	{
		//向きを変える
		m_rot.y = pCamera->GetRot().x + D3DX_PI;
		//前に進む
		if (Key->GetPress(DIK_A) == true)
		{
			m_rot.y = pCamera->GetRot().x + ((D3DX_PI * 3) / 4);
		}
		//左に進む
		else if (Key->GetPress(DIK_D) == true)
		{
			m_rot.y = pCamera->GetRot().x + ((D3DX_PI * 5) / 4);
		}
	}



	//移動できる状態になったら
	if (m_nMovePush)
	{
		m_pos.x -= (sinf(m_rot.y))*m_fMoveSpeed;
		m_pos.z -= (cosf(m_rot.y))*m_fMoveSpeed;
		m_fSoundInterval += 0.1f;
		if (m_fSoundInterval >= 1.3f)
		{
			m_fSoundInterval = 0.0f;
			//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		}
	}
}
//-----------------------------------------------
//移動の処理(ゲームパッド)
//-----------------------------------------------
void CPlayer::PadMove()
{
	float fLength = 0.0f;

	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();

	//前に進む

	if ((float)GamePad.lX >= MAX_DEAD_ZOON || (float)GamePad.lY >= MAX_DEAD_ZOON ||
		(float)GamePad.lX <= -MAX_DEAD_ZOON || (float)GamePad.lY <= -MAX_DEAD_ZOON)
	{
		//移動モーションにする
		StateChange(MOVE);
		//スティックの傾きの長さを求める
		fLength = (float)sqrt(GamePad.lX * GamePad.lX + GamePad.lY * GamePad.lY);
		fLength = fLength / 1000.f;
		float fRot = atan2f((float)GamePad.lX, (float)GamePad.lY);
		m_rot.y = -(fRot - pCamera->GetRot().x);
		m_rot.x -= (sinf(m_rot.y)*PLAYER_MOVE_SPEED)*fLength;
		m_pos.z -= (cosf(m_rot.y)*PLAYER_MOVE_SPEED)*fLength;
		m_fSoundInterval += 0.1f;
		if (m_fSoundInterval >= 1.3f)
		{
			m_fSoundInterval = 0.0f;
			//CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WALK);
		}

	}
	else
	{
		//待機モーションに戻る
		StateChange(IDLE);

		m_fSoundInterval = 1.3f;

		//CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);

	}
}
//-----------------------------------------------
//攻撃の処理(stateがAttackなら呼ばれる)
//-----------------------------------------------
void CPlayer::Attack()
{
	//攻撃モーションにする
	switch (m_nComboType)
	{
	case COMBO_1:
		m_fAttackWaitTime = 5.0f;
		m_motionType = N_ATTACK_1;
		break;
	case COMBO_2:
		m_motionType = N_ATTACK_2;
		m_fAttackWaitTime = 8.0f;

		break;
	case COMBO_3:
		m_motionType = N_ATTACK_3;
		break;
	}
	//キー数が３になったら攻撃可能にする
	if (m_pMotion->GetMotionKey() >= 3 && !m_bAttackWait)
	{
		if (m_pSword)
		{
			//武器の当たり判定をオフにする
			m_pSword->SetCanHit(false);
		}
		if (FixedTimeInterval(m_fAttackWaitTime))
		{
			m_bCanAttack = true;
		}
	}
	//攻撃するときに移動する
	if (m_bCanAttackMove)
	{
		m_fAttackMoveTime++;
		if (m_fAttackMoveTime > AttackMoveTime)
		{
			m_fAttackMoveTime = 0.0f;
			//攻撃時に移動を制限
			m_bCanAttackMove = false;
		}
		//攻撃時に移動
		AttackMove(AttackMoveSpeed);
	}

	//最大コンボから最初のコンボに戻るときの待機
	if (m_bAttackWait)
	{

		if (m_pMotion->GetMotionKey() >= 3)
		{
			if (m_pSword)
			{
				//武器の当たり判定をオフにする
				m_pSword->SetCanHit(false);
			}

			if (FixedTimeInterval(AttackWaitTime))
			{
				m_bAttackWait = false;
				m_bCanAttack = true;
				m_nComboType = COMBOWAIT;
				m_bMoveStop = false;
			}
		}
	}
	//剣の当たり判定を攻撃できるようにする

}
//-----------------------------------------------
//通常攻撃の操作の処理
//-----------------------------------------------
void CPlayer::AttackCtrl()
{
	//DirectInputのゲームパッドの取得
	CDirectInput *pGamePad = CManager::GetDirectInput();
	//ゲームパッドのボタン情報の取得
	DIJOYSTATE2 GamePad = pGamePad->GetJoyState();
	//マウス情報の取得
	CMouse *pMouse = CManager::GetMouse();

	//攻撃できる状態なら攻撃操作を可能にする
	if (m_bCanAttack)
	{
		if (pGamePad->GetButtonTrigger(CDirectInput::B) == true || pMouse->GetTrigger(CMouse::MOUSE_LEFT) == true)
		{
			//現在のキー数を０にする
			if (m_pMotion)
			{
				m_pMotion->SetNumKey(0);
			}
			//攻撃状態にする
			StateChange(ATTACK);

			//移動を制限する
			m_bMoveStop = true;

			//これ以上攻撃できないようにする
			m_bCanAttack = false;

			//攻撃するとき少し移動できるようにする
			m_bCanAttackMove = true;

			//プレイヤーを敵の方向に向かせる
			if (m_pNearEnemy != nullptr&&m_bNearEnemy == true)
			{
				NearEnemyFace();
			}

			m_nComboType++;

			if (m_pSword)
			{
				//武器の当たり判定をオンにする
				m_pSword->SetCanHit(true);
			}
			if (m_pSword)
			{
				m_pSword->SetCombo(m_nComboType);
				//武器の当たり判定をオンにする
				m_pSword->SetCanHit(true);
			}
			//最大コンボに行ったら
			if (m_nComboType >= COMBO_3)
			{
				//最大コンボから最初のコンボに戻るときの待機する
				m_bAttackWait = true;
				
			}

			//CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WALK);
			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SWORD_ATTACK);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_SWORD_ATTACK, 0.5f);

		}

	}
}
//-----------------------------------------------
//マウスのカメラ操作
//-----------------------------------------------
void CPlayer::MouseCameraCtrl()
{

	CMouse *pMouse = CManager::GetMouse();
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	D3DXVECTOR3 PosV = pCamera->GetPosV();
	D3DXVECTOR3 PosR = pCamera->GetPosR();
	D3DXVECTOR3 Rot = pCamera->GetRot();
	float fLong = 0.0f;
	float fGetLong = pCamera->GetLong();
	fLong = (float)pMouse->GetMousePos().lZ / 5.0f*-1;


	//カメラの回転
	Rot.y += (float)pMouse->GetMousePos().lY / MouseCameraLimit_X;
	Rot.x += (float)pMouse->GetMousePos().lX / MouseCameraLimit_Y;


	if (Rot.y > -0.1f)
	{
		Rot.y = -0.1f;
	}
	else if (Rot.y < -1.5f)
	{
		Rot.y = -1.5f;
	}
	if (Rot.x > D3DX_PI)
	{
		Rot.x = -D3DX_PI;
	}
	else if (Rot.x < -D3DX_PI)
	{
		Rot.x = D3DX_PI;
	}

	pCamera->SetLong(fLong);
	if (fGetLong <= 10.0f)
	{
		pCamera->SetLong(-fGetLong);
	}

	PosV.x = PosR.x + sinf(Rot.y)* sinf(Rot.x) * pCamera->GetLong();
	PosV.y = PosR.y + cosf(Rot.y)			   * pCamera->GetLong();
	PosV.z = PosR.z + sinf(Rot.y)* cosf(Rot.x) * pCamera->GetLong();

	pCamera->SetPosR(PosR);
	pCamera->SetPosV(PosV);
	pCamera->SetRot(Rot);

}
//-----------------------------------------------
//敵に近いかの判定
//-----------------------------------------------
bool CPlayer::IsNear(D3DXVECTOR3 EnemyPos, float Radius)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//自分と相手のベクトル
	vec = EnemyPos - m_pos;
	float fLength = 0.0f;
	//相手と自分の距離を求める
	fLength = sqrtf((vec.z*vec.z) + (vec.x*vec.x));
	float fCollisionRadius = PossibleRange + Radius;
	//相手と自分の距離が自分の当たり判定の大きさより小さくなったら
	if (fLength <= fCollisionRadius)
	{
		return true;
	}
	return false;

}
//-----------------------------------------------
//近い敵の方向にプレイヤーを向かせる
//-----------------------------------------------
void CPlayer::NearEnemyFace()
{
	D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetPos();
	D3DXVECTOR3 EnemyVec = EnemyPos - m_pos;			//敵とプレイヤーのベクトル
														//プレイヤーと敵の向きを求める
	float fAng = atan2(EnemyVec.x, EnemyVec.z);//敵の向き
	m_rot.y = fAng + D3DX_PI;

}
//-----------------------------------------------
//レベルアップの処理
//-----------------------------------------------
void CPlayer::LevelUp(int nType)
{
	//レベルを加算する
	m_fMaxExp *= 1.1f;
	int nEXP_B = m_nLevel * 15;
	m_fMaxExp = (m_fMaxExp + nEXP_B) / 2;
	m_nLevel += 1;

	//一度取得した判定にする
	CSaveData::FetchSetHaveSkill(nType);

	CManager::GetGame()->GetExpGauge()->GetGaugeBer(0)->SetGaugeValueMax(m_fMaxExp);
	switch (nType)
	{
	case ATKup:
		//攻撃力を上げる
		m_fPowerDiameter += 0.1f;
		m_fPower = PlayerPower * m_fPowerDiameter;
		if (m_pSword)
		{
			//剣に攻撃力を反映
			m_pSword->SetPower(m_fPower);

			CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_STATUSUP);
			CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_STATUSUP, 0.6f);
		}

		//エフェクト
		CPresetEffect::SetEffect3D(2, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {},{} ,{});	//回るやつ
		CPresetEffect::SetEffect3D(3, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {},{} ,{});	//散らすやつ
		CPresetEffect::SetEffect3D(4, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {},{} ,{});	//大きくなるだけのやつ

		break;
	case Heal:
		//オートヒール設定をオンにする
		m_bCanAutoHeel = true;
		m_HealLevel++;
		if (m_HealLevel <= MAX_HEAL_LEVEL)
		{
			m_fAutoHeel += 10.0f;
		}
		else
		{
			m_HealLevel = MAX_HEAL_LEVEL;
		}

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_STATUSUP);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_STATUSUP, 0.6f);

		break;
	case OverHeal:
		CManager::GetGame()->GetHPGauge()->SetGauge(-MaxHP,0);
		CManager::GetGame()->GetHPGauge()->ResetGauge(1);
		CManager::GetGame()->GetHPGauge()->SetGauge(-MaxHP/2.0f, 1);


		//エフェクト
		CPresetEffect::SetEffect3D(8, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {},{}, {});	//回るやつ
		CPresetEffect::SetEffect3D(9, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {},{}, {});	//散らすやつ
		CPresetEffect::SetEffect3D(10, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {}, {}, {});	//大きくなるだけのやつ

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_STATUSUP);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_STATUSUP, 0.6f);

		break;
	case Sheild:

		CPresetEffect::SetEffect3D(5, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {},{},{});	//回るやつ
		CPresetEffect::SetEffect3D(6, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {},{},{});	//散らすやつ
		CPresetEffect::SetEffect3D(7, D3DXVECTOR3(m_pos.x, EFFECT_PLAYER_POS_Y, m_pos.z), {},{},{});	//大きくなるだけのやつ
																								//ダメージガード生成
		m_bCanDamegeGuard = true;
		m_bCanDamege = false;
		m_SheildLevel++;
		if (m_pDamegeGuard)
		{
			m_pDamegeGuard->Uninit();
			m_pDamegeGuard = nullptr;
		}
		if (m_SheildLevel >= CGuard::Level_MAX)
		{
			m_SheildLevel = CGuard::Level_MAX;
		}

		if (!m_pDamegeGuard)
		{
			//ダメージガードの生成
			m_pDamegeGuard = CGuard::Create(m_pos, m_pParts[0]->GetPos().y, m_SheildLevel);
			//現在のダメージガードの個数をマックス地にする
			m_nNumGuard = m_pDamegeGuard->GetMaxGuard();
		}

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_GET, 0.6f);

		break;
	case Beam:
		m_bCanAssistCrystal = true;
		m_BeamLevel++;
		for (int nCnt = 0; nCnt < MAX_ASSISTCRYSTAL; nCnt++)
		{
			if (!m_pAssistCrystal[nCnt])
			{
				m_pAssistCrystal[nCnt] = CAssistCrystal::Create({ 0.0f,100.0,0.0f }, { 0.0,D3DXToRadian(180 + (180*nCnt)),0.0 }, this, m_BeamLevel);
			}
		}
		if (m_BeamLevel >= CAssistCrystal::Level_MAX)
		{
			m_BeamLevel = CAssistCrystal::Level_MAX;
		}

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_GET, 0.6f);

		break;
	case BlackHole:
		m_bCanBlackHole = true;
		m_BlackHoleLevel++;
		if (m_BlackHoleLevel >= CBlackHole::Level_MAX)
		{
			m_BlackHoleLevel = CBlackHole::Level_MAX;
		}

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_GET, 0.6f);

		break;
	case Rocket:
		m_bCanMissile = true;
		m_RocketLevel++;
		if (m_RocketLevel >= CMissile::Level_MAX)
		{
			m_RocketLevel = CMissile::Level_MAX;
		}

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_GET, 0.6f);

		break;
	case RushAttack:
		m_bCanRushAttack = true;
		m_RushAttackLevel++;
		if (m_RushAttackLevel >= CRushAttack::Level_MAX)
		{
			m_RushAttackLevel = CRushAttack::Level_MAX;
		}

		CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GET);
		CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_GET, 0.6f);

		break;
	}
}
//-----------------------------------------------
//敵の攻撃が当たった時のガードが壊れる処理
//-----------------------------------------------
void CPlayer::HitDamege(int nPower)
{
	//プレイヤーの体力を減らす
	if (m_bCanDamege)
	{
		CGauge *pGauge = CManager::GetGame()->GetHPGauge();
		float fSecondHP = pGauge->GetGaugeBer(1)->GetGaugeValue();
		if (fSecondHP > 0)
		{
			pGauge->SetGauge(nPower, 1);
		}
		else
		{
			pGauge->SetGauge(nPower, 0);
		}
		CSmallScore::Create({ m_pos.x,m_pParts[2]->GetMaxPos().y + 30.0f,m_pos.z }, { 10.0f,20.0f,0.0f }, { 1.0f, 0.6f, 0.6f, 0.0f }, nPower);

	}
	else
	{
		if (m_bCanDamegeGuard && m_nNumGuard != 0)
		{
			if (m_pDamegeGuard)
			{
				m_nNumGuard--;

				if (m_nNumGuard <= 0)
				{
					m_nNumGuard = 0;
					m_bCanDamegeGuard = false;
					m_bCanDamege = true;
				}
				if (m_pDamegeGuard)
				{
					//ダメージガードの数を減らす
					m_pDamegeGuard->SetGuardQuantity(m_nNumGuard);

					CManager::GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_SHEILD);
					CManager::GetSound()->ControllVoice(CSound::SOUND_LABEL_SE_SHEILD, 0.6f);
				}

			}
		}
		if (m_nNumGuard <= 0)
		{
			if (m_pDamegeGuard)
			{
				m_pDamegeGuard->Uninit();
				m_pDamegeGuard = nullptr;

			}

		}

	}
}
D3DXVECTOR3 CPlayer::GetNearEnemyPos()
{
	if (m_pNearEnemy)
	{
		int nSize = m_pNearEnemy->GetParts().size();
		if (nSize != 0)
		{
			return m_pNearEnemy->GetPos();
		}

	}
	return{ 0.0f,0.0f,0.0f };

}
//-----------------------------------------------
//今敵の近くかを算出
//-----------------------------------------------
bool CPlayer::IsNearEnemyState()
{
	bool bNearEnemy = false;
	CScene *pScene_E = CScene::GetScene(CScene::OBJTYPE_ENEMY);//一番最初の敵
	float fLength = 0.0f;//プレイヤーと敵の間の距離
	float fLength2 = 0.0f;//プレイヤーと敵の間の距離

						  //一番近い敵の情報の保存用
	m_pNearEnemy = (CEnemy*)pScene_E;
	//一番近い敵を求める
	while (pScene_E != NULL)
	{
		//今の敵の距離の長さを求める

		if (m_pNearEnemy != NULL)
		{
			D3DXVECTOR3 EnemyPos = m_pNearEnemy->GetPos();
			D3DXVECTOR3 EnemyVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			EnemyVec = EnemyPos - m_pos;			//敵とプレイヤーのベクトル
			fLength = sqrtf((EnemyVec.z*EnemyVec.z) + (EnemyVec.x*EnemyVec.x));
		}
		if (pScene_E != NULL)
		{
			//次の敵の距離の長さを求める
			CScene *pSceneNext = pScene_E->GetSceneNext(pScene_E);
			if (pSceneNext != NULL)
			{
				CEnemy *pEnemyNext = (CEnemy*)pSceneNext;
				D3DXVECTOR3 EnemyPosNext = pEnemyNext->GetPos();
				D3DXVECTOR3 EnemyVecNext = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				EnemyVecNext = EnemyPosNext - m_pos;			//敵とプレイヤーのベクトル
				fLength2 = sqrtf((EnemyVecNext.z*EnemyVecNext.z) + (EnemyVecNext.x*EnemyVecNext.x));
				//敵との距離が近かったらロックオンできるようにする
				if (fLength2 < PLAYER_ROCK_LENGTH)
				{
					bNearEnemy = true;
				}

				//今の敵が次の敵より遠かったら
				if (fLength > fLength2)
				{
					m_pNearEnemy = pEnemyNext;
				}
			}
		}
		pScene_E = pScene_E->GetSceneNext(pScene_E);
	}

	return bNearEnemy;
}
//-----------------------------------------------
//プレイヤーにカメラを追従させる処理
//-----------------------------------------------
void CPlayer::FollowingPlayerCamera()
{
	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	D3DXVECTOR3 PosR = pCamera->GetPosR();
	//キーボードの取得
	CInputKeyBoard *Key = CManager::GetInputKeyboard();

	{
		//カメラの注視点とプレイヤーの位置の差分
		float PosDifference_X = PosR.x - m_pos.x;

		//プレイヤーの位置に戻す速度
		float RevertSpeed = abs(PosDifference_X / CameraLimit);
		//RevertSpeedが0.0f出はなかったら
		if (RevertSpeed != 0.0f)
		{
			//差分が正の数なら負の数に戻すようにする
			if (PosDifference_X > 0)
			{
				PosR.x -= RevertSpeed;
			}
			else
			{
				PosR.x += RevertSpeed;
			}
		}
	}
	{
		float PosDifference_Z = PosR.z - m_pos.z;//カメラの注視点とプレイヤーの位置の差分
		float RevertSpeed = abs(PosDifference_Z / CameraLimit);//プレイヤーの位置に戻す速度
		//RevertSpeedが0.0f出はなかったら
		if (RevertSpeed != 0.0f)
		{
			//差分が正の数なら負の数に戻すようにする
			if (PosDifference_Z > 0)
			{
				PosR.z -= RevertSpeed;
			}
			else
			{
				PosR.z += RevertSpeed;
			}

		}

	}
	pCamera->SetPosR(PosR);

}
void CPlayer::Drawtext()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[3000];
	int nNum = 0;

	nNum = sprintf(&str[0], "\n\n 情報 \n");

	//カメラの取得
	CCamera *pCamera = CRenderer::GetCamera();
	D3DXVECTOR3 PosV = pCamera->GetPosV();
	D3DXVECTOR3 PosR = pCamera->GetPosR();
	D3DXVECTOR3 Rot = pCamera->GetRot();
	CMouse *pMouse = CManager::GetMouse();
	POINT po;
	GetCursorPos(&po);
	float MousePos_X = po.x;
	float MousePos_Y = po.y;
	RECT rec;
	HWND hDeskWnd = GetForegroundWindow();
	GetWindowRect(hDeskWnd, &rec); //デスクトップのハンドルからその(画面の)大きさを取得

	//nNum += sprintf(&str[nNum], " [GetMousePos().lX] %.5f\n", MousePos_X);
	//nNum += sprintf(&str[nNum], " [GetMousePos().lY] %.5f\n", MousePos_Y);
	//nNum += sprintf(&str[nNum], " [rec.left] %d\n", rec.left);
	//nNum += sprintf(&str[nNum], " [rec.right] %d\n", rec.right);

	nNum += sprintf(&str[nNum], " [MotionCnt] %.5f\n", m_pMotion->GetMotionCnt());
	nNum += sprintf(&str[nNum], " [攻撃力] %.5f\n", m_fPower);
	nNum += sprintf(&str[nNum], " [攻撃増加倍率] %.5f\n", m_fPowerDiameter);
	nNum += sprintf(&str[nNum], " [レベル] %d\n", m_nLevel);
	nNum += sprintf(&str[nNum], " [必要経験値] %.2f\n", m_fMaxExp);

	nNum += sprintf(&str[nNum], " [視点] %.5f,%.5f,%.5f\n", PosV.x, PosV.y, PosV.z);
	nNum += sprintf(&str[nNum], " [注視点] %.5f,%.5f,%.5f\n", PosR.x, PosR.y, PosR.z);
	nNum += sprintf(&str[nNum], " [回転] %.5f,%.5f,%.5f\n", Rot.x, Rot.y, Rot.z);

	//nNum += sprintf(&str[nNum], " [MotionKey] %d\n", m_pMotion->GetMotionKey());
	//nNum += sprintf(&str[nNum], " [Canhit] %d\n", m_pSword->GetCanHit());
	//nNum += sprintf(&str[nNum], " [m_bCanAttack] %d\n", m_bCanAttack);


	LPD3DXFONT pFont = CManager::GetRenderer()->GetFont();
	// テキスト描画
	pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

}

//----------------------------------------------
//移動用のキーを押しているかの判定を返す処理
//----------------------------------------------
bool CPlayer::IsMoveKeyCheck()
{
	CInputKeyBoard *Key = CManager::GetInputKeyboard();

	return (Key->GetPress(DIK_W) == true ||
		Key->GetPress(DIK_A) == true ||
		Key->GetPress(DIK_S) == true ||
		Key->GetPress(DIK_D) == true);
}
//----------------------------------------------
//一定時間間隔を置く処理
//----------------------------------------------
bool CPlayer::FixedTimeInterval(float fMaxTime)
{
	bool bStop = false;
	m_fStopTime++;
	if (m_fStopTime > fMaxTime)
	{
		m_fStopTime = 0.0f;
		bStop = true;
	}
	//一定時間たったかを返す
	return bStop;
}
//-----------------------------------------------
//各スキルの処理のまとめ
//-----------------------------------------------
void CPlayer::EachSkillManager()
{
	//オートヒール
	if (m_bCanAutoHeel)
	{
		m_nTimer++;

		if (m_nTimer % 8 == 0)
		{
			CPresetEffect::SetEffect3D(16, m_pos, {}, {}, {});	//リジェネ
		}
		if (m_nTimer >= 60)
		{
			m_nTimer = 0;
			float fHP = CManager::GetGame()->GetHPGauge()->GetGaugeBer(1)->GetGaugeValue();
			if (fHP <= 0.0f)
			{
				CManager::GetGame()->GetHPGauge()->SetGauge(-m_fAutoHeel, 0);
			}

		}

	}
	//ブラックホール
	if (m_bCanBlackHole)
	{
		m_nBlackHoleCnt++;
		if (m_nBlackHoleCnt >= BlackHoleShotTime)
		{
			m_nBlackHoleCnt = 0;
			CBlackHole::Create(m_pos, m_rot, m_pNearEnemy, m_BlackHoleLevel);
		}

	}
	if (m_bCanDamegeGuard)
	{
		if (m_pDamegeGuard)
		{
			m_pDamegeGuard->SetPos(m_pos);
		}
	}
	//ミサイル
	if (m_bCanMissile)
	{
		m_nMissileCnt++;
		if (m_nMissileCnt >= MissileCntMax)
		{
			m_nMissileCnt = 0;
			CMissile::Create();

		}
	}
	//アシスト攻撃
	if (m_bCanAssistCrystal)
	{
		for (int nCnt = 0; nCnt < MAX_ASSISTCRYSTAL; nCnt++)
		{
			if (m_pAssistCrystal[nCnt])
			{
				m_pAssistCrystal[nCnt]->Update();
			}
		}
	}
}
//-----------------------------------------------
//攻撃したときの移動処理
//-----------------------------------------------
void CPlayer::AttackMove(float fMoveVolume)
{
	m_pos.x -= sinf(m_rot.y)*fMoveVolume;
	m_pos.z -= cosf(m_rot.y)*fMoveVolume;
}
int CPlayer::GetSkillLevel(int nSkillType)
{
	switch (nSkillType)
	{
	case CPlayer::ATKup:
		return m_ATKupLevel;
		break;
	case CPlayer::Heal:
		return m_HealLevel;
		break;
	case CPlayer::OverHeal:
		return m_OverHealLevel;
		break;
	case CPlayer::Sheild:
		return m_SheildLevel;
		break;
	case CPlayer::Beam:
		return m_BeamLevel;
		break;
	case CPlayer::BlackHole:
		return m_BlackHoleLevel;
		break;
	case CPlayer::Rocket:
		return m_RocketLevel;
		break;
	case CPlayer::RushAttack:
		return m_RushAttackLevel;
		break;
	}
	return -1;

}
//-----------------------------------------------
//武器のセット
//---------------------------------------------
void CPlayer::WeaponSet(const char * pcFileName)
{
	int nParent = 0;				//親子関係
	char string[6][255];
	D3DXVECTOR3 PartsPos, PartsRot;	//各パーツの位置と向き
									//textファイル読み込み
	FILE *pfile = fopen(pcFileName, "r");
	//ヌルチェック
	if (pfile != NULL)
	{
		while (1)
		{
			//一単語を読み込む
			fscanf(pfile, "%s", &string[0]);

			while (strcmp(string[0], "CHARACTERSET") == 0)
			{
				fscanf(pfile, "%s", &string[1]);
				//パーツの設定
				while (strcmp(string[1], "WEAPONSET") == 0)
				{
					//一単語を読み込む
					fscanf(pfile, "%s", &string[2]);

					//位置
					if (strcmp(string[2], "POS") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%f %f %f", &PartsPos.x, &PartsPos.y, &PartsPos.z);
					}
					//向き
					if (strcmp(string[2], "ROT") == 0)
					{
						fscanf(pfile, "%s", &string[2]);
						fscanf(pfile, "%f %f %f", &PartsRot.x, &PartsRot.y, &PartsRot.z);
					}
					//モデルの生成
					if (strcmp(string[2], "END_WEAPONSET") == 0)
					{
						//武器の生成
						if (!m_pSword)
						{
							m_pSword = CSword::Create(PartsPos, PartsRot, m_pParts[5]);
							m_pSword->SetPower(m_fPower);
						}
						break;
					}
				}
				//終了
				if (strcmp(string[1], "END_CHARACTERSET") == 0)
				{
					break;
				}

			}
			if (strcmp(string[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
}
