#include "pch.h"
#include "CameraManager.h"
#include "Utils.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CameraManager::CameraManager():
	m_popCount(NULL)//削除数
{
}

void CameraManager::Update()
{
	//カメラの削除及び、終了処理
	for (int i = 0; i < m_popCount; i++)
	{
		//稼働しているシーンに対して、行う
		m_cameraList.back()->Finalize();
		m_cameraList.pop_back();
	}

	//削除カウントを初期化する
	m_popCount = NULL;

	//カメラの生成を行う
	for (std::string& info : m_camereInfo)
	{
		//次のカメラを生成する
		m_cameraList.push_back(m_cameraFactory[info]());
		//カメラの初期化を行う
		m_cameraList.back()->Initialize();
	}

	//カメラの情報をクリアする
	m_camereInfo.clear();

	//カメラの更新を行う
	if (!m_cameraList.empty())
	{
		m_cameraList.back()->Update();
	}
}

void ::CameraManager::PopCamera(int count)
{
	//変数の初期化
	int Inspection = NULL;

	//追加したものを消して、消去数を減らす
	if (!m_camereInfo.empty())
	{
		m_camereInfo.pop_back();
		count--;
	}

	//カメラの消す数を補正する
	Inspection = std::max<int>(count, 0);
	Inspection = std::min<int>(Inspection, (int)m_cameraList.size() - 1);

	//消去数を設定する
	this->PopCount(Inspection);
}

//対象物が画面内にあるか確認する
const bool CameraManager::ScreenCheck(const Vector3& pos)
{
	//稼働中のカメラを取得する
	CameraBase* nowCamera = this->GetNowCamera();

	//行列から、座標を取り出す
	Vector3 trans = nowCamera->GetMatrix().Translation();

	//取り出した座標は、マイナスなので、絶対値に直す
	trans = Length(trans, Vector3::Zero);

	//判定は、X座標のみで行う
	//対象物が画面内にある時は、true
	if (pos.x >= trans.x && pos.x <= trans.x + Game::MAX_WIDTH)return true;

	//それ以外の時は、falseを返す
	return false;
}
