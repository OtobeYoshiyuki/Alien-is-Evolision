#pragma once
#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include "OtobeLib/StackScene/SceneState/SceneState.h"
#include <functional>
namespace OtobeLib
{
	class SceneManager :public Singleton<SceneManager>
	{
		friend Singleton<SceneManager>;
	private:
		//シーンのリスト
		std::list<std::unique_ptr<ISceneBase>>m_SceneList;


		//シーンの生成を行うファクトリー
		std::map<std::string, std::function<std::unique_ptr<ISceneBase>()>> m_SceneFactory;

		//シーンの情報
		std::list<std::string> m_SceneInfo;

		//シーンを削除する数
		int			m_PopCount;

		//フェードの管理
		std::unique_ptr<SceneState>m_State;
	private:
		//コンストラクタ
		SceneManager();
	public:
		//デストラクタ
		~SceneManager() {};

		//更新
		void Update();

		//描画
		void Render();

		//稼働中のシーンを取得する
		ISceneBase* GetNowScene() { return m_SceneList.back().get(); }

		//フェードのステートを取得する
		SceneState* GetState() { return m_State.get(); }

		//シーンの情報を追加する
		void AddScene(const std::string& name, std::function<std::unique_ptr<ISceneBase>()> func)
		{
			//std::make_unique関数を設定する
			m_SceneFactory[name] = func;
		}

		//次のシーンに切り替える(現在稼働しているシーンを全て消す)
		void ChangeScene(const std::string& next) 
		{ 
			//シーンを追加生成する
			this->PushScene(next);
			//削除数を稼働中のシーンの数にする
			this->PopCount((int)m_SceneList.size());
		}

		//シーンを追加生成する
		void PushScene(const std::string& next)
		{
			//次の生成するシーンを追加する
			m_SceneInfo.push_back(next);
		}

		//シーンを削除する
		void PopScene(int count = 1);

	private:
		//シーンの消去数を設定する
		void PopCount(const int& count)
		{
			//シーンの削除数を設定する
			m_PopCount = count;
		}
	};
}

