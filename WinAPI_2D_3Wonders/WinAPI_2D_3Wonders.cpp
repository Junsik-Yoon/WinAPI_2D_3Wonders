
#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "WinAPI_2D_3Wonders.h"
#include <locale.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;                                      // 윈도우의 핸들
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

//HWND wHnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//hInstance: 실행된 프로세스의 시작주소, 인스턴스 핸들
//nCmdShow:프로그램이 시작될 형태

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    setlocale(LC_ALL, "");
    //메모리 누수 체크
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(53440);

    UNREFERENCED_PARAMETER(hPrevInstance);//안씀
    UNREFERENCED_PARAMETER(lpCmdLine);

   
    //필요한 초기화 작업들을 진행

    // 전역 문자열을 초기화합니다.
    // 리소스 뷰의 String Table에서 ID값으로 String을 가져오는 함수
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI2D3WONDERS, szWindowClass, MAX_LOADSTRING);
    //윈도우의 속성들을 정해주는 함수
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    //단축키에 대한 내용을 리소스에서 가져오는 함수
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI2D3WONDERS));
    CCore::getInst()->init();
    MSG msg;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            //게임 처리
            //게임 업데이트
            CCore::getInst()->update();
            //게임 렌더링
            CCore::getInst()->render();

        }
    }

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    //윈도우창의 정보를 저장하기 위한 구조체
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX); //구조체의 크기를 넣어준것

    wcex.style = CS_HREDRAW | CS_VREDRAW;//윈도우 클래스의 스타일 지정
    wcex.lpfnWndProc = WndProc;//함수포인터에 함수를 넣음 // 윈도우의 메세지를 처리하는 함수 WndProc지정
    wcex.cbClsExtra = 0;//윈도우클래스에서 사용하고자하는 여분의 메모리 양//잘안씀
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;//윈도우 클래스를 등록한 인스턴스 핸들 지정
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));//윈도우의 아이콘
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));//스몰아이콘
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); //윈도우에서 제공하는 기본커서중에 하나 골라서 쓰거나 커서를 추가해서 사용할 수 있음 
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//윈도우 백그라운드 컬러,게임은 어차피 렌더링하면서 계속 그리기떄문에 큰 의미가 없다
    wcex.lpszMenuName = nullptr;//윈도우의 메뉴 지정 -> nullptr로 지정하면 메뉴가 사라지게 할 수 있다
    wcex.lpszClassName = szWindowClass;
    // wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));//스몰아이콘 ->아이콘끼리 모으기위해 위치변경함

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    //윈도우를 생성하는 함수 -> hWnd에 담음
    hWnd = CreateWindowW(
        szWindowClass,      //클래스이름
        szTitle,            //윈도우 타이틀 String
        //WS_OVERLAPPEDWINDOW,//윈도우 스타일
        //WS_CAPTION | WS_SYSMENU, //이런식으로 스타일을 변경가능(최대화없애기 크기못바꾸게 등)
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //게임에서 크기가 바뀌거나 하면 곤란하기때문에 이 형태로 쓰기로
        CW_USEDEFAULT,      //윈도우 시작 X //변경하면 변경값으로 윈도우가 생성됨
        0,                  //윈도우 시작 Y
        CW_USEDEFAULT,      //윈도우 가로 크기
        0,                  //윈도우 세로 크기
        nullptr,            //부모 윈도우 핸들값
        nullptr,            //메뉴 핸들값
        hInstance,          //프로세스 인스턴스의 핸들
        nullptr             //추가 매개변수
    );

    if (!hWnd)
        return FALSE;

    RECT rc;
    rc.left = 0;
    rc.top = 0;
    rc.right = WINSIZEX;
    rc.bottom = WINSIZEY;

    //실제 내용 창이 크기에 맞게끔 하도록 사이즈를 변경
    AdjustWindowRect(&rc, WINSTYLE, false);
    //위에서 얻은 사이즈로 윈도우 사이즈를 세팅
    SetWindowPos(hWnd, NULL, WINSTARTX, WINSTARTY, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOZORDER | SWP_NOMOVE);


    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_TILEBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        //Device Context를 만들어서 ID를 반환
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);

    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
