#pragma once
#include <list>
#include <map>
#include <vector>
#include <functional>
namespace OtobeLib
{
	template<class T>
	class ICollisionBase//インターフェースクラス
	{
	public:
		//バーチャルデストラクタ(派生クラスのデストラクタを呼ぶため)
		virtual ~ICollisionBase(){}

		//他のオブジェクトと当たった時に呼ばれる関数
		virtual void OnCollisionEnter(T* obj) = 0;

		//他のオブジェクトと当たっていないときに呼ばれる関数
		virtual void NonCollisionEnter(T* obj) = 0;

		//------------セッター------------------------------------------//
		//当たったかどうかのフラグを設定する
		virtual void SetCheck(const bool& check) = 0;

		//当たったオブジェクトのIDをリストに追加する
		virtual void Add(const int& id) = 0;

		//当たったオブジェクトをリストに追加する
		virtual void Add(T* object) = 0;

		//当たり判定の関数を設定する
		virtual void AddFunc(const std::string& name, std::function<void(T*)> func) = 0;

		//当たった他のIDを設定する
		virtual void ReMove(const int& id) = 0;

		//当たった他のIDを全て消す
		virtual void ALLMove() = 0;

		//当たり判定の中心を設定する
		virtual void SetCenter(const DirectX::SimpleMath::Vector3& center) = 0;

		//当たり判定の半径を設定する
		virtual void SetRadius(const DirectX::SimpleMath::Vector3& radius) = 0;

		//---------------------------------------------------------------//

		//------------ゲッター-------------------------------------------//
		//当たったかどうかのフラグを取得する
		virtual const bool& GetCheck()const = 0;

		//自身の当たり判定を取得する
		virtual ICollisionBase* GetThisCollision() = 0;

		//リストの中から指定したIDを取得する（ただし、取得できなかったときは－1が返る）
		virtual const int Find(const int& id) = 0;

		//設定した関数を呼ぶかどうか
		virtual const bool RequestFunc(const std::string& name) = 0;

		//IDを取得する
		virtual const int& GetID()const = 0;

		//当たり判定の中心点を取得する
		virtual const DirectX::SimpleMath::Vector3& GetCenter()const = 0;

		//当たり判定の半径を取得する
		virtual const DirectX::SimpleMath::Vector3& GetRadius()const = 0;

		//当たったオブジェクトのリストを取得する
		virtual std::vector<T*>& GetObjectList() = 0;

		//-----------------------------------------------------------------//
	};
}

