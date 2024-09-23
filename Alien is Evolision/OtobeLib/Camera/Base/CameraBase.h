#pragma once

namespace OtobeLib
{
	//カメラのベースクラス
	class CameraBase
	{
	private:
		//座標
		DirectX::SimpleMath::Vector3 m_pos;

		//行列
		DirectX::SimpleMath::Matrix m_matrix;

		//オブジェクトの更新の有無
		bool						m_objectMove;
	public:
		//コンストラクタ
		CameraBase();

		//デストラクタ
		virtual ~CameraBase() {}

		//初期化
		virtual void Initialize() {}

		//更新処理
		virtual void Update() {}

		//終了処理
		virtual void Finalize() {}

		//-------------------------------セッター---------------------------------//
			//座標を設定する
		virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }

			//行列を設定する
		virtual void SetMatrix(const DirectX::SimpleMath::Matrix& matrix) { m_matrix = matrix; }

			//オブジェクトの移動の有無を設定する
		virtual void SetObjectMove(const bool& move) { m_objectMove = move; }
		//-------------------------------セッター---------------------------------//

		//-------------------------------ゲッター---------------------------------//
			//座標を設定する
		virtual const DirectX::SimpleMath::Vector3& GetPosition() { return m_pos; }

			//行列を設定する
		virtual const DirectX::SimpleMath::Matrix& GetMatrix() { return m_matrix; }

			//オブジェクトの移動の有無を設定する
		virtual const bool& GetObjectMove() { return m_objectMove; }
		//-------------------------------ゲッター---------------------------------//
	};

}

