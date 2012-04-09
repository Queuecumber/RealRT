/*
	SphereTest.cpp

	compiles to SphereTest.exe, links to RTEngine.lib

	basic test of the ray tracing using the sphere class
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gmath.h>
#include <condebug.h>

using gmath::vector;
using gmath::matrix;

//include the engine code
#include "..\RTEngine\RTEngine.h"

//1280x1024 window
const int MAJORLENGTH = 800;
const double ASPECTRATIO = 4.0/3.0;

const int SCREENWIDTH = MAJORLENGTH;
const int SCREENHEIGHT = (int)(MAJORLENGTH/ASPECTRATIO);  

//matrix transform, scales screensize to 40.0x15.0 window with double coordinates
matrix<2,2,double> SCRNTOLOGTRANSFORM( (20.0/double(SCREENWIDTH)) ,0.0,
										0.0,                       (15.0/double(SCREENHEIGHT)) );
//rotation matrix test
//matrix<3,3,double> piover4 = gmath::rotationMatrix(45,45,45);

//prototype of the callback function
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//the engine for this application
RTEngine *engine;

//fill in gmath transformation functions so the engine can use them
//

vector<2,double> _cdecl SCRNTOLOG(gmath::vector<2,double> pt) 
{
	vector<2,double> scpt((pt - vector<2,double>(SCREENWIDTH/2.0,SCREENHEIGHT/2.0)) * SCRNTOLOGTRANSFORM);

	scpt[1] = -scpt[1];

	return scpt;
}

vector<2,double> _cdecl LOGTOSCRN(gmath::vector<2,double> pt) 
{
	//invert diagonal matrix
	//
	matrix<2,2,double> invert = SCRNTOLOGTRANSFORM;
	for(int i=0,j=0; i < 2; i++,j++)
		invert[i][j] = 1 / invert[i][j];

	pt[1] = -pt[1];
	vector<2,double> scpt((pt * invert) - vector<2,double>(SCREENWIDTH/2.0,SCREENHEIGHT/2.0));

	return scpt;
} 

//console subsystem redirect for debugging
//
int main(int argc, char **argv)
{
	WinMain(GetModuleHandle(NULL),NULL,argv[1],SW_SHOW);
}

//WinMain subsystem
//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
	wc.lpszClassName = "RTSPHERETESTCLS";
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	//register the window class
	//
	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	//intitialize engine
	engine = RTEngine::Instantiate(SCREENWIDTH,SCREENHEIGHT);

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = SCREENWIDTH, rect.bottom = SCREENHEIGHT;
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, 0 );
	rect.right -= rect.left, rect.bottom -= rect.top;

	//create the application window
	//
	hwnd = CreateWindowEx(
		0,
		"RTSPHERETESTCLS",
		"SphereTest -- RTEngine",
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom,
		NULL, NULL, GetModuleHandle(NULL), NULL);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}									

	//create a sphere and a light source
	//
	sphere s1(DIFFREDOBJECT,vector<3,double>(-5.0,0.0,2.0));
	sphere s2(DIFFGREENOBJECT,vector<3,double>(0.0,4.0,0.0),0.5);
	sphere s3(DIFFGREENOBJECT,vector<3,double>(0.0,2.0,0.0),0.5);
	sphere s4(DIFFGREENOBJECT,vector<3,double>(0.0,0.0,0.0),0.5);
	sphere s5(REFLBLUEOBJECT,vector<3,double>(5.0,0.0,0.0),1.5);

	plane3d floorobj(MIRROR,vector<3,double>(0.0,1.0,0.0),3.0);

	SphericalLight l1(WHITELIGHT,vector<3,double>(-9.5,6.0,0.0),0.1);
	SphericalLight l2(WHITELIGHT,vector<3,double>(0.0,6.0,-2.0),0.1);
	SphericalLight l3(WHITELIGHT,vector<3,double>(9.5,6.0,0.0),0.1);

	//add the spheres to the world
	engine->AddWorldObject(&floorobj);

	engine->AddWorldObject(&s1);
	engine->AddWorldObject(&s2);
	engine->AddWorldObject(&s3);
	engine->AddWorldObject(&s4);
	engine->AddWorldObject(&s5);

	engine->AddWorldObject(&l1);
	engine->AddWorldObject(&l2);
	engine->AddWorldObject(&l3);

	//calculate the scene (this only needs to be done once for a still image, so get it
	//out of the way before the window is displayed)
	engine->CalculateSceneAsync();
	

	//display the window
	//
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


//Store window contents in memory for fast redrawing, these are filled in at the first paint request
//
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

			//blit from the backbuffer
			BitBlt(hDC,0,0,SCREENWIDTH,SCREENHEIGHT,hMemDC,0,0,SRCCOPY);

			EndPaint(hwnd,&ps);
		}
		break;

		case WM_KEYUP:
			switch(wParam)
			{
			case VK_ESCAPE:
				DestroyWindow(hwnd);		//quit on escape
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
