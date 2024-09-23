#pragma once
#include <DirectXMath.h>
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
namespace OtobeLib
{
	//	ＯＢＢ情報
	typedef struct {
		DirectX::SimpleMath::Vector3 center;	//	OBBの中心
		DirectX::SimpleMath::Vector3 e[3];	//	ローカル座標系の各軸ベクトル
		float r[3];			//	OBBの各軸の幅の半分の値
	} OBB;
	class Collision:public Singleton<Collision>
	{
		friend Singleton<Collision>;
	public:
		Collision();
		~Collision();
		static bool HitCheck_AABB(ITextureBase* obj1, ITextureBase* obj2);
		static bool HitCheck_OBB(ITextureBase* obj1, ITextureBase* obj2);
		static bool HitCheck_Circle(ITextureBase* obj1, ITextureBase* obj2);
		static bool HitCheck_Circle(const DirectX::SimpleMath::Vector3& pos1,
			const DirectX::SimpleMath::Vector3& pos2, const float& radius);
		static bool HitCheck_Mouse_To_Texture(ITextureBase* tex);
	};
}

