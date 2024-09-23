#pragma once
#include "Effect/Star/Star.h"
#include <list>
class StarManager
{
private:
	//中心の星
	Star* m_MainStar;

	//はじける星のリスト
	std::list<Star*>	m_PopStars;

	//はじける星（青）
	Star* m_PopStarBlue;

	//はじける星（赤）
	Star* m_PopStarRed;
public:
	//コンストラクタ
	StarManager();

	//デストラクタ
	~StarManager(){}

	//初期化処理
	void Initialize(const DirectX::SimpleMath::Vector3& pos);

	//更新処理
	void Update();

public:

	//中心の星を取得する
	Star* GetMainStar() { return m_MainStar; }

	//はじける星のリストを取得する
	std::list<Star*>& GetPopStarList() { return m_PopStars; }

	//はじける赤い星を取得する
	Star* GetPopStarRed() { return m_PopStarRed; }

	//はじける青い星を取得する
	Star* GetPopStarBlue() { return m_PopStarBlue; }

private:
	//中心の星の更新処理
	void UpdateMainStar();

	//はじける赤い星の更新処理
	void UpdatePopStarRed();

	//はじける青い星の更新処理
	void UpdatePopStarBlue();

};

