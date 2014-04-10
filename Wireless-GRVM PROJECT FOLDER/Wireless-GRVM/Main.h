#ifndef MAIN_H
#define MAIN_H

#include "Includes.h"
#include "Session.h"
#include "Transport.h"
#include <commctrl.h>

#define UNTITLED TEXT ("(untitled)")
#define EDITID   1
#define WINDOW_HEIGHT	800
#define WINDOW_WIDTH	1000
#define crc_ok  0x470F

// Function Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL Register(HINSTANCE);
HWND Create(HINSTANCE, int);
BOOL Window_OnCreate(HWND, LPCREATESTRUCT);
void Window_OnCommand (HWND, int, HWND, UINT);
void Window_OnDestroy (HWND);
BOOL CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM);
void OpenFileInitialize(HWND);
BOOL FileOpenDlg (HWND, PTSTR, LPCSTR);
BOOL FileRead(HWND, const LPCSTR);
void OkMessage(HWND, TCHAR*, TCHAR*);
BOOL ErrorCheck(char*);
void GenerateCRC(char*, char*);
void DisplayText(HWND, LPCSTR);
void Window_OnVScroll(HWND, HWND, UINT, int);
void Window_OnPaint(HWND);
void Window_OnSize(HWND, UINT, int, int);
BOOL FileSave(HWND, LPCTSTR);
BOOL FileSaveDlg (HWND, PTSTR, PTSTR);

// Variables
const LPTSTR lpszCommName	= TEXT("COM1");

#endif
