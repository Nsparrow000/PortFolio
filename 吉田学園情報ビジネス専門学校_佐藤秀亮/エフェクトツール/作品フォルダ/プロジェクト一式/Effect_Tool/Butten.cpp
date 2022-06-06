//*****************************************************************************
// ボタン(Butten.cpp)
// Author : 佐藤秀亮
//*****************************************************************************
#include "Butten.h"
#include "manager.h"
#include "control.h"
#include "renderer.h"
#include "mouse.h"
#include "IntermediateSave.h"
#include "LoadEffect.h"

#include <assert.h>


//*****************************************************************************
//静的
//*****************************************************************************
POINT CButten::m_po = {};
D3DXVECTOR2 CButten::m_Mousepos = {};
int CButten::m_nPatten = {};
int CButten::m_nButtenTotal = 0;
int CButten::m_nTotal[MAX_BUTTENPATTERN] = {};

bool CButten::m_MouseButtenPush = false;
int CButten::m_MousePushTime = 0.0f;

bool CButten::m_PushDeley = false;
int CButten::m_DeleyTime = 0.0f;

CButten::BUTTEN_STATE CButten::ButtenState[MAX_BUTTENPATTERN][MAX_BUTTEN] = {};
//*****************************************************************************
//マクロ
//*****************************************************************************
#define LOAD_PRESET_TEXT "data/Preset.txt"
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CButten::CButten(int nPriority) : CScene2D(nPriority)
{
	m_Mousepos = {};
	m_po.x = SCREEN_WIDTH / 2;
	m_po.y = SCREEN_HEIGHT / 2;

	ButtenState[MAX_BUTTENPATTERN][MAX_BUTTEN] = {};
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CButten::~CButten(void)
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CButten::Init(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, OPERATION Operation, float nIndeNum, DISPLAY Display, DISPLAY_PATTERN Pattern)
{
	CScene2D::Init(pos);

	m_pKeyboard = CManager::GetKeyboard();
	m_pMouse = CManager::GetMouse();

	m_aOperation = Operation;
	aDisplay = Display;

	m_nIndeNum = nIndeNum;

	if (Pattern == NONE)
	{
		aDisplayPattern = {};
	}
	else
	{
		aDisplayPattern = Pattern;
	}
	m_Size = D3DXVECTOR2(SizeX, SizeY);
	m_bUninit = false;

	CScene2D::SetTexture(nType);	//選択した番号のテクスチャを貼る
	CScene2D::SetWhidth(m_Size.x);
	CScene2D::SetHight(m_Size.y);

	return S_OK;

}

//*****************************************************************************
//終了
//*****************************************************************************
void CButten::Uninit()
{
	CScene2D::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CButten::Update()
{
	D3DXVECTOR3 pos = GetPosition();


	//マウス座標
	m_Mousepos.x = CManager::GetRenderer()->GetMousePos().x;
	m_Mousepos.y = CManager::GetRenderer()->GetMousePos().y;

	if (CControl::GetPlayerMode() == false)
	{
		//マウスがポリゴンの範囲内に
		if (pos.x - (m_Size.x / 2) < m_Mousepos.x &&
			pos.x + (m_Size.x / 2) > m_Mousepos.x &&
			pos.y - (m_Size.y / 2) < m_Mousepos.y &&
			pos.y + (m_Size.y / 2) > m_Mousepos.y)
		{
			m_fAlpha = 255.0f;
			//左クリック
			if (m_pMouse->GetMouseButton(CMouse::DIM_L) == true)
			{
				m_fAlpha = 150.0f;
				Set(m_aOperation, m_nIndeNum);

				m_MouseButtenPush = true;
			}
			if (m_MouseButtenPush == true)
			{
				m_MousePushTime++;

				if (m_MousePushTime >= 20)
				{
					m_PushDeley = true;
				}
				if (m_PushDeley == true)
				{
					m_DeleyTime++;
					if (m_DeleyTime > 5)
					{
						m_DeleyTime = 0;
						m_fAlpha = 150.0f;
						Set(m_aOperation, m_nIndeNum);

					}
				}
			}
		}
		else
		{
			m_fAlpha = 100.0f;
		}
	}
	else
	{
		m_fAlpha = 0.0f;
	}

	if (m_pMouse->GetRelease(CMouse::DIM_L) == true)
	{
		m_PushDeley = false;
		m_MouseButtenPush = false;
		m_MousePushTime = 0;
	}

	if (aDisplay == CHANGE)
	{
		if (aDisplayPattern - 1 != CControl::GetPattern())
		{
			m_bUninit = true;
		}
	}

	CScene2D::FadeColorChange((int)m_fAlpha);
	SetPosition(pos);
	//破棄
	if (m_bUninit == true)
	{
		Uninit();
	}

}

//*****************************************************************************
// 描画
//*****************************************************************************
void CButten::Draw()
{
	CScene2D::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CButten *CButten::Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, OPERATION Operation, float nIndeNum, DISPLAY Display, DISPLAY_PATTERN Pattern)
{
	CButten *pButten = NULL;
	pButten = new CButten(CManager::PRIORITY_BUTTEN);		//メモリ確保
	//NULLチェック
	if (pButten != NULL)
	{
		pButten->Init(pos, SizeX, SizeY, nType, Operation, nIndeNum, Display, Pattern);
	}
	return pButten;
}

//*****************************************************************************
//数値の変化
//*****************************************************************************
void CButten::Set(OPERATION Operation, float nIndeNum)
{
	switch (Operation)
	{
	case(PATTERN):
		CControl::AddPattern((int)nIndeNum);
		
		break;
	case(COLSELECT):
		CControl::AddSelectCol((int)nIndeNum);
		break;
	case(COL):
		CControl::AddColor((int)nIndeNum, CControl::GetSerectColor());
		break;
	case(ADDCOL):
		CControl::AddChangeCol((int)nIndeNum, CControl::GetSerectColor());
		break;
	case(SIZE):
		CControl::AddSize(nIndeNum);
		break;
	case(ADDSIZE):
		CControl::AddChangeSize(nIndeNum);
		break;
	case(LIFE):
		CControl::AddLife((int)nIndeNum);
		break;
	case(DENSITY):
		CControl::AddDensity((int)nIndeNum);
		break;
	case(DIFFUSION):
		CControl::AddDiffusion((int)nIndeNum);
		break;
	case(TEXTURE):
		CControl::AddTexture((int)nIndeNum);
		break;
	case(ROTATE):
		CControl::AddRotateAdd(nIndeNum);
		break;
	case(SAVE):
		CControl::SaveEffect(CManager::GetMode(), CControl::GetPattern());
		if (CManager::GetMode() == CManager::MODE_2D)
		{
			CIntermeiateSave::IntermeiateSave(CManager::GetMode(), CControl::GetPattern(), INTERMEDIATE_TEXT2D);	//中間セーブ
		}
		else if (CManager::GetMode() == CManager::MODE_3D)
		{
			CIntermeiateSave::IntermeiateSave(CManager::GetMode(), CControl::GetPattern(), INTERMEDIATE_TEXT3D);	//中間セーブ
		}
		break;
	case(LOAD):
		if (CManager::GetMode() == CManager::MODE_2D)
		{
			CIntermeiateSave::IntermeiateLoad(CManager::GetMode(), INTERMEDIATE_TEXT2D);	//中間ロード
		}
		else if (CManager::GetMode() == CManager::MODE_3D)
		{
			CIntermeiateSave::IntermeiateLoad(CManager::GetMode(), INTERMEDIATE_TEXT3D);	//中間ロード
		}
		break;
	case(MOVEX):
		CControl::AddMoveX(nIndeNum);
		break;
	case(MOVEY):
		CControl::AddMoveY(nIndeNum);
		break;
	case(ADDMOVEX):
		CControl::AddMoveXAdd(nIndeNum);
		break;
	case(ADDMOVEY):
		CControl::AddMoveYAdd(nIndeNum);
		break;
	case(DESTROY):
		CControl::AddUninitVectl((int)nIndeNum);
		break;

	case(TRAJECTTOP):
		CControl::AddTrajectTop((int)nIndeNum);
		break;
	case(TRAJECTTOPCOL):
		CControl::TrajectColor((int)nIndeNum, CControl::GetSerectColor());
		break;
	case(TRAJECTTOPADDCOL):
		CControl::AddTrajectCol((int)nIndeNum, CControl::GetSerectColor());
		break;
	case(TRAJECTCUR):
		CControl::AddTrajectCur((int)nIndeNum);
		break;
	case(MOVE3DX):
		CControl::Addmove3d(nIndeNum,0);
		break;
	case(MOVE3DY):
		CControl::Addmove3d(nIndeNum, 1);
		break;
	case(MOVE3DZ):
		CControl::Addmove3d(nIndeNum, 2);
		break;
	case(RANDMOVE3D):
		CControl::AddRandMove((int)nIndeNum);
		break;
	case(SYNTHETIC):
		CControl::AddSynthetic((int)nIndeNum);
		break;
	case(PARTICLESIZE):
		CControl::AddParticleSize(nIndeNum);
		break;
	case(PARTICLETIME):
		CControl::AddParticleTime((int)nIndeNum);
		break;
	case(DISTANCE):
		CControl::AddDistance(nIndeNum);
		break;
	case(ACTIVETIME):
		CControl::AddActiveTime((int)nIndeNum);
		break;
	case(MAXSIZE):
		CControl::AddMaxSize(nIndeNum);
		break;
	case(PARTICLEADDSIZE):
		CControl::AddParticleAddSize(nIndeNum);
		break;
	case(PARTICLECOLOR):
		CControl::AddParticleColor((int)nIndeNum, CControl::GetSerectColor());
		break;
	case(PARTICLEADDCOLOR):
		CControl::AddParticleAddCol((int)nIndeNum, CControl::GetSerectColor());
		break;
	case(PARTICLESYNSETIC):
		CControl::AddParticleSynthetic((int)nIndeNum);
		break;
	case(ACTIVEADDSIZE):
		CControl::AddAvctiveAddSize(nIndeNum);
		break;
	case(PRESETLOAD):
		CLoadEffect::EffectOrder(LOAD_PRESET_TEXT);
		CLoadEffect::EffectStateLoad(LOAD_PRESET_TEXT);

		break;
	case(SCONDTIME):
		CControl::AddSecondTime((int)nIndeNum);
		break;
	case(VTX):
		CControl::AddVtx((int)nIndeNum);
		break;
	case(TYPE):
		CControl::AddType((int)nIndeNum);
		break;
	case(TEXU):
		CControl::AddTexMoveU(nIndeNum);
		break;
	case(TEXV):
		CControl::AddTexMoveV(nIndeNum);
		break;
	case(TEXNUM):
		CControl::AddTexNumU(nIndeNum);
		break;
	case(SECONDTYPE):
		CControl::AddSecondType(nIndeNum);
		break;
	case(TEXSPLITU):
		CControl::AddSplitU(nIndeNum);
		break;
	case(TEXSPLITV):
		CControl::AddSplitV(nIndeNum);
		break;
	case(ANIMCONT):
		CControl::AddAnimCont(nIndeNum);
		break;
	case(TEXNUMV):
		CControl::AddTexNumV(nIndeNum);
		break;
	case(HIGTH):
		CControl::AddHigth(nIndeNum);
		break;
	case(CONTROLBEZIERX):
		CControl::AddContorolBezierX(nIndeNum);
		break;
	case(CONTROLBEZIERY):
		CControl::AddContorolBezierY(nIndeNum);
		break;
	case(CONTROLBEZIERZ):
		CControl::AddContorolBezierZ(nIndeNum);
		break;
	case(SECONDTEX):
		CControl::AddSecondTex(nIndeNum);
		break;
	default:
		assert(false);
		break;
	}
}

//*****************************************************************************
//ボタンステータスの格納
//*****************************************************************************
void CButten::SetButtenState(D3DXVECTOR3 pos,
	float SizeX, float SizeY,
	int nType,
	OPERATION Operation,
	float nIndeNum,
	DISPLAY_PATTERN Pattern)
{
	ButtenState[m_nPatten][m_nButtenTotal].m_pos = pos;
	ButtenState[m_nPatten][m_nButtenTotal].m_SizeX = SizeX;
	ButtenState[m_nPatten][m_nButtenTotal].m_SizeY = SizeY;
	ButtenState[m_nPatten][m_nButtenTotal].m_nType = nType;
	ButtenState[m_nPatten][m_nButtenTotal].m_Operation = Operation;
	ButtenState[m_nPatten][m_nButtenTotal].m_nIndeNum = nIndeNum;
	ButtenState[m_nPatten][m_nButtenTotal].Pattrn = Pattern;

	m_nButtenTotal++;
}


//*****************************************************************************
//ボタンのセット
//*****************************************************************************
void CButten::SetButten(int nPatten)
{
	for (int nCnt = 0; nCnt < m_nTotal[nPatten]; nCnt++)
	{
		CButten::Create(
			ButtenState[nPatten][nCnt].m_pos,
			ButtenState[nPatten][nCnt].m_SizeX,
			ButtenState[nPatten][nCnt].m_SizeY,
			ButtenState[nPatten][nCnt].m_nType,
			ButtenState[nPatten][nCnt].m_Operation,
			ButtenState[nPatten][nCnt].m_nIndeNum,
			(CButten::DISPLAY)1,
			ButtenState[nPatten][nCnt].Pattrn);
	}
}
