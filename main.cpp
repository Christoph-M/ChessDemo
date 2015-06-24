#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cstdio>

#include "wApp.h"
#include "wWnd.h"
#include "square.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("Chess Demo");
bool gameRunning = true;    // Game runs until this value is set to false
HWND gHwnd;                 // Global copy of hwnd from PeekMessaage()

POINT   p;                  // To be used to calculate cursor position
RECT    clientRect;         // To be used to calculate cient area
HBITMAP bmpWhiteSquare, bmpBlackSquare, bmpHighlight, bmpSelection; // Bitmaps to be loaded

const int boardWidth    = 8;        // Width of the chessboard
const int boardHeight   = 8;        // Height of the chessboard
const int boardEdge     = 20;       // Edge size around the chessboard
int       winWidth, winHeight;      // Size of client area
bool      squareSelect  = false;    // Tracking if a square is selected

square square[boardHeight][boardWidth], highlight, selection;   // Creating objects for the bitmaps drawn to screen

int calcPos(int i){ return (i * 64) + boardEdge; }              // Calcuation of position based on the grid position

/*void drawBitmap(HDC memHdc, HDC bmpHdc, HBITMAP hBitmap, int posX, int posY){
    BITMAP bm;
    SelectObject(bmpHdc, hBitmap);
    GetObject(hBitmap, sizeof(bm), &bm);
    BitBlt(memHdc, posX, posY, bm.bmWidth, bm.bmHeight, bmpHdc, 0, 0, SRCCOPY);
}*/

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow){

    MSG messages;            /* Here messages to the application are saved */
    wApp winApp(hThisInstance, szClassName, WindowProcedure);

    /* Register the window class, and if it fails quit the program */
    if (!winApp.regWnd()){
        puts("Failed to register window\n");
        return 0;
    }

    wWnd window;

    window.create(hThisInstance, szClassName, _T("Chess Demo"), (boardWidth * 64) + (boardEdge + 30), (boardHeight * 64) + (boardEdge + 50));

    /* Make the window visible on the screen */
    if (window.show(nCmdShow)){
        puts("Failed to show window\n");
        return 0;
    }

    // Set coordinates for chessboard squares
    for (int y = 0; y < boardHeight; ++y){
        for (int x = 0; x < boardWidth; ++x){
            square[y][x].setPosX(calcPos(x));
            square[y][x].setPosY(calcPos(y));
        }
    }

    while (gameRunning){
        /* Run the message loop. It will run until PeekMessage() returns 0 */
        while (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)){
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&messages);
            /* Send message to WindowProcedure */
            DispatchMessage(&messages);
        }

        /// Game logic here

        // Calculate client area
        GetClientRect(gHwnd, &clientRect);

        winWidth  = clientRect.right  - clientRect.left;
        winHeight = clientRect.bottom + clientRect.left;

        //__________________________________________Start Drawing__________________________________________

        HDC hdc = GetDC(gHwnd);
            bmpWhiteSquare = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/square_white.bmp",   IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpBlackSquare = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/square_black.bmp",   IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpHighlight   = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/highlight.bmp",      IMAGE_BITMAP, 72, 72, LR_LOADFROMFILE);
            bmpSelection   = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/selection.bmp",      IMAGE_BITMAP, 72, 72, LR_LOADFROMFILE);
            HDC     memHdc = CreateCompatibleDC(hdc);   // Image-Buffer
            HDC     bmpHdc = CreateCompatibleDC(hdc);   // Bitmap-Buffer
            HBITMAP memHbm = CreateCompatibleBitmap(hdc, winWidth, winHeight);  // Canvas

            HBITMAP hBitmap = (HBITMAP)SelectObject(bmpHdc, bmpWhiteSquare);
            HANDLE  hOld    = (HANDLE)SelectObject(memHdc, memHbm);

            //__________________________________________Draw Chess Board__________________________________________

            bool isWhite = true;

            for (int y = 0; y < boardHeight; ++y){
                for (int x = 0; x < boardWidth; ++x){
                    if (isWhite){
                        square[y][x].drawBitmap(memHdc, bmpHdc, bmpWhiteSquare);

                        if (x < boardWidth - 1 || boardWidth % 2) isWhite = !isWhite;
                    } else {
                        square[y][x].drawBitmap(memHdc, bmpHdc, bmpBlackSquare);

                        if (x < boardWidth - 1 || boardWidth % 2) isWhite = !isWhite;
                    }
                }
            }

            //__________________________________________Draw Highlight__________________________________________

            if ((p.x >= boardEdge && p.x <= calcPos(boardWidth)) && (p.y >= boardEdge && p.y <= calcPos(boardHeight))){
                for (int y = 0; y < boardHeight; ++y){
                   for (int x = 0; x < boardWidth; ++x){
                        if ((p.x > square[y][x].getPosX() && p.x < (square[y][x].getPosX() + 64)) && (p.y > square[y][x].getPosY() && p.y < (square[y][x].getPosY() + 64))){
                            highlight.setPosX(square[y][x].getPosX() - 4);
                            highlight.setPosY(square[y][x].getPosY() - 4);
                            highlight.drawBitmap(memHdc, bmpHdc, bmpHighlight);
                        }
                    }
                }
            }

            //__________________________________________Draw Selection__________________________________________

            if (squareSelect){
                for (int y = 0; y < boardHeight; ++y){
                    for (int x = 0; x < boardWidth; ++x){
                        if ((p.x > square[y][x].getPosX() - 1 && p.x < (square[y][x].getPosX() + 64) + 1) && (p.y > square[y][x].getPosY() - 1 && p.y < (square[y][x].getPosY() + 64) + 1)){
                            if (!selection.getPosX() && !selection.getPosY()){
                                selection.setPosX(square[y][x].getPosX() - 4);
                                selection.setPosY(square[y][x].getPosY() - 4);
                            }

                            selection.drawBitmap(memHdc, bmpHdc, bmpSelection);
                        } else if(squareSelect){
                            if (selection.getPosX() && selection.getPosY())
                                selection.drawBitmap(memHdc, bmpHdc, bmpSelection);
                        }
                    }
                }
            }

            //______________________________Copy the image in memHdc to screen___________________________________

            BitBlt(hdc, 0, 0, winWidth, winHeight, memHdc, 0, 0, SRCCOPY);

            //__________________________________________Clean-up_________________________________________________

            SelectObject(memHdc, hOld);
            SelectObject(bmpHdc, hBitmap);
            DeleteDC(memHdc);
            DeleteDC(bmpHdc);
            DeleteObject(memHbm);
            DeleteObject(bmpWhiteSquare);
            DeleteObject(bmpBlackSquare);
            DeleteObject(bmpHighlight);
            DeleteObject(bmpSelection);
        ReleaseDC(gHwnd, hdc);

        /// Regulate framerate, if possible
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message){                 /* handle the messages */
        case WM_CREATE:
            gHwnd = hwnd;
            break;
        case WM_MOUSEMOVE:
            GetCursorPos(&p);
            ScreenToClient(hwnd, &p);

            printf("x: %ld y: %ld\n", p.x, p.y);
            break;
        case WM_LBUTTONUP:
            if ((p.x >= boardEdge && p.x <= calcPos(boardWidth)) && (p.y >= boardEdge && p.y <= calcPos(boardHeight))){
                squareSelect = true;
                selection.setPosX(0);
                selection.setPosY(0);
            } else {
                squareSelect = false;
            }
            break;
        case WM_RBUTTONUP:
            squareSelect = false;
            break;
        case WM_DESTROY:
            gameRunning = false;
            PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
