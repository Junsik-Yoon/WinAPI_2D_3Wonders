#pragma once
class CGameObject;
class CTexture;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	HIT_EFFECT,

	NONE,
};


class CCameraManager
{
	SINGLETON(CCameraManager);

private:
	Vec2 m_fptLookAt;			// 보고 있는 위치
	Vec2 m_fptCurLookAt;		// 카메라가 지금 보는 위치
	Vec2 m_fptPrevLookAt;		// 카메라가 이전에 보던 위치
	CGameObject* m_pTargetObj;	// 트래킹 할 오브젝트

	Vec2 m_fptDiff;				// 해상도 중심과 카메라 LookAt 사이의 차이

	float m_fTime = 2;			// 타겟을 따라가는 총시간
	float m_fAccTime;			// 타겟을 따라간 소요시간
	float m_fSpeed;				// 타겟을 따라가는 속도
	float m_fPreSpeed;			// 타겟을 따라가던 속도
	float m_fAccel;				// 타겟을 따라가는 등가속도
	float m_fAccDir = 1.f;		// 등가속도의 증감

	CAM_EFFECT m_eEffect;
	CTexture* m_pTex;

	float m_fEffectDuration;
	float m_fCurTime;

private:
	void CalDiff();

public:
	void init();
	void update();
	void render(HDC hDC);

	void SetLookAt(Vec2 lookAt);
	void SetTargetObj(CGameObject* target) { m_pTargetObj = target; }
	void Scroll(Vec2 vec, float velocity);

	Vec2 GetLookAt() { return m_fptCurLookAt; }	// 현재 카메라 위치 반환
	Vec2 GetRenderPos(Vec2 objPos) { return objPos - m_fptDiff; }
	Vec2 GetRealPos(Vec2 renderPos) { return renderPos + m_fptDiff; }// 렌더링 좌표에서 차이값만큼 더해주면 절대 좌표가 나옴.

	void FadeIn(float duration);
	void FadeOut(float duration);

};
