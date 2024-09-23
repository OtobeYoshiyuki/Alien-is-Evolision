#pragma once
#include "DeviceResources.h"
#include <CommonStates.h>
#include <SpriteBatch.h>
#include "OtobeLib/FrameWork/Singleton/Singleton.h"

namespace OtobeLib
{
	class Graphics:public Singleton<Graphics>
	{
		friend class Singleton<Graphics>;//��{�I�Ƀ����o�ɃA�N�Z�X���Ȃ�
	private:
		//SpriteBatch�̃��j�[�N�|�C���^
		std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

		//CommonStates�̃��j�[�N�|�C���^
		std::unique_ptr<DirectX::CommonStates>m_State;
	private:
		//�R���X�g���N�^
		Graphics();
	public:
		//�f�X�g���N�^
		~Graphics();
	public:
		//SpriteBatch���擾
		DirectX::SpriteBatch * GetSpriteBatch()const;

		//CommoStates���擾
		DirectX::CommonStates * GetCommonStates()const;
	private:
		//�f�o�C�X����
		void Create();
	};
}

