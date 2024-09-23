#pragma once
template <typename T>
class IState
{
public:
	//バーチャルデストラクタ
	virtual ~IState(){}

	//現在の状態を設定する
	virtual void SetNowState(const T& now) = 0;

	//現在の状態を取得する
	virtual const T& GetNowState()const = 0;

	//ひとつ前の状態を取得する
	virtual const T& GetPreState()const = 0;
};

template <typename T>
class State:public IState<T>
{
public:
	//コンストラクタ
	State():m_NowState(T(-1)),m_PreState(T(-1))
	{}
public:
	//バーチャルデストラクタ
	virtual ~State()override {}

	//現在の状態を設定する
	virtual void SetNowState(const T& now)override 
	{
		//ステートが変更されたときに変更を検知する
		if (m_PreState != m_NowState)m_PreState = m_NowState;

		//ステートを更新する
		m_NowState = now; 
	};

	//現在の状態を取得する
	virtual const T& GetNowState()const override { return m_NowState; };

	//ひとつ前の状態を取得する
	virtual const T& GetPreState()const override { return m_PreState; };
private:
	T			m_NowState;	//現在の状態
	T			m_PreState;	//ひとつ前の状態
};

//StateのArrayクラス
//異なるデータ型を4つサポートする(４つ以上は、サポートしない)
template <typename T1, typename T2, typename T3 = T1, typename T4 = T2>
class StateArray
{
private:
	//データ１
	State<T1> m_stateData1;

	//データ２
	State<T2> m_stateData2;

	//データ３
	State<T3> m_stateData3;

	//データ４
	State<T4> m_stateData4;

public:
	//コンストラクタ
	StateArray() :
		m_stateData1(State<T1>()),//データ１
		m_stateData2(State<T2>()),//データ２
		m_stateData3(State<T3>()),//データ３
		m_stateData4(State<T4>())//データ４
	{}

	//デストラクタ
	virtual ~StateArray() = default;

public:
	//データを登録する
	virtual void AddData1(const T1& state) { m_stateData1.SetNowState(state); }
	virtual void AddData2(const T2& state) { m_stateData2.SetNowState(state); }
	virtual void AddData3(const T3& state) { m_stateData3.SetNowState(state); }
	virtual void AddData4(const T4& state) { m_stateData4.SetNowState(state); }

	//現在のデータを取得する
	virtual const T1& GetNowData1()const { return m_stateData1.GetNowState(); }
	virtual const T2& GetNowData2()const { return m_stateData2.GetNowState(); }
	virtual const T3& GetNowData3()const { return m_stateData3.GetNowState(); }
	virtual const T4& GetNowData4()const { return m_stateData4.GetNowState(); }

	//１つ前のデータを取得する
	virtual const T1& GetPreData1()const { return m_stateData1.GetPreState(); }
	virtual const T2& GetPreData2()const { return m_stateData2.GetPreState(); }
	virtual const T3& GetPreData3()const { return m_stateData3.GetPreState(); }
	virtual const T4& GetPreData4()const { return m_stateData4.GetPreState(); }

};

//StateのContextクラス
//参照と実態のサポートをする
class StateContext
{
public:
	//コンストラクタ
	StateContext() = default;

	//デストラクタ
	~StateContext() = default;

	//指定した型のステートを追加する
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
	//指定した型のStaticなステート
private:
	static std::unique_ptr<State<T>> s_state;

public:
	//ステートを生成する
	static void CreateState()
	{
		s_state = std::make_unique<State<T>>();
	}

	//ステートを追加する
	static void AddState(const T& state)
	{
		s_state->SetNowState(state);
	}

	//ステートを取得する
	static State<T>& GetState() { return *s_state.get(); }
};

template <typename T>
std::unique_ptr<State<T>> StaticState<T>::s_state = nullptr;