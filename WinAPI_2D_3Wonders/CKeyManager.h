#pragma once
class CKeyManager
{
	SINGLETON(CKeyManager);

private:
	static const int VKEY_SIZE = 0xFF;

	bool m_arrPrevKey[VKEY_SIZE];	//이전 키의 입력상태를 저장할 배열
	bool m_arrCurKey[VKEY_SIZE];	//현재 키의 입력상태를 저장할 배열
	Vec2 m_fptCurMousePos;			// 마우스 좌표

public:
	void update();
	void init();

	bool GetButton(const int key) { return (true == m_arrCurKey[key] && true == m_arrPrevKey[key]); }		//키가 눌려진 상태라면 true를 반환
	bool GetButtonDown(const int key) { return (true == m_arrCurKey[key] && false == m_arrPrevKey[key]); }	//키가 눌려진 순간만 true를 반환
	bool GetButtonUp(const int key) { return (false == m_arrCurKey[key] && true == m_arrPrevKey[key]); }	//키가 올라간 순간만 true를 반환
	Vec2 GetMousePos() { return m_fptCurMousePos; }											// 마우스 좌표 얻기

};

