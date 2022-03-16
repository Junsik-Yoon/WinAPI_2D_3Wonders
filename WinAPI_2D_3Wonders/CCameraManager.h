#pragma once
class CGameObject;
class CD2DImage;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	HIT_EFFECT,

	NONE,
};

struct tCamEffect
{
	CAM_EFFECT m_eEffect;
	float fDuration;
	float fCurTime;
};
class CCameraManager
{
	SINGLETON(CCameraManager);

private:
	Vec2 m_fptLookAt;			// ���� �ִ� ��ġ
	Vec2 m_fptCurLookAt;		// ī�޶� ���� ���� ��ġ
	Vec2 m_fptPrevLookAt;		// ī�޶� ������ ���� ��ġ
	CGameObject* m_pTargetObj;	// Ʈ��ŷ �� ������Ʈ

	Vec2 m_fptDiff;				// �ػ� �߽ɰ� ī�޶� LookAt ������ ����

	float m_fTime = 2;			// Ÿ���� ���󰡴� �ѽð�
	float m_fAccTime;			// Ÿ���� ���� �ҿ�ð�
	float m_fSpeed;				// Ÿ���� ���󰡴� �ӵ�
	float m_fPreSpeed;			// Ÿ���� ���󰡴� �ӵ�
	float m_fAccel;				// Ÿ���� ���󰡴� ��ӵ�
	float m_fAccDir = 1.f;		// ��ӵ��� ����

	list<tCamEffect> m_listCamEffect;
	CTexture* m_pImg;

private:
	void CalDiff();

public:
	void init();
	void update();
	void render();

	void SetLookAt(Vec2 lookAt);
	void SetTargetObj(CGameObject* target) { m_pTargetObj = target; }
	void Scroll(Vec2 vec, float velocity);

	Vec2 GetLookAt() { return m_fptCurLookAt; }	// ���� ī�޶� ��ġ ��ȯ
	Vec2 GetRenderPos(Vec2 objPos) { return objPos - m_fptDiff; }
	Vec2 GetRealPos(Vec2 renderPos) { return renderPos + m_fptDiff; }// ������ ��ǥ���� ���̰���ŭ �����ָ� ���� ��ǥ�� ����.

	void FadeIn(float duration);
	void FadeOut(float duration);

};
