#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include <vector>
#include <string>
#include <map>


class HoleFloor :public MapTip
{
	//型名が長いので省略
	using HoleList = std::list<HoleFloor*>;
	using HoleMap = std::map<int, HoleList>;
private:
	//爆発するカウント
	 float m_ExprosionCount;
	//カウントを更新するフラグ
	 bool m_ExprosionFlag;
	 //シリアルナンバー
	 int	m_SelealNumber;
	 //個別の名前
	 std::string m_SelealName;
	 //落とし穴をグループとして管理する
	 static HoleMap s_holeGrop;
public:
	//コンストラクタ
	HoleFloor(OtobeLib::ITextureBase* texture);

	//デストラクタ
	~HoleFloor()override {};

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

	//--------------------------------------------------------------//

public:
	//爆発するタイミングを設定する
	void SetExproTiming(const float& timing) { m_ExprosionCount = timing; }

	//ファイルから読み込んだデータをconvertする
	static const std::vector<float> FileDataConvert(const std::vector<std::vector<std::string>>& Input);

	//シリアルナンバーを設定する
	void SetSerealNumber(const int& number) { m_SelealNumber = number; }

	//シリアルナンバーを取得する
	const int& GetSerealNumber()const { return m_SelealNumber; }

	//指定したグループを取得する
	static HoleList& GetHoleGrop(const int& number) { return s_holeGrop[number]; }
	
	//グループを解除する
	static void ResetHoleGrop() { s_holeGrop.clear(); }
private:
	//爆発するタイミングを更新する
	void UpdateExprosion();
};

