#pragma once
class CAnimator;
class CTexture;

struct tAniFrm
{
	Vec2 fptLT;
	Vec2 fptSlice;
	Vec2 fptOffset;
	float fDuration;
};

class CAnimation
{
	friend class CAnimator;

private:
	wstring			m_strName;		// 애니메이션 이름
	CAnimator* m_pAnimator;	// 애니메이터
	CTexture* m_pTex;			// 애니메이션 이미지
	vector<tAniFrm> m_vecFrm;		// 모든 프레임의 자르기 영역 및 유지시간
	int				m_iCurFrm;		// 현재 프레임의 index
	float			m_fAccTime;		// 다음 프레임까지 축적시간

public:
	void update();
	void render(HDC hDC);

public:
	void SetName(const wstring& strName) { m_strName = strName; }
	const wstring& GetName() { return m_strName; }
	void SetFrame(int frmIndex) { m_iCurFrm = frmIndex; }
	tAniFrm& GetFrame(int frmIndex) { return m_vecFrm[frmIndex]; }

	void Create(CTexture* tex, Vec2 lt, Vec2 slice, Vec2 step,
		float duration, UINT frmCount);

public:
	CAnimation();
	~CAnimation();
};

