#pragma once
class CAnimator;
class CD2DImage;

struct tAniFrm
{
	Vec2 fptLT;
	Vec2 fptSlice;
	Vec2 fptOffset;
	float fDuration;
	bool loop;
};

class CAnimation
{
	friend class CAnimator;

private:
	wstring			m_strName;		// �ִϸ��̼� �̸�
	CAnimator* m_pAnimator;	// �ִϸ�����
	CD2DImage* m_pImg;			// �ִϸ��̼� �̹���
	vector<tAniFrm> m_vecFrm;		// ��� �������� �ڸ��� ���� �� �����ð�
	int				m_iCurFrm;		// ���� �������� index
	float			m_fAccTime;		// ���� �����ӱ��� �����ð�
	bool			m_bReverse;
	bool			m_loop;

public:
	void update();
	void render();

public:
	void SetName(const wstring& strName) { m_strName = strName; }
	const wstring& GetName() { return m_strName; }
	void SetFrame(int frmIndex) { m_iCurFrm = frmIndex; }
	tAniFrm& GetFrame(int frmIndex) { return m_vecFrm[frmIndex]; }

	void SetOffset(Vec2 _offset, int _index) { m_vecFrm[_index].fptOffset = _offset; }

	void Create(CD2DImage* Img, Vec2 lt, Vec2 slice, Vec2 step,
		float duration, UINT frmCount);

public:
	CAnimation();
	~CAnimation();
};

