#pragma once
template <typename T>
class IState
{
public:
	//�o�[�`�����f�X�g���N�^
	virtual ~IState(){}

	//���݂̏�Ԃ�ݒ肷��
	virtual void SetNowState(const T& now) = 0;

	//���݂̏�Ԃ��擾����
	virtual const T& GetNowState()const = 0;

	//�ЂƂO�̏�Ԃ��擾����
	virtual const T& GetPreState()const = 0;
};

template <typename T>
class State:public IState<T>
{
public:
	//�R���X�g���N�^
	State():m_NowState(T(-1)),m_PreState(T(-1))
	{}
public:
	//�o�[�`�����f�X�g���N�^
	virtual ~State()override {}

	//���݂̏�Ԃ�ݒ肷��
	virtual void SetNowState(const T& now)override 
	{
		//�X�e�[�g���ύX���ꂽ�Ƃ��ɕύX�����m����
		if (m_PreState != m_NowState)m_PreState = m_NowState;

		//�X�e�[�g���X�V����
		m_NowState = now; 
	};

	//���݂̏�Ԃ��擾����
	virtual const T& GetNowState()const override { return m_NowState; };

	//�ЂƂO�̏�Ԃ��擾����
	virtual const T& GetPreState()const override { return m_PreState; };
private:
	T			m_NowState;	//���݂̏��
	T			m_PreState;	//�ЂƂO�̏��
};

//State��Array�N���X
//�قȂ�f�[�^�^��4�T�|�[�g����(�S�ȏ�́A�T�|�[�g���Ȃ�)
template <typename T1, typename T2, typename T3 = T1, typename T4 = T2>
class StateArray
{
private:
	//�f�[�^�P
	State<T1> m_stateData1;

	//�f�[�^�Q
	State<T2> m_stateData2;

	//�f�[�^�R
	State<T3> m_stateData3;

	//�f�[�^�S
	State<T4> m_stateData4;

public:
	//�R���X�g���N�^
	StateArray() :
		m_stateData1(State<T1>()),//�f�[�^�P
		m_stateData2(State<T2>()),//�f�[�^�Q
		m_stateData3(State<T3>()),//�f�[�^�R
		m_stateData4(State<T4>())//�f�[�^�S
	{}

	//�f�X�g���N�^
	virtual ~StateArray() = default;

public:
	//�f�[�^��o�^����
	virtual void AddData1(const T1& state) { m_stateData1.SetNowState(state); }
	virtual void AddData2(const T2& state) { m_stateData2.SetNowState(state); }
	virtual void AddData3(const T3& state) { m_stateData3.SetNowState(state); }
	virtual void AddData4(const T4& state) { m_stateData4.SetNowState(state); }

	//���݂̃f�[�^���擾����
	virtual const T1& GetNowData1()const { return m_stateData1.GetNowState(); }
	virtual const T2& GetNowData2()const { return m_stateData2.GetNowState(); }
	virtual const T3& GetNowData3()const { return m_stateData3.GetNowState(); }
	virtual const T4& GetNowData4()const { return m_stateData4.GetNowState(); }

	//�P�O�̃f�[�^���擾����
	virtual const T1& GetPreData1()const { return m_stateData1.GetPreState(); }
	virtual const T2& GetPreData2()const { return m_stateData2.GetPreState(); }
	virtual const T3& GetPreData3()const { return m_stateData3.GetPreState(); }
	virtual const T4& GetPreData4()const { return m_stateData4.GetPreState(); }

};

//State��Context�N���X
//�Q�ƂƎ��Ԃ̃T�|�[�g������
class StateContext
{
public:
	//�R���X�g���N�^
	StateContext() = default;

	//�f�X�g���N�^
	~StateContext() = default;

	//�w�肵���^�̃X�e�[�g��ǉ�����
	template <typename T>
	static void CreateState() { StaticState<T>::CreateState(); }

	template <typename T>
	static void AddState(const T& state) { State<T>::AddState(state); }

	template <typename T>
	static State<T>& GetState() { return StaticState<T>::GetState(); }
};

template <typename T>
class StaticState
{
	//�w�肵���^��Static�ȃX�e�[�g
private:
	static std::unique_ptr<State<T>> s_state;

public:
	//�X�e�[�g�𐶐�����
	static void CreateState()
	{
		s_state = std::make_unique<State<T>>();
	}

	//�X�e�[�g��ǉ�����
	static void AddState(const T& state)
	{
		s_state->SetNowState(state);
	}

	//�X�e�[�g���擾����
	static State<T>& GetState() { return *s_state.get(); }
};

template <typename T>
std::unique_ptr<State<T>> StaticState<T>::s_state = nullptr;