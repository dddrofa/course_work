#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib,"Ws2_32.lib")
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <winsock2.h> // Wincosk2.h ������ ���� ������ windows!
#include <vector>
#include <windows.h>
#define MAIN_STATICBOX 104
#define BUFFER_SIZE 2048
#define WM_ACCEPT WM_USER + 1
using namespace std;
SOCKET s;
DWORD WINAPI ReceivingMessages(LPVOID client_socket);
char name[16] = "������";
char buff[1024];
int nsize;
wchar_t* getwc(const char* c) {
    size_t iSize = strlen(c) + 1;
    wchar_t* wc = (wchar_t*)calloc(iSize, sizeof(wchar_t));
    mbstowcs(wc, c, iSize);
    return wc;
}

HWND hwnd; // ���������� ����


LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM); // ������� ��������� ��������� ����
static HWND hStat; // ���������� ������������ ������
// ��������� �������
int  WINAPI  WinMain(HINSTANCE  hInstance,
    HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    setlocale(LC_ALL, "Russian");
    char response[1024] = { 0 };
    WSADATA ws;
    if (FAILED(WSAStartup(MAKEWORD(1, 1), &ws)))
    {
        WSAGetLastError();
    }
    else {
      //  cout << "[WSAStartup]: SPI - Success" << endl;
    }

  
    if (INVALID_SOCKET == (s = socket(AF_INET, SOCK_STREAM, 0)))
    {
        WSAGetLastError();
    }
    else {
       // cout << "[Socket]: SPI - Success" << endl;
    }

    sockaddr_in adr;
    adr.sin_family = AF_INET;
   // cout << "������� ���� ���: ";
   
    
    //cout << "������� ����� �����������(local/host): ";
    
        adr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
        adr.sin_port = htons(80);
   
    if (SOCKET_ERROR == (connect(s, (sockaddr*)&adr, sizeof(adr))))
    {
         WSAGetLastError();
    }
    else {
        //cout << "[Connect]: SPI - Success\n\n\n" << endl;
    }
        DWORD thID;
        CreateThread(NULL, NULL, ReceivingMessages, &s, NULL, &thID);



    MSG msg;   // ��������� ���������
    WNDCLASS w; // ��������� ������ ����
    memset(&w, 0, sizeof(WNDCLASS)); // ������� ������ ��� ���������
    w.style = CS_HREDRAW | CS_VREDRAW;
    w.lpfnWndProc = WndProc;
    w.hInstance = hInstance;
    w.hbrBackground = CreateSolidBrush(0x00FFFFFF);
    w.lpszClassName = L"MyClass";
    RegisterClass(&w); // ����������� ������ ����
    // �������� ����
    hwnd = CreateWindow(L"MyClass", L"������",
        WS_OVERLAPPEDWINDOW,
        500, 500, 500, 380,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow); // ����������� ����
    UpdateWindow(hwnd);         // ����������� ����
    // ���� ��������� ���������
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
// ������� ��������� ���������
LONG WINAPI WndProc(HWND hwnd, UINT Message,
    WPARAM wparam, LPARAM lparam) {
    HDC hdc;
    HINSTANCE hInst;
    PAINTSTRUCT ps;
    static HWND hBtn; // ���������� ������
    static HWND hEdt1, hEdt2; // ����������� ����� ��������������
   
    CHAR StrA[20];
    int a, b, sum, Len;
 
    switch (Message) {

    case WM_CREATE: // ��������� �������� ����
        hInst = ((LPCREATESTRUCT)lparam)->hInstance; // ���������� ����������
        // ������� � ���������� ������ ���� ��������������
        hEdt1 = CreateWindow(L"edit", L"80",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 50, 300, 150, 30,
            hwnd, 0, hInst, NULL);
        ShowWindow(hEdt1, SW_SHOWNORMAL);
        // ������� � ���������� ������ ���� ��������������
        hEdt2 = CreateWindow(L"edit", L"������",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER ,150, 0, 150,
            20, hwnd, 0, hInst, NULL);
        ShowWindow(hEdt2, SW_SHOWNORMAL);
        // ������� � ���������� ������
        hBtn = CreateWindow(L"button", L"���������",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            250, 300, 150, 30, hwnd, 0, hInst, NULL);
        ShowWindow(hBtn, SW_SHOWNORMAL);
        // ������� � ���������� ���� ������ ��� ����������
        hStat = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY ,
            10, 30, 400,250, hwnd, (HMENU)MAIN_STATICBOX, hInst, NULL);
        ShowWindow(hStat, SW_SHOWNORMAL);
        break;
    case WM_COMMAND:  // ��������� � �������
        if (lparam == (LPARAM)hBtn)    // ���� ������ �� ������
        {
           
            size_t   i;
            char* buff = (char*)malloc(BUFFER_SIZE);
            wchar_t* Mes_wc = (wchar_t*)malloc(BUFFER_SIZE);
            GetWindowText(hEdt1, Mes_wc, 20);


            // Conversion
            wcstombs_s(&i, buff, (size_t)BUFFER_SIZE,
                Mes_wc, (size_t)BUFFER_SIZE);

            char* Name_convert = (char*)malloc(BUFFER_SIZE);
            wchar_t* Name = (wchar_t*)malloc(BUFFER_SIZE);;;
            GetWindowText(hEdt2, Name, 20);


            // Conversion
            wcstombs_s(&i, Name_convert, (size_t)BUFFER_SIZE,
                Name, (size_t)BUFFER_SIZE);



            SendDlgItemMessage(hwnd, MAIN_STATICBOX, EM_REPLACESEL, 0, (LPARAM)Name);
            SendDlgItemMessage(hwnd, MAIN_STATICBOX, EM_REPLACESEL, 0, (LPARAM)L": ");
            // �������� ������ ������� �������
            SendDlgItemMessage(hwnd, MAIN_STATICBOX, EM_REPLACESEL, 0, (LPARAM)Mes_wc);
            SendDlgItemMessage(hwnd, MAIN_STATICBOX, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
            send(s, strcat(strcat(Name_convert, ": "), buff), nsize, 0);
        
            
            //SetWindowText(hStat, Mes_wc); // ������� ��������� � ����������� ����
        
        }
        break;
    case WM_PAINT: // ����������� ����
        hdc = BeginPaint(hwnd, &ps); // ������ �����������
      //  TextOut(hdc, 50, 300, L"������� ����       ", 18); // ����� ��������� ���������
        TextOut(hdc, 30, 2, L"������� ���     ", 18); // ����� ��������� ���������

        EndPaint(hwnd, &ps); // ����� �����������
        break;
   
    case WM_DESTROY: // �������� ����
        PostQuitMessage(0);
        break;
    default: // ��������� ��������� �� ���������
        return DefWindowProc(hwnd, Message, wparam, lparam);
    }
    return 0;
}
DWORD WINAPI ReceivingMessages(LPVOID client_socket)
{
    SOCKET my_sock;
    my_sock = ((SOCKET*)client_socket)[0];
    while (1)
    {
        nsize = recv(my_sock, buff, sizeof(buff) - 1, 0);
        buff[nsize] = 0;
        SendDlgItemMessage(hwnd, MAIN_STATICBOX, EM_REPLACESEL, 0, (LPARAM)getwc(buff));
        SendDlgItemMessage(hwnd, MAIN_STATICBOX, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
      
    }
}