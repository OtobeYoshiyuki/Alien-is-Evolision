#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
class Goal:public MapTip
{
public:
	//コンストラクタ
	Goal(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~Goal()override {};

	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Render()override;

	//終了
	void Finalize()override;

	//他のオブジェクトと当たった時に呼ばれる関数
	void OnCollisionEnter(ITextureBase* obj)override;

	//他のオブジェクトと当たっていないときに呼ばれる関数
	void NonCollisionEnter(ITextureBase* obj)override;
};

