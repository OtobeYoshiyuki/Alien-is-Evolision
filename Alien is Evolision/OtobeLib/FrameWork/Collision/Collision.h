#pragma once
#include <DirectXMath.h>
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
namespace OtobeLib
{
	//	�n�a�a���
	typedef struct {
		DirectX::SimpleMath::Vector3 center;	//	OBB�̒��S
		DirectX::SimpleMath::Vector3 e[3];	//	���[�J�����W�n�̊e���x�N�g��
		float r[3];			//	OBB�̊e���̕��̔����̒l
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

