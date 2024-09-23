#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"

class CutInManager
{
private:
	//カットインのテクスチャ
	OtobeLib::TextureBase* m_cutinTexture;

	//背景のテクスチャ
	OtobeLib::TextureBase* m_backTexture;

	//タイマー
	float					m_timer;

	//経過時間
	float					m_totaltimer;

public:
	//コンストラクタ
	CutInManager();

	//デストラクタ
	~CutInManager() = default;

	//初期化
	void Initialize();

	//更新
	void Update();

};

