#include "OtobeLib/StackScene/Interface/ISceneBase.h"
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Manager/TextureManager.h"
#include "OtobeLib/FrameWork/Novel/Novel.h"
#include "ChoicesState.h"
#include "TemplateState.h"

namespace OtobeLib
{
	class ChoicesScene :public ISceneBase,StateArray<CHOICESSTATE,int>
	{
	public:
		//YES��I�����Ă���
		static const int YES = 0;

		//NO��I�����Ă���
		static const int NO = 1;
	private:
		//�摜�̊Ǘ��N���X
		std::unique_ptr<TextureManager>	m_textureManager;

		//�m�x���̊Ǘ��N���X
		std::unique_ptr<Novel>			m_novel;

		//�͂��̃e�N�X�`��
		ITextureBase*					m_yesTexture;

		//�������̃e�N�X�`��
		ITextureBase*					m_noTexture;
	public:
		//�R���X�g���N�^
		ChoicesScene();

		//�f�X�g���N�^
		~ChoicesScene()override {};

		//����������
		void Initialize()override;

		//�X�V����
		void Update()override;

		//�`�揈��
		void Render()override;

		//�I������
		void Finalize()override;

		//��������
		void Create()override;

	public:
		//���݂̏�Ԃ�ݒ肷��
		void AddData1(const CHOICESSTATE& state)override { StateArray::AddData1(state); };
		void AddData2(const int& state)override { StateArray::AddData2(state); };

		//���݂̏�Ԃ��擾����
		const CHOICESSTATE& GetNowData1()const override { return StateArray::GetNowData1(); };
		const int& GetNowData2()const override { return StateArray::GetNowData2(); };

		//�ЂƂO�̏�Ԃ��擾����
		const CHOICESSTATE& GetPreData1()const override { return StateArray::GetPreData1(); };
		const int& GetPreData2()const override { return StateArray::GetPreData2(); };

	private:
		//�X�e�[�g�̍X�V����
		void UpdateState();

		//State_Wait�̏���
		void StateStoryWait();

		//State_Input�̏���
		void StateInput();
	};
}
