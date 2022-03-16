#pragma once
//Object Group
enum class GROUP_GAMEOBJ
{
	BACKGROUND,
	DEFAULT,
	TILE,
	PLAYER,
	MONSTER,
	MISSILE_PLAYER,
	MISSILE_MONSTER,

	UI,
	SIZE,
};
//Scene Group
enum class GROUP_SCENE
{
	DEFAULT,
	START,
	STAGE1,
	STAGE2,
	TOOL,

	SIZE,
};


//GDI
enum class TYPE_BRUSH
{
	HOLLOW,

	SIZE,
};

enum class TYPE_PEN
{
	RED,
	GREEN,
	BLUE,

	SIZE,
};

//event type
enum class TYPE_EVENT
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	CHANGE_SCENE,

	SIZE,
};
//tile type
enum class GROUP_TILE
{
	NONE,
	GROUND,
	WALL,
	DOOR,

	SIZE,
};

//library
#pragma comment(lib,"Msimg32.lib")


//STL, etc
#include <string>
#include <vector>
#include <map>
#include <list>
using std::string;
using std::wstring;
using std::vector;
using std::make_pair;
using std::map;
using std::list;
//
#include <chrono>
#include <assert.h>
//#include "Logger.h"
#include "framework.h"
#include "struct.h"
//singleton
#define SINGLETON(type)								\
													\
public:												\
	static type* getInst()							\
	{												\
		static type _instance;						\
		return &_instance;							\
	}												\
private:											\
	type();											\
	~type();	
//core
#include "CCore.h"
//managers
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"
#include "CCollisionManager.h"
#include "CEventManager.h"
#include "CResourceManager.h"
#include "CCameraManager.h"
#include "CUIManager.h"
#include "CSoundManager.h"
#include "CRenderManager.h"


//macro
#define WINSTYLE WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU 
#define WINSTARTX 100
#define WINSTARTY 100
#define WINSIZEX 600
#define WINSIZEY 450
//time manager
#define fDT CTimeManager::getInst()->GetfDT()
#define DT CTimeManager::getInst()->GetDT()
//key manager
#define KEY(vk_key) CKeyManager::getInst()->GetButton(vk_key)
#define KEYDOWN(vk_key) CKeyManager::getInst()->GetButtonDown(vk_key)
#define KEYUP(vk_key) CKeyManager::getInst()->GetButtonUp(vk_key)
#define MousePos()		CKeyManager::getInst()->GetMousePos()
//event manager
#define CreateObj(pObj, group)	CEventManager::getInst()->EventCreateObject(pObj, group)
#define DeleteObj(pObj)			CEventManager::getInst()->EventDeleteObject(pObj)
#define ChangeScn(scene)		CEventManager::getInst()->EventChangeScene(scene)
//pi
#define PI 3.1415926535f


//핸들 전역변수
extern HINSTANCE hInst;
extern HWND hWnd;