#pragma once
#include "OtobeLib/Camera/Base/CameraBase.h"
class GameCamera:public OtobeLib::CameraBase
{
public:
	//コンストラクタ
	GameCamera();

	//デストラクタ
	~GameCamera()override {};

	//初期化
	void Initialize()override;

	//更新処理
	void Update()override;

	//終了処理
	void Finalize()override;

	//-------------------------------セッター---------------------------------//
		//座標を設定する
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) { CameraBase::SetPosition(pos); }

	//行列を設定する
	virtual void SetMatrix(const DirectX::SimpleMath::Matrix& matrix) { CameraBase::SetMatrix(matrix); }

	//オブジェクトの移動の有無を設定する
	virtual void SetObjectMove(const bool& move) { CameraBase::SetObjectMove(move); }
	//-------------------------------セッター---------------------------------//

	//-------------------------------ゲッター---------------------------------//
		//座標を設定する
	virtual const DirectX::SimpleMath::Vector3& GetPosition() { return CameraBase::GetPosition(); }

	//行列を設定する
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() { return CameraBase::GetMatrix(); }

	//オブジェクトの移動の有無を設定する
	virtual const bool& GetObjectMove() { return CameraBase::GetObjectMove(); }
	//-------------------------------ゲッター---------------------------------//
};

