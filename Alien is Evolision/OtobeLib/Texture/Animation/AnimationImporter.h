#pragma once
#include <vector>
#include <string>

namespace OtobeLib
{
	//アニメーションの情報を送信するクラス
	class AnimationImporter
	{
	public:
		enum class ANIMATIONINFO :int
		{
			GROP,//グループ名
			ANIMATION,//アニメーション
			NEXT,//次のアニメーション
			ENDTIME,//アニメーションの終了フレーム
			TEXTURE,//使用するテクスチャ
		};
	private:
		//アニメーショングループの名前
		std::string			m_GropName;
		//アニメーションの情報
		std::string			m_Animation;
		//次に再生するアニメーションの情報
		std::string	        m_Next;
		//アニメーションの終了時間
		int					m_End;
		//使用するテクスチャ
		std::string			m_Refference;
	public:
		//コンストラクタ
		AnimationImporter();

		//コンストラクタ(引数あり)
		AnimationImporter(const AnimationImporter& importer);

		//デストラクタ
		~AnimationImporter() {}

		//グループ名を設定する
		void SetGrop(const std::string& grop) { m_GropName = grop; }

		//アニメーションの情報を設定する
		void SetAnimation(const std::string& animation) { m_Animation = animation; }

		//次のアニメーションの情報を設定する
		void SetNext(const std::string& next) { m_Next = next; }

		//アニメーションの終了時間を設定する
		void SetEnd(const int& end) { m_End = end; }

		//使用するテクスチャを設定する
		void SetRefference(const std::string& ref) { m_Refference = ref; }

		//グループ名を取得する
		const std::string& GetGrop()const { return m_GropName; }

		//アニメーションの情報を取得する
		const std::string& GetAnimation()const { return m_Animation; }

		//次のアニメーションの情報を取得する
		const std::string& GetNext()const { return m_Next; }

		//アニメーションの終了時間を設定する
		const int& GetEnd()const { return m_End; }

		//使用するテクスチャを取得する
		const std::string& GetRefference()const { return m_Refference; }
	public:
		//データをアニメーション用に変換する
		static const std::vector<AnimationImporter> Cast(const std::vector<std::vector<std::string>>& Input);
	};
}

