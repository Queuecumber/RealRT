/*
	Refraction Test

	test of the ray tracing refraction
*/


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gmath.h>
#include <shellapi.h>
#include <cstdlib>

#include <iostream>

using gmath::vector;
using gmath::matrix;

//include the engine code
#include "..\RTEngine\RTEngine.h"

//1280x1024 window
const int MAJORLENGTH = 800;
const double ASPECTRATIO = 4.0/3.0;

const int SCREENWIDTH = MAJORLENGTH;
const int SCREENHEIGHT = (int)(MAJORLENGTH/ASPECTRATIO);

matrix<2,2,double> SCRNTOLOGTRANSFORM( (double)(20.0/SCREENWIDTH),   0.0,
											 0.0,                          (double)((20.0/ASPECTRATIO)/SCREENHEIGHT) );

template<> 
vector<2,double> _cdecl SCRNTOLOG(gmath::vector<2,double> pt) 
{
	vector<2,double> scpt((pt - vector<2,double>(SCREENWIDTH/2.0,SCREENHEIGHT/2.0)) * SCRNTOLOGTRANSFORM);

	scpt[1] = -scpt[1];	//invert y paremeter for GDI

	return scpt;
}


//prototype of the callback function
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

RTEngine *engine = RTEngine::Instantiate(SCREENWIDTH,SCREENHEIGHT);

//application entry point
//
int main(int argc, char **argv)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//fill in window class
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = GetModuleHandle(NULL);
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "RTREFRACTIONTESTCLS";
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	//register the window class
	//
	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = SCREENWIDTH, rect.bottom = SCREENHEIGHT;
	AdjustWindowRect( &rect, WS_POPUP|WS_SYSMENU|WS_CAPTION, 0 );
	rect.right -= rect.left, rect.bottom -= rect.top;

	//create the application window
	//
	hwnd = CreateWindowEx(
		0,
		"RTREFRACTIONTESTCLS",
		"Refraction Test",
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom,
		NULL, NULL, GetModuleHandle(NULL), NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	plane3d back(DIFFREDOBJECT,vector<3,double>(0.3,0.0,-1.0),4.0);
	plane3d bottom(DIFFGREENOBJECT,vector<3,double>(0.0,1.0,0.0),7.0);

	sphere grid[9];
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
			grid[i * 3 + j] = sphere(DIFFGREENOBJECT,vector<3,double>(2.2 - (1.2 * i),2.2 - (1.2 * j),1.0),0.5,false); 
	}

	sphere lensish(TRANSPARENTOBJECT,vector<3,double>(0.0,0.0,0.0));

	sphere l1(WHITELIGHT,vector<3,double>(0.0,7.5,0.0),0.1,true);
	sphere l2(WHITELIGHT,vector<3,double>(6.0,2.0,-2.0),0.1,true);
	sphere l3(WHITELIGHT,vector<3,double>(-6.0,2.0,-2.0),0.1,true);

	for(int i = 0; i < 9; i++)
		engine->AddWorldObject(&grid[i]);

//	engine->AddWorldObject(&lensish);
	engine->AddWorldObject(&l1);
	engine->AddWorldObject(&l2);
	engine->AddWorldObject(&l3);
	engine->AddWorldObject(&back);
	engine->AddWorldObject(&bottom);

	engine->AddWorldObject(&l1);

	engine->CalculateScene();


	//display the window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//message loop
	//
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

//WinMain subsystem redirect
//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int argc;
	wchar_t **unicode = CommandLineToArgvW(GetCommandLineW(),&argc);
	
	char **argv = new char *[argc];
    for(int i = 0; i < argc; i++)
	{
		argv[i] = new char[wcslen(unicode[i])];
		wcstombs(argv[i],unicode[i],sizeof(char) * wcslen(unicode[i]));
	}

	return main(argc,argv);
}


HDC hMemDC = NULL;
HBITMAP hMemBMP = NULL;

//callback procedure
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	

	switch(msg)
	{
		case WM_CREATE:
		{

		}
		break;
		
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;

		case WM_PAINT:
		{
			//begin painting
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(hwnd,&ps);

			if(hMemDC == NULL)
			{
				//create the backbuffer
				hMemBMP = CreateCompatibleBitmap(hDC,SCREENWIDTH,SCREENHEIGHT);
				hMemDC = CreateCompatibleDC(hDC);

				HGDIOBJ old = SelectObject(hMemDC,hMemBMP);

				//render the precalculated scene to the back buffer
				engine->Render(hMemDC);
			}

			////draw axis lines, note that the engine provides all the tools needed to
			////do this from any position, but calculations are unneccesary for a static scene
			////
			//for(int i = 0; i < SCREENWIDTH; i++)
			//	SetPixel(hMemDC,i,SCREENHEIGHT/2,0x0000FF00);

			//for(int j = 0; j < SCREENHEIGHT; j++)
			//	SetPixel(hMemDC,SCREENWIDTH/2,j,0x000000FF);

			//blit from the backbuffer
			BitBlt(hDC,0,0,SCREENWIDTH,SCREENHEIGHT,hMemDC,0,0,SRCCOPY);

			EndPaint(hwnd,&ps);
		}
		break;

		case WM_KEYUP:
			switch(wParam)
			{
			case VK_ESCAPE:
				DestroyWindow(hwnd);
				break;
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
