#pragma once

class CEffect;
class CD2DImage;

class CEffectManager
{
	SINGLETON(CEffectManager);
private:
	map<wstring, CEffect*> m_effectTex;
public:

};

