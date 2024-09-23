#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
class MiniMap
{
private:
	//ミニマップのワールド行列
	DirectX::SimpleMath::Matrix m_world;

	//ミニマップ用の配列
	std::vector<std::vector<OtobeLib::ITextureBase*>> m_miniMapTex;

	//動くミニマップ用の配列
	std::vector<OtobeLib::ITextureBase*>	m_moveTex;

	//RECTアニメーション用のミニマップの配列
	std::vector<OtobeLib::ITextureBase*>	m_miniAnimeTex;

	//RECTアニメーション用のゲームデータ用の配列
	std::vector<OtobeLib::ITextureBase*>	m_gameAnimeTex;

	//ステージのゲームデータ用の配列
	std::vector<OtobeLib::ITextureBase*>	m_gameTex;
public:
	//コンストラクタ
	MiniMap();

	//デストラクタ
	~MiniMap() {};

	//初期化
	void Initialize(std::vector<std::vector<std::string>>& before,std::vector<std::vector<int>>& after);

	//更新
	void Update();

	//描画
	void Render();

public:
	//ワールド行列を取得する
	const DirectX::SimpleMath::Matrix& GetWorld()const { return m_world; }

	//配列の情報を設定する
	void ArraySetUp();

	//配列の中身をチェックする
	const bool CheckArray(const std::string& target);

	//配列の中身が子供かどうか確認する
	const bool CheckChild(const std::string& child);

	//配列の中身がプレイヤー関連のものかどうか
	const bool CheckPlayers(const std::string& players);

	//ミニマップの配列を取得する
	std::vector<std::vector<OtobeLib::ITextureBase*>>& GetMiniMap() { return m_miniMapTex; }

private:
	//ミニマップのインスタンスを生成する
	void CreateMiniMap(std::unique_ptr<OtobeLib::ITextureBase>& pchild,const int& col,
		const DirectX::XMVECTORF32& color, const std::string& name,const RECT& rect);
};

