#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib,"Ws2_32.lib")
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <winsock2.h> // Wincosk2.h должен быть раньше windows!
#include <vector>
#include <windows.h>
#define MAIN_STATICBOX 104
#define BUFFER_SIZE 2048
#define WM_ACCEPT WM_USER + 1
using namespace std;
SOCKET s;
DWORD WINAPI ReceivingMessages(LPVOID client_socket);
char name[16] = "Аноним";
char buff[1024];
int nsize;
wchar_t* getwc(const char* c) {
    size_t iSize = strlen(c) + 1;
    wchar_t* wc = (wchar_t*)calloc(iSize, sizeof(wchar_t));
    mbstowcs(wc, c, iSize);
    return wc;
}

HWND hwnd; // дескриптор окна


LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM); // функция обработки сообщений окна
static HWND hStat; // дескриптор статического текста
// Стартовая функция
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
   // cout << "Укажите ваше имя: ";
   
    
    //cout << "Укажите метод подключения(local/host): ";
    
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



    MSG msg;   // структура сообщения
    WNDCLASS w; // структура класса окна
    memset(&w, 0, sizeof(WNDCLASS)); // очистка памяти для структуры
    w.style = CS_HREDRAW | CS_VREDRAW;
    w.lpfnWndProc = WndProc;
    w.hInstance = hInstance;
    w.hbrBackground = CreateSolidBrush(0x00FFFFFF);
    w.lpszClassName = L"MyClass";
    RegisterClass(&w); // регистрация класса окна
    // Создание окна
    hwnd = CreateWindow(L"MyClass", L"Клиент",
        WS_OVERLAPPEDWINDOW,
        500, 500, 500, 380,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow); // отображение окна
    UpdateWindow(hwnd);         // перерисовка окна
    // Цикл обработки сообщений
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
// Функция обработки сообщений
LONG WINAPI WndProc(HWND hwnd, UINT Message,
    WPARAM wparam, LPARAM lparam) {
    HDC hdc;
    HINSTANCE hInst;
    PAINTSTRUCT ps;
    static HWND hBtn; // дескриптор кнопки
    static HWND hEdt1, hEdt2; // дескрипторы полей редактирования
   
    CHAR StrA[20];
    int a, b, sum, Len;
 
    switch (Message) {

    case WM_CREATE: // сообщение создания окна
        hInst = ((LPCREATESTRUCT)lparam)->hInstance; // дескриптор приложения
        // Создаем и показываем первое поле редактирования
        hEdt1 = CreateWindow(L"edit", L"80",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 50, 300, 150, 30,
            hwnd, 0, hInst, NULL);
        ShowWindow(hEdt1, SW_SHOWNORMAL);
        // Создаем и показываем второе поле редактирования
        hEdt2 = CreateWindow(L"edit", L"Аноним",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER ,150, 0, 150,
            20, hwnd, 0, hInst, NULL);
        ShowWindow(hEdt2, SW_SHOWNORMAL);
        // Создаем и показываем кнопку
        hBtn = CreateWindow(L"button", L"Отправить",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            250, 300, 150, 30, hwnd, 0, hInst, NULL);
        ShowWindow(hBtn, SW_SHOWNORMAL);
        // Создаем и показываем поле текста для результата
        hStat = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY ,
            10, 30, 400,250, hwnd, (HMENU)MAIN_STATICBOX, hInst, NULL);
        ShowWindow(hStat, SW_SHOWNORMAL);
        break;
    case WM_COMMAND:  // сообщение о команде
        if (lparam == (LPARAM)hBtn)    // если нажали на кнопку
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
            // передаем строку клиента серверу
            SendDlgItemMessage(hwnd, MAIN_STATICBOX, EM_REPLACESEL, 0, (LPARAM)Mes_wc);
            SendDlgItemMessage(hwnd, MAIN_STATICBOX, EM_REPLACESEL, 0, (LPARAM)L"\r\n");
            send(s, strcat(strcat(Name_convert, ": "), buff), nsize, 0);
        
            
            //SetWindowText(hStat, Mes_wc); // выводим результат в статическое поле
        
        }
        break;
    case WM_PAINT: // перерисовка окна
        hdc = BeginPaint(hwnd, &ps); // начало перерисовки
      //  TextOut(hdc, 50, 300, L"Введите порт       ", 18); // вывод текстовых сообщений
        TextOut(hdc, 30, 2, L"Введите имя     ", 18); // вывод текстовых сообщений

        EndPaint(hwnd, &ps); // конец перерисовки
        break;
   
    case WM_DESTROY: // закрытие окна
        PostQuitMessage(0);
        break;
    default: // обработка сообщения по умолчанию
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