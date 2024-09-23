#pragma once
#include "OtobeLib/Camera/Base/CameraBase.h"
#include <list>
#include <map>
#include <string>
#include <functional>
namespace OtobeLib
{
	class CameraManager
	{
	private:
		//カメラのリスト
		std::list<std::unique_ptr<CameraBase>> m_cameraList;

		//カメラのの工場
		std::map<std::string, std::function<std::unique_ptr<CameraBase>()>> m_cameraFactory;

		//カメラの消去数
		int m_popCount;

		//生成するカメラの名前
		std::list<std::string> m_camereInfo;
	public:
		//コンストラクタ
		CameraManager();

		//デストラクタ
		~CameraManager() = default;

		//更新処理
		void Update();

		//稼働中のカメラを取得する
		CameraBase* GetNowCamera() { return m_cameraList.back().get(); }

		//カメラのインスタンスを追加する
		void AddCamera(const std::string& name,std::function<std::unique_ptr<CameraBase>()> func)
		{
			//カメラのポインタを登録する
			m_cameraFactory[name] = func;
		}

		//次のカメラに切り替える(現在稼働しているカメラを全て消す)
		void ChangeCamera(const std::string& next)
		{
			//カメラを追加生成する
			this->PushCamera(next);
			//削除数を稼働中のカメラの数にする
			this->PopCount((int)m_cameraList.size());
		}

		//シーンを追加生成する
		void PushCamera(const std::string& next)
		{
			//次の生成するシーンを追加する
			m_camereInfo.push_back(next);
		}

		//カメラを削除する
		void PopCamera(int count = 1);

	private:
		//カメラの消去数を設定する
		void PopCount(const int& count)
		{
			//カメラの削除数を設定する
			m_popCount = count;
		}

	public:
		//対象物が画面内にあるかチェックする
		const bool ScreenCheck(const DirectX::SimpleMath::Vector3& pos);
	};
}

