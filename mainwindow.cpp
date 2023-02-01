#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#include <process.h>
#include <cstring>

#include <QPainter>

#include <QVBoxLayout>

#include <QApplication>
#include <QPushButton>

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
int ikran_x = 0;
int ikran_y = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ikran_x = GetSystemMetrics(SM_CXSCREEN);
    ikran_y = GetSystemMetrics(SM_CYSCREEN);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setGeometry(0,0,ikran_x,ikran_y);

    ui->gridLayout->addWidget(myPicture);
    ui->gridWidget->setGeometry(-8,-8,ikran_x+16,ikran_y+16);
    myPicture->show();

    connect(this,&MainWindow::set_pos,myPicture,&MyGraphicView::get_pos);
}
//gafask
MainWindow::~MainWindow()
{
    delete ui;
}

HANDLE hProcess = 0;
float mem_getValue_float(uintptr_t statik_adress)
{
    float ammoValue = 0;
    DWORD oldProtect = 0;
    VirtualProtectEx(hProcess, (void*)statik_adress, sizeof(ammoValue), PAGE_EXECUTE_READWRITE, &oldProtect);
    ReadProcessMemory(hProcess, (void*)statik_adress, &ammoValue, sizeof(ammoValue), nullptr);
    VirtualProtectEx(hProcess, (void*)statik_adress, sizeof(ammoValue), oldProtect, NULL);
    return ammoValue;
}
bool mem_getValue_bool(uintptr_t statik_adress)
{
    bool ammoValue = 0;
    DWORD oldProtect = 0;
    VirtualProtectEx(hProcess, (void*)statik_adress, sizeof(ammoValue), PAGE_EXECUTE_READWRITE, &oldProtect);
    ReadProcessMemory(hProcess, (void*)statik_adress, &ammoValue, sizeof(ammoValue), nullptr);
    VirtualProtectEx(hProcess, (void*)statik_adress, sizeof(ammoValue), oldProtect, NULL);
    return ammoValue;
}
void mem_setValue(uintptr_t stitik_adress, float value)
{
    DWORD oldProtect = 0;
    VirtualProtectEx(hProcess, (void*)stitik_adress, sizeof(value), PAGE_EXECUTE_READWRITE, &oldProtect);
    WriteProcessMemory(hProcess, (void*)stitik_adress, &value, sizeof(value), 0);
    VirtualProtectEx(hProcess, (void*)stitik_adress, sizeof(value), oldProtect, NULL);
}
uintptr_t Smeshenie_PLUS(HANDLE hProc, uintptr_t start_adress, std::vector<unsigned int> smeshenie)
{
    uintptr_t addr = start_adress;
    for (unsigned int i = 0; i < smeshenie.size(); ++i)
    {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += smeshenie[i];
    }
    return addr;
}
DWORD GetProcId1(const wchar_t* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        if (Process32First(hSnap, &procEntry))
        {
            do
            {
                if (!_wcsicmp(procEntry.szExeFile, procName))
                {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));

        }
    }
    CloseHandle(hSnap);
    return procId;
}
uintptr_t GetModuleBaseAddress1(DWORD procId, const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}
uintptr_t go_offcet(HANDLE handl, uintptr_t start_addres, uintptr_t offset[])
{
    int size_masiv = 0;
    uintptr_t modul = start_addres;
    while (true)
    {
        size_masiv++;
        if (offset[size_masiv] > 0 && 10000 > offset[size_masiv])
        {
            size_masiv--;
            modul += offset[size_masiv];
            if(ReadProcessMemory(handl, (void*)modul, &modul, sizeof(modul), nullptr) == 0){return 0 ; break;  }
            if(modul == 0){return 0; break; }
        }
        else
        {
            size_masiv--;
            modul += offset[size_masiv];
            break;
        }
        size_masiv++;
    }
    return modul;
}
uintptr_t getModul(DWORD procID, HANDLE handl_proc, const wchar_t addres_modul[], uintptr_t offset_modul)
{
    uintptr_t g_modul = GetModuleBaseAddress1(procID, addres_modul);
    uintptr_t g_addres_modul = g_modul + offset_modul;
    DWORD oldProtect = 0;
    VirtualProtectEx(handl_proc, (void*)g_addres_modul, sizeof(g_addres_modul), PAGE_EXECUTE_READWRITE, &oldProtect);
    ReadProcessMemory(handl_proc, (void*)g_addres_modul, &g_addres_modul, sizeof(g_addres_modul), nullptr);
    VirtualProtectEx(handl_proc, (void*)g_addres_modul, sizeof(g_addres_modul), oldProtect, NULL);
    return  g_addres_modul;
}
void MainWindow::on_exit_push_clicked(){exit(1);}

uintptr_t offset_heal[5][5] = {{ 0x7C0 , 0x58 ,0x4 },
                               { 0x7C0 , 0x50 ,0x4 },
                               { 0x7C0 , 0x48 ,0x4 },
                               { 0x7C0 , 0x40 ,0x4 },
                               { 0x7C0 , 0x38 ,0x4 }
                              };
uintptr_t offset_pt_X[5][5] = {{ 0x7C0 , 0x58 ,0x24 },
                               { 0x7C0 , 0x50 ,0x24 },
                               { 0x7C0 , 0x48 ,0x24 },
                               { 0x7C0 , 0x40 ,0x24 },
                               { 0x7C0 , 0x38 ,0x24 }
                              };
uintptr_t offset_pt_Y[5][5] = {{ 0x7C0 , 0x58 ,0x28 },
                               { 0x7C0 , 0x50 ,0x28 },
                               { 0x7C0 , 0x48 ,0x28 },
                               { 0x7C0 , 0x40 ,0x28 },
                               { 0x7C0 , 0x38 ,0x28 }
                              };
uintptr_t offset_ugol_povorota[1][2] = { {0x408 , 0xC34} };




DWORD procId;
HWND hwnd;
HDC hDC;
uintptr_t m_addres_pt_X[5];
uintptr_t m_addres_pt_Y[5];
uintptr_t m_addres_heal[5];


int X_pt_ikran[5];
int Y_pt_ikran[5];

float X_pt[5];
float Y_pt[5];


void MainWindow::ESP()
{
    hDC = ::GetDC (0);
    RECT rect;
    procId = GetProcId1(L"gtutorial-x86_64.exe");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
    hwnd  = FindWindow(NULL, L"Step 1");
    if (hwnd == 0) { qDebug() << "hwnd Eror"; }else
        if (procId == 0) { qDebug() << "procId Eror"; }else
            if (hProcess == 0) { qDebug() << "hProcess Eror";}

    uintptr_t modul_pt = getModul(procId, hProcess, L"gtutorial-x86_64.exe", 0x003CCD20);





    bool vidimost_vistrila[5] = {false,false,false,false,false};
    uintptr_t m_addres_ugol_povorota = go_offcet(hProcess, modul_pt, offset_ugol_povorota[0]);



    while (true)
    {
        Sleep(1);
        GetWindowRect(hwnd, &rect);
        emit set_pos(0,0,99,mem_getValue_float(m_addres_ugol_povorota),(rect.left + (rect.right - rect.left) * (0 + 1) / 2), (rect.top  + (rect.bottom - rect.top) * (0.8000000119f + 1) / 2));
        for (int shetshik = 0; shetshik < 5; ++shetshik)
        {
            m_addres_pt_X[shetshik] = 0;
            m_addres_pt_Y[shetshik] = 0;
            m_addres_heal[shetshik] = 0;

            m_addres_heal[shetshik] = go_offcet(hProcess, modul_pt, offset_heal[shetshik]);
            if(m_addres_heal[shetshik] != 0)
            {
                m_addres_pt_X[shetshik] = go_offcet(hProcess, modul_pt, offset_pt_X[shetshik]);
                m_addres_pt_Y[shetshik] = go_offcet(hProcess, modul_pt, offset_pt_Y[shetshik]);
            }
        }
        for (int shetshik = 0; shetshik < 5; ++shetshik)
        {
            X_pt_ikran[shetshik] = 0;
            Y_pt_ikran[shetshik] = 0;
            X_pt[shetshik] = 0;
            Y_pt[shetshik] = 0;
            if(m_addres_pt_X[shetshik] != 0 && m_addres_pt_Y[shetshik] != 0 && m_addres_heal[shetshik] != 0)
            {
                vidimost_vistrila[shetshik] = true;
                if ( mem_getValue_bool(m_addres_heal[shetshik]) == true )
                {
                    X_pt[shetshik] = mem_getValue_float(m_addres_pt_X[shetshik]);
                    Y_pt[shetshik] = mem_getValue_float(m_addres_pt_Y[shetshik]);
                    if (X_pt[shetshik] > -1.0f && X_pt[shetshik] < 1.0 && Y_pt[shetshik] > -1.0f && Y_pt[shetshik] < 1.0)
                    {
                        X_pt_ikran[shetshik] = rect.left + (rect.right - rect.left) * (X_pt[shetshik] + 1) / 2;
                        Y_pt_ikran[shetshik] = rect.top  + (rect.bottom - rect.top) * (Y_pt[shetshik] + 1) / 2;
                        emit set_pos(X_pt_ikran[shetshik],Y_pt_ikran[shetshik],shetshik,mem_getValue_float(m_addres_ugol_povorota),(rect.left + (rect.right - rect.left) * (0 + 1) / 2),(rect.top  + (rect.bottom - rect.top) * (0.8000000119f + 1) / 2));
                    }
                }
            }
            else if ( vidimost_vistrila[shetshik] == true)
            {
                emit set_pos(0,0,shetshik,mem_getValue_float(m_addres_ugol_povorota),(rect.left + (rect.right - rect.left) * (0 + 1) / 2),(rect.top  + (rect.bottom - rect.top) * (0.8000000119f + 1) / 2));
                vidimost_vistrila[shetshik] = false;
                qDebug()<<"Пропал";
            }
        }
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    potok_ESP_run();
}
