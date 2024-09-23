#pragma once
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/ICollisionBase.h"
namespace OtobeLib
{
	template<class T>
	class CollisionBase:public ICollisionBase<T>
	{
	private:
		bool mCheck;			//当たったかどうかの判定

		int	 mID;				//個別のID

		static int sID;			//生成するID

		std::list<int>  mOtherID;//当たった相手のIDのリスト

		std::vector<T*>	m_OtherObject;//当たった他のオブジェクト

		//当たり判定の中心点
		DirectX::SimpleMath::Vector3 m_center;

		//当たり判定の半径
		DirectX::SimpleMath::Vector3 m_radius;

		//呼ぶ当たり判定の関数を設定する
		std::map<std::string, std::function<void(T*)>>m_callerFunc;

	public:
		//コンストラクタ
		CollisionBase():mCheck(false)
		{
			mID = sID;
			sID++;
		}

		//デストラクタ
		virtual ~CollisionBase()override {};

		//他のオブジェクトと当たった時に呼ばれる関数
		virtual void OnCollisionEnter(T* obj)override
		{
			//当たり判定のフラグを立てる
			this->SetCheck(true);
		};

		//他のオブジェクトと当たっていないときに呼ばれる関数
		virtual void NonCollisionEnter(T* obj)override
		{
			////当たり判定のフラグを立てる
			//this->SetCheck(false);
		};

		//------------セッター------------------------------------------//
		//当たったかどうかのフラグを設定する
		void SetCheck(const bool& check)override { mCheck = check; };

		//当たったオブジェクトのIDをリストに追加する
		void Add(const int& id)override { mOtherID.push_back(id); };

		//当たったオブジェクトを追加する
		void Add(T* obj)override { m_OtherObject.push_back(obj); }

		//当たり判定の中心を設定する
		virtual void SetCenter(const DirectX::SimpleMath::Vector3& center)override { m_center = center; };

		//当たり判定の半径を設定する
		virtual void SetRadius(const DirectX::SimpleMath::Vector3& radius)override { m_radius = radius; };

		//当たり判定の関数を設定する
		void AddFunc(const std::string& name, std::function<void(T*)> func)override
		{
			m_callerFunc[name] = func;
		}


		//当たった他のIDを設定する
		void ReMove(const int& id)override 
		{
			for (int& otherid : mOtherID)
			{
				if (otherid == id)
				{
					mOtherID.remove(id);
					return;
				}
			}
		};

		//当たった他のIDを全て消す
		void ALLMove()override { mOtherID.clear(); };
		//-----------------------------------------------------------------//

		//------------ゲッター-------------------------------------------//
		//当たったかどうかのフラグを取得する
		virtual const bool& GetCheck()const override { return mCheck; };

		//IDを取得する
		const int& GetID()const override { return mID; };

		//自身の当たり判定を取得する
		virtual ICollisionBase* GetThisCollision()override { return this; };

		//リストの中から指定したIDを取得する（ただし、取得できなかったときは－1が返る）
		const int Find(const int& id)override 
		{
			for (int& otherid : mOtherID)
			{
				if (otherid == id)return otherid;
			}
			return -1;
		};

		//設定した関数を呼ぶかどうか
		const bool RequestFunc(const std::string& name)override
		{
			//イテレータを宣言する
			std::map<std::string, std::function<void(T*)>>::const_iterator iter;

			//指定した関数を探す
			iter = m_callerFunc.find(name);

			//指定した関数が見つからなかった(iterがend)
			if (iter == m_callerFunc.end())return false;

			return true;
		}

		//当たり判定の中心を設定する
		virtual const DirectX::SimpleMath::Vector3& GetCenter()const override { return m_center; }

		//当たり判定の半径を設定する
		virtual const DirectX::SimpleMath::Vector3& GetRadius()const override { return m_radius; }

		std::vector<T*>& GetObjectList()override { return m_OtherObject; }

		//-----------------------------------------------------------------------//

		//IDを生成する
		static int CreateID() { return sID; };
	};

	template <class T>
	int CollisionBase<T>::sID = NULL;
}

