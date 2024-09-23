#pragma once
#include "DeviceResources.h"
#include <CommonStates.h>
#include <SpriteBatch.h>
#include "OtobeLib/FrameWork/Singleton/Singleton.h"

namespace OtobeLib
{
	class Graphics:public Singleton<Graphics>
	{
		friend class Singleton<Graphics>;//基本的にメンバにアクセスしない
	private:
		//SpriteBatchのユニークポインタ
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

		//CommonStatesのユニークポインタ
		std::unique_ptr<DirectX::CommonStates>m_State;
	private:
		//コンストラクタ
		Graphics();
	public:
		//デストラクタ
		~Graphics();
	public:
		//SpriteBatchを取得
		DirectX::SpriteBatch * GetSpriteBatch()const;

		//CommoStatesを取得
		DirectX::CommonStates * GetCommonStates()const;
	private:
		//デバイス生成
		void Create();
	};
}

