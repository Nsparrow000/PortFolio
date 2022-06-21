//=============================================================================
//
// テクスチャ処理 [texture.cpp]
// Author : 羽鳥太一作
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "texture.h"
#include "Renderer.h"
#include "manager.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// テクスチャの生成
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する

	m_pTexture[None] = nullptr;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/text_texture.png",//仮テクスチャ
		&m_pTexture[Text]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Flag.png",//仮テクスチャ
		&m_pTexture[GOAL]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number000.png",//数字
		&m_pTexture[TIME]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/fade.png",//数字
		&m_pTexture[FADE]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/sky_loop001.png",//空
		&m_pTexture[SKY]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",//影
		&m_pTexture[SHADOW]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Fire.png",//火
		&m_pTexture[FIRE]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/MagicCircle_TypeB.png",//魔法陣
		&m_pTexture[MagicCircle_TypeB]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Ring001.png",//ring
		&m_pTexture[RingEffect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/HeelEffect.jpg",//回復系の画像
		&m_pTexture[LifeEffect]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/shadow000.jpg",//エフェクト
		&m_pTexture[Effect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/GlitterEffect000.png",//エフェクト
		&m_pTexture[GlitterEffect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/hiteffect000.jpg",//エフェクト
		&m_pTexture[HitEffect]);

	//============================================================
	//エフェクトテクスチャ
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/CycleEffect00.png",//わっかエフェクト
		&m_pTexture[CycleEffect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Target0.png",//ミサイル着弾予定地エフェクト
		&m_pTexture[TrgetLock]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Explosion.png",//爆発ドームエフェクト
		&m_pTexture[Explosion]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Thunder3.png",//サンダー
		&m_pTexture[Thunder]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Crater.png",//サンダー
		&m_pTexture[Crater]);



	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Trajectory000.png",//エフェクト
		&m_pTexture[SwordEffect]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/GaugeBer.png",//ゲージ
		&m_pTexture[GAUGEBER]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/GaugeBG000.png",//ゲージの枠
		&m_pTexture[GAUGEFRAME]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BG000.png",//ゲージの枠
		&m_pTexture[COL_AREA]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Field000.jpg",//床	
		&m_pTexture[FIELD]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Map_Player000.png",//床	
		&m_pTexture[Map_Player]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/MapEnemy.png",//床	
		&m_pTexture[Map_Enemy]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Purpose000.png",//床	
		&m_pTexture[Map_Purpose]);
	//============================================================
	//スキル関係
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SKILL/ATKup.png",//Aボタン
		&m_pTexture[ATKup]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SKILL/Beam.png",//Aボタン
		&m_pTexture[Beam]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SKILL/BlackHole.png",//Aボタン
		&m_pTexture[BlackHole]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SKILL/Heal.png",//Aボタン
		&m_pTexture[Heal]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SKILL/OverHeal.png",//Aボタン
		&m_pTexture[OverHeal]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SKILL/Rocket.png",//Aボタン
		&m_pTexture[Rocket]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SKILL/RushAttack.png",//Aボタン
		&m_pTexture[RushAttack]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SKILL/Sheild.png",//Aボタン
		&m_pTexture[Sheild]);
	//============================================================
	//ボタン
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/A_button.png",//Aボタン
		&m_pTexture[A_BUTTON]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/B_button.png",//Aボタン
		&m_pTexture[B_BUTTON]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/X_button.png",//Aボタン
		&m_pTexture[X_BUTTON]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/L_button.png",//Aボタン
		&m_pTexture[L_BUTTON]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/BUTTON/SkillGauge000.png",//ゲージ
		&m_pTexture[SKILLGAUGE]);

	//============================================================
	//タイトル
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Title000.png",//タイトル背景
		&m_pTexture[Title]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Please Enter.png",//ゲームスタート
		&m_pTexture[GameStart]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/operation_method000.png",//チュートリアル(ゲーム中に表示する操作方法)
		&m_pTexture[Operation]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Cursor.png",		  //カーソル
		&m_pTexture[Cursol]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title_file01.png",
		&m_pTexture[Title_File1]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title_file02.png",
		&m_pTexture[Title_File2]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/title_file03.png",
		&m_pTexture[Title_File3]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/datadelete.png",
		&m_pTexture[Title_Delete]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/databack.png",
		&m_pTexture[Title_Back]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/backtitle.png",
		&m_pTexture[Menu_Back]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/menu_gamestart.png",
		&m_pTexture[Menu_GameStart]);

	//============================================================
	//ゲームオーバー
	//============================================================

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/FAILED000.png",//ゲームオーバーロゴ
		&m_pTexture[Gameover000]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Result000.png",//ゲームクリア背景
		&m_pTexture[GameClear000]);
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Retry000.png",//リトライ
		&m_pTexture[Retry]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/titleback.png",//タイトルに戻る
		&m_pTexture[TitleBack001]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Gameback.png",//ゲームに戻る
		&m_pTexture[GameBack]);
	//============================================================
	//ゲーム画面
	//============================================================
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Tutorial000.png",//チュートリアル画面
		&m_pTexture[Operation01]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/targettext000.png",//目的地表示用の
		&m_pTexture[TargetText]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/stagecursor.png",//ロックオン用
		&m_pTexture[RockOn]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/SkillOK000.png",//ロックオン用
		&m_pTexture[SkillOk]);

}

//=============================================================================
// 終了
//=============================================================================
void CTexture::Uniinit(void)
{
	for (int nCntTexture = 0; nCntTexture < Max; nCntTexture++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
}