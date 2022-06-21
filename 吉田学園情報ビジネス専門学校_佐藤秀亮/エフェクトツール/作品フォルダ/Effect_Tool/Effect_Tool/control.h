//*****************************************************************************
// エフェクトの諸々設定(control.h)
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "main.h"
#include "manager.h"

//=============================================================================
//マクロ
//=============================================================================
#define MAX_PATTERN (10)	//最大パターン
#define MAX_DENSITY (30)		//最大密度
#define MAX_LIFE (1000)			//最大寿命

//前方宣言
class CKeyboard;

class CControl
{
public:
	CControl();		//コンストラクタ
	~CControl();	//デストラクタ

	static HRESULT Init();
	static void Update();

	static void Control();
	static void Control2D();
	static void Control3D();
	static int GetControlCoror(int nSerectcolor);	//色取得
	static int GetChangeCol(int nSerectcolor);	//増減値取得
	static void SetControlCoror(int nSerectcolor);	//色セット
	static bool GetRandCol(int nSerectcolor);	//色ランダム化取得
	static void AddColor(int Color, int nSerect);		//色増減
	static void AddChangeCol(int Color, int nSerect);	//色増減値の増減
	static int GetSerectColor() { return m_SerectColor; }	//色選択番号
	static const float GetSize() { return m_ControlSize; }	//サイズ
	static const float GetChangeSize() { return m_ControlChangeSize; }	//サイズ増減
	static const int GetLife() { return m_nLife; }			//寿命
	static const int GetTex() { return m_nTex; }				//テクスチャ
	static const int GetDensity() { return m_nDensity; }		//密度
	static const bool GetTap() { return m_bTap; }	//タップON or OFF
	static const int GetPattern() { return m_nPattern; }		//パターン
	static const float GetRotate() { return m_fAddRotate; }	//回転
	static const bool GetRandMoveX() { return m_RandMoveX; }	//ランダムXON or OFF
	static const bool GetRandMoveY() { return m_RandMoveY; }	//ランダムYON or OFF
	static void SetColorRandom(int nSerectcol , bool bAdaptation);	//カラーランダムON or OFF
	static const int GetSynthetic() { return nSynthetic; }	//合成
	static const D3DXVECTOR2 GetMove() { return m_move; }		//初期の動き値
	static const bool GetMouseCursor() { return m_bMouseCursor; }	//カーソル追従ON or OFF
	static const D3DXVECTOR2 GetAddMove() { return m_Addmove; }		//移動加算
	static const int GetDiffusion() { return m_Diffusion; }			//密度
	static const int GetUninitVectl() { return m_nUninitVectl; }		//消えるベクトル
	static const bool GetPlayerFollow() { return m_bPlayerFollow; }	//プレイヤー追従ON or OFF
	static const bool GetPlayerMode()  { return m_bPlayerMode; }
	static const void SetPlayerMode(bool bMode) {m_bPlayerMode = bMode; }
	static const int GetModelNum() { return m_ModelNum; }
	static const D3DXVECTOR3 Getmove3d() { return m_move3d; }		//移動
	static const int GetTrajectModelNum() { return m_ModelTrajecNum; }
	static const int GetRandMove1() { return m_nRandMove1; }
	static const D3DCOLORVALUE GetColor(){ return m_ControlColor; }
	static const D3DCOLORVALUE GetChangeColor() { return m_ControlChangeCol; }
	static const float GetParticleSize() { return ParticleSize; }
	static const int GetParticleTime() { return ParticleTime; }
	static const float GetDistance() { return fDistance; }
	static const int GetActiveTime() { return nActiveTime; }
	static const float GetMaxSize() { return fMaxSize; }
	static const float GetParticleAddSize() {return fParticleAddSize ; }
	static int GetParticleColor(int nSerect);		//色増減
	static int GetParticleAddCol(int nSerect);	//色増減値の増減
	static const int GetParticleSynthetic() { return nParticleSynthetic; }	//合成
	static const float GetAvctiveAddSize() { return ActiveAddSize; }	//アクティブ中の大きさ変更
	static int GetSecondTime() { return m_nSecondTime; }
	static int GetVtx() { return m_nVtx; }	//球の頂点数
	static int GetType() { return m_nType; }	//球タイプ
	static void TrajectColor(int Color, int nSerect);
	static int GetTrajectColor(int nSerectcolor);	//色取得
	static void AddTrajectCol(int Color, int nSerect);
	static int GetTrajectCol(int nSerectcolor);	//色取得
	static float GetTexMoveU() { return m_TexMove.x; }	
	static float GetTexMoveV() { return m_TexMove.y; }
	static D3DXVECTOR2 GetTexNum() { return m_TexNum; }	//テクスチャ枚数
	static int GetSecondType() { return m_nSecondTime; }
	static float GetTexPatternU() { return m_TexPattern.x; }
	static float GetTexPatternV() { return m_TexPattern.y; }
	static int GetAnimCont() { return m_nAnimCont; }
	static float GetSplitU() { return m_nSplit.x; }
	static float GetSplitV() { return m_nSplit.y; }
	static float GetHigth() { return m_fHigth; }

	static float GetContorolBezierX() {return m_ContorolBezier.x; }	//制御点
	static float GetContorolBezierY() {return m_ContorolBezier.y; }	//制御点
	static float GetContorolBezierZ() {return m_ContorolBezier.z; }	//制御点
	static int GetSecondTex() { return m_SecondTex; }
	static int GetAnimPatternType() { return m_AnimPatternType; }


	//テキストセーブ
	static void SaveEffect(CManager::MODE mode, int nPattern);
	static void SetButten(int nPattern);

	//中間セーブ関係
	static void SetPattern(int nPattern) { m_nPattern = nPattern; }
	static void SetColor(D3DCOLORVALUE Color) { m_ControlColor = Color; }
	static void SetChangeColor(D3DCOLORVALUE ChangeColor) { m_ControlChangeCol = ChangeColor; }
	static void SetSize(float Size) { m_ControlSize = Size; }
	static void SetChangeSize(float ChangeSize) { m_ControlChangeSize = ChangeSize; }
	static void SetLife(int nLife) { m_nLife = nLife; }
	static void SetTexture(int nTex) { m_nTex = nTex; }
	static void SetDensity(int nDensity) { m_nDensity = nDensity; }
	static void SetAddRotate(float fAddRotate) { m_fAddRotate = fAddRotate; }
	static void SetDiffusion(int Diffusion) { m_Diffusion = Diffusion; }
	static void SetMove(D3DXVECTOR2 move) {m_move = move; }
	static void SetAddmove(D3DXVECTOR2 Addmove) { m_Addmove = Addmove; }
	static void SetUninitVectl(int nUninitVectl) { m_nUninitVectl = nUninitVectl; }
	static void SetSynthetic(int Synthetic) { nSynthetic = Synthetic; }
	static void SetTrajectColor(D3DCOLORVALUE Color) { m_TrajectColor = Color; }
	static void SetTrajectChangeColor(D3DCOLORVALUE ChangeColor) { m_TrajectChangeCol = ChangeColor; }
	static void SetTrajectTop(int Top) { m_ModelTrajecNum = Top; }
	static void SetTrajectCur(int Cur) { m_ModelNum = Cur; }
	static void SetMove3D(D3DXVECTOR3 move) { m_move3d = move; }
	static void SetRandMove(int Randmove) { m_nRandMove1 = Randmove; }
	static void SetParticleSize(float Size) { ParticleSize = Size; }
	static void SetParticleTime(int Time) { ParticleTime = Time; }
	static void SetDistance(float Distance) { fDistance = Distance; }
	static void SetActive(int Active) { nActiveTime = Active; }
	static void SetParticleAddSize(float AddSize) { fParticleAddSize = AddSize; }
	static void SetMaxSize(float MaxSize) { fMaxSize = MaxSize; }
	static void SetParticleColor(D3DCOLORVALUE Color) { m_ParticleColor = Color; }		//色増減
	static void SetParticleAddCol(D3DCOLORVALUE ChangeColor) { m_ParticleAddCol = ChangeColor; }	//色増減値の増減
	static void SetParticleSynthetic(int Synthetic) { nParticleSynthetic = Synthetic; }
	static void SetAvctiveAddSize(float ActiveSize) { ActiveAddSize = ActiveSize; }
	static void SetSecondTime(int SecondTime) { m_nSecondTime = SecondTime; }
	static void SetVtx(int Vtx) { m_nVtx = Vtx; }
	static void SetType(int nType) { m_nType = nType;}	//球タイプ
	static void SetTexMoveUV(D3DXVECTOR2 TexMove) { m_TexMove = TexMove; }	//テクスチャ
	static void SetTexNum(D3DXVECTOR2 Num) { m_TexNum = Num; }	//テクスチャ
	static void SetSecondType(int SecondType) { m_nSecondTime = SecondType; }
	static void SetTexPattern(D3DXVECTOR2 TexPattern) { m_TexPattern = TexPattern; }	//テクスチャ
	static void SetnAnimCont(int nAnimCont) { m_nAnimCont = nAnimCont; }	//テクスチャ
	static void SetnSplit(D3DXVECTOR2 nSplit) { m_nSplit = nSplit; }	//テクスチャ
	static void SetHigth(float Higth) { m_fHigth = Higth; }	//高さ
	static void SetControlBezier(D3DXVECTOR3 ControlBezier) { m_ContorolBezier = ControlBezier; }	//ベジェ制御点
	static void SetSecondTex(int SecondTex) { m_SecondTex = SecondTex; }
	static void SetAnimPatternType(int AnimPatternType) { m_AnimPatternType = AnimPatternType; }

	//増減関係
	static void AddPattern(int nPattern) {
		m_nPattern += nPattern;
		if (m_nPattern >= MAX_PATTERN)
		{
			m_nPattern = MAX_PATTERN - 1;
		}
		else if (m_nPattern <= -1)
		{
			m_nPattern = 0;
		}
		else
		{
			CControl::SetButten(m_nPattern);
		}
		; }

	static void AddSelectCol(int nPattern) { m_SerectColor += nPattern; }
	static void AddSize(float Size) {
		m_ControlSize += Size;
	if (m_ControlSize <= 0)
	{
		m_ControlSize = 0;
	}

	}
	static void AddChangeSize(float ChangeSize) {
		m_ControlChangeSize += ChangeSize;
	}
	static void AddLife(int nLife) {
		m_nLife += nLife;
		if (m_nLife >= MAX_LIFE)
		{
			m_nLife = MAX_LIFE;
		}
		else if (m_nLife <= 0)
		{
			m_nLife = 0;
		}
	}
	static void AddTexture(int nTex) { m_nTex += nTex; }
	static void AddDensity(int nDensity) { m_nDensity += nDensity;}
	static void AddRotateAdd(float fAddRotate) { m_fAddRotate += fAddRotate; }
	static void AddDiffusion(int Diffusion) { m_Diffusion += Diffusion; }
	static void AddUninitVectl(int nUninitVectl) { m_nUninitVectl += nUninitVectl; }
	static void AddMoveX(float MoveX) { m_move.x += MoveX;}
	static void AddMoveY(float MoveY) {m_move.y += MoveY;}
	static void AddMoveXAdd(float AddMoveX) { m_Addmove.x += AddMoveX; }
	static void AddMoveYAdd(float AddMoveY) { m_Addmove.y += AddMoveY; }

	static void AddTrajectTop(int nTraject) { m_ModelTrajecNum += nTraject;
	if (m_ModelTrajecNum > 14)
	{
		m_ModelTrajecNum = 0;
	}
	else if (m_ModelTrajecNum <= 0)
	{
		m_ModelTrajecNum = 14;
	}
	}
	static void AddTrajectCur(int nTraject) { m_ModelNum += nTraject;
	if (m_ModelNum > 14)
	{
		m_ModelNum = 0;
	}
	else if (m_ModelNum <= 0)
	{
		m_ModelNum = 14;
	}
	}
	static void Addmove3d(float Move, int nNum);		//移動
	static void AddRandMove(int nMove) { 
		m_nRandMove1 += nMove;
		if (m_nRandMove1 <= 0)
		{
			m_nRandMove1 = 1;
		}
	}

	static void AddSynthetic(int Synthetic) {
		nSynthetic += Synthetic;
		if (nSynthetic < 0)
		{
			nSynthetic = 0;
		}
	}	//合成

	static void AddParticleSize(float Size) { ParticleSize += Size;}	//パーティクルサイズ
	static void AddParticleTime(int Time) { ParticleTime += Time;
	if (ParticleTime <= 0)
	{
		ParticleTime = 1;
	}
	}	//パーティクル発生時間
	static void AddDistance(float Distance) { fDistance += Distance;
	if (fDistance <= 0)
	{
		fDistance = 1.0f;
	}
	}	//距離の調整

	static void AddActiveTime(int nActive) { nActiveTime += nActive; }
	static void AddMaxSize(float MaxSize) { fMaxSize += MaxSize; }
	static void AddParticleAddSize(float ParticleAddSize) { fParticleAddSize += ParticleAddSize; }
	static void AddAvctiveAddSize(float ActiveSize) { ActiveAddSize += ActiveSize; }

	static void AddSecondTime(int nScondTime) {m_nSecondTime += nScondTime;}

	static void AddParticleColor(int Color, int nSerect);		//色増減
	static void AddParticleAddCol(int Color, int nSerect);	//色増減値の増減

	static void AddParticleSynthetic(int Synthetic) {
		nParticleSynthetic += Synthetic;
		if (nParticleSynthetic < 0)
		{
			nParticleSynthetic = 0;
		}
	}	//合成

	static void AddVtx(int Vtx) { m_nVtx += Vtx; }
	static void AddType(int nType) { m_nType += nType; }	//球タイプ

	static void AddTexMoveU(float MoveU) { m_TexMove.x += MoveU; }	//テクスチャU
	static void AddTexMoveV(float MoveV) { m_TexMove.y += MoveV; }	//テクスチャV
	static void AddTexNumU(float NumU) { m_TexNum.x += NumU; }	//テクスチャ枚数
	static void AddTexNumV(float NumV) { m_TexNum.y += NumV; }	//テクスチャ枚数

	static void AddSecondType(int SecondType) { m_nSecondTime += SecondType; }

	static void AddTexPatternU(float PatternU) {m_TexPattern.x += PatternU;}	//テクスチャU
	static void AddTexPatternV(float PatternV) { m_TexPattern.y += PatternV; }	//テクスチャV
	static void AddAnimCont(int AnimCont) { m_nAnimCont += AnimCont; }	//テクスチャV
	static void AddSplitU(float SplitU) { m_nSplit.x += SplitU; }	//テクスチャV
	static void AddSplitV(float SplitU) { m_nSplit.y += SplitU; }	//テクスチャV
	static void AddHigth(float Higth) { m_fHigth += Higth; }	//高さ
	
	static void AddContorolBezierX(float NumX) { m_ContorolBezier.x += NumX;}	//制御点
	static void AddContorolBezierY(float NumY) { m_ContorolBezier.y += NumY;}	//制御点
	static void AddContorolBezierZ(float NumZ) { m_ContorolBezier.z += NumZ;}	//制御点

	static void AddSecondTex(int SecondTex) { m_SecondTex += SecondTex; }	//制御点

	static void AddAnimPatternType(int AnimPatternType) { m_AnimPatternType += AnimPatternType; }	//制御点

private:
	static CKeyboard *m_pKeyboard;
	static int m_SerectColor;		//色選択
	static bool m_bTap;	//タップで出すかどうか
	static D3DCOLORVALUE m_ControlColor;		//色変更
	static D3DCOLORVALUE m_ControlChangeCol;	//増減値
	static float m_ControlSize;			//サイズ
	static float m_ControlChangeSize;		//サイズ増減
	static int m_nLife;	//寿命
	static int m_nTex;	//テクスチャ選択
	static int m_nDensity;	//密度
	static int m_nPattern;	//パターン

	static bool m_RandMoveX;	//ランダムX ON OFF
	static bool m_RandMoveY;	//ランダムY ON OFF

	static bool m_ColorR;	//カラーランダム ON OFF
	static bool m_ColorG;	//カラーランダム ON OFF
	static bool m_ColorB;	//カラーランダム ON OFF

	static float m_fAddRotate;	//回転

	static int nSynthetic;	//合成関係

	static D3DXVECTOR2 m_TexMove;	//テクスチャ座標

	//２D
	static bool m_bMouseCursor;			//カーソル追従
	static D3DXVECTOR2 m_move;	//初期の移動値
	//固有
	static D3DXVECTOR2 m_Addmove;	//移動加算
	static int m_Diffusion;		//ランダム拡散率
	static int m_nUninitVectl;	//消える直線距離
	//3D
	static bool m_bPlayerFollow;			//プレイヤーにするか追従
	static bool m_bPlayerMode;	//プレイヤー操作かどうか
	static int m_ModelNum;	//軌跡手元
	static D3DXVECTOR3 m_move3d;	//移動加算
	static int m_nRandMove1;	//ランダムの最大・最小移動量

	static float ParticleSize;	//パーティクルのサイズ
	static int ParticleTime;	//パーティクル発生時間
	static float fDistance;		//出現地点
	static int nActiveTime;		//効果時間
	static float fMaxSize;		//最大の大きさ
	static float fParticleAddSize;	//パーティクル大きさ増減変更
	static D3DCOLORVALUE m_ParticleColor;		//パーティクル色
	static D3DCOLORVALUE m_ParticleAddCol;	//パーティクル色加算
	static int nParticleSynthetic;	//パーティクル合成関係
	static float ActiveAddSize;	//アクティブ中の大きさ変更
	static int m_nSecondTime;	//２番目の時間
	static int m_SecndType;	//2つ目のパターン選択

	//球
	static int m_nVtx;	//球頂点数
	static int m_nType;	//タイプ

	static D3DXVECTOR2 m_TexNum;	//テクスチャ枚数

	//軌跡
	static int m_ModelTrajecNum;	//軌跡先端
	static D3DCOLORVALUE m_TrajectColor;		//色変更
	static D3DCOLORVALUE m_TrajectChangeCol;	//色変更

	static D3DXVECTOR2 m_TexPattern;
	static int m_nAnimCont;	//アニメーションカウント
	static D3DXVECTOR2 m_nSplit;	//分割数
	static float m_fHigth;	//高さ
	static D3DXVECTOR3 m_ContorolBezier;	//ベジェ制御点
	static int m_SecondTex;

	static int m_AnimPatternType;

};

#endif