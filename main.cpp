#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <cstdio>

#include "header.h"
#include "wApp.h"
#include "wWnd.h"
#include "square.h"
#include "highlight.h"
#include "pawn.h"
#include "knight.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("Chess Demo");
bool gameRunning = true;    // Game runs until this value is set to false
HWND gHwnd;                 // Global copy of hwnd from PeekMessaage()

POINT   p;                  // To be used to determine cursor position
RECT    clientRect;         // To be used to determine cient area
HBITMAP bmpWhiteSquare, bmpBlackSquare, bmpHighlight, bmpHighlightM, bmpSelection, bmpPawnW, bmpPawnB, bmpPawnM, // Bitmaps to be loaded
        bmpKnightW,     bmpKnightB,     bmpKnightM;

int       winWidth, winHeight;      // Size of client area

square    square[BOARD_HEIGHT][BOARD_WIDTH], board;   // Object declaration for the chessboard squares
highlight highlight, selection;                // Object declarations for the highlight and selection squares

// Chessman objects
pawn   pawnW[8]   = {pawn(WHITE, 111),   pawn(WHITE, 112),   pawn(WHITE, 113),   pawn(WHITE, 114),     // Object declarations for the white pawns
                     pawn(WHITE, 115),   pawn(WHITE, 116),   pawn(WHITE, 117),   pawn(WHITE, 118)};
pawn   pawnB[8]   = {pawn(BLACK, 211),   pawn(BLACK, 212),   pawn(BLACK, 213),   pawn(BLACK, 214),     // Object declarations for the black pawns
                     pawn(BLACK, 215),   pawn(BLACK, 216),   pawn(BLACK, 217),   pawn(BLACK, 218)};
knight knightW[8] = {knight(WHITE, 121), knight(WHITE, 122), knight(WHITE, 123), knight(WHITE, 124),   // Object declarations for the white knights
                     knight(WHITE, 125), knight(WHITE, 126), knight(WHITE, 127), knight(WHITE, 128)};
knight knightB[8] = {knight(BLACK, 221), knight(BLACK, 222), knight(BLACK, 223), knight(BLACK, 224),   // Object declarations for the black knights
                     knight(BLACK, 225), knight(BLACK, 226), knight(BLACK, 227), knight(BLACK, 228)};

int checkMatch(int id){
    for (int i = 0; i < 8; ++i){
        if (pawnW[i].getID() == id) { square[selection.getPosY()][selection.getPosX()].setContID(0); square[selection.getPosY()][selection.getPosX()].setHasID(false); return i; }
        if (pawnB[i].getID() == id) return i;
        if (knightW[i].getID() == id) return i;
        if (knightB[i].getID() == id) return i;
    }

    return -1;
}




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

    window.create(hThisInstance, szClassName, _T("Chess Demo"), (BOARD_WIDTH * 64) + (BOARD_EDGE + 30), (BOARD_HEIGHT * 64) + (BOARD_EDGE + 50));

    /* Make the window visible on the screen */
    if (window.show(nCmdShow)){
        puts("Failed to show window\n");
        return 0;
    }

    // Set position and color for chessboard squares
    bool isWhite = true;

    for (int y = 0; y < BOARD_HEIGHT; ++y){
        for (int x = 0; x < BOARD_WIDTH; ++x){
            square[y][x].setPosX(x);
            square[y][x].setPosY(y);
            square[y][x].setColor(isWhite);

            if (x < BOARD_WIDTH - 1 || BOARD_WIDTH % 2) isWhite = !isWhite;
        }
    }

    // Set position for chessmen
    for (int i = 0; i < 8; ++i){
        pawnW[i].setPosX(((BOARD_WIDTH - 8) / 2) + i);
        pawnW[i].setPosY(BOARD_HEIGHT - 2);
        square[pawnW[i].getPosY()][pawnW[i].getPosX()].setContID(pawnW[i].getID());
        pawnB[i].setPosX(((BOARD_WIDTH - 8) / 2) + i);
        pawnB[i].setPosY(1);
        square[pawnB[i].getPosY()][pawnB[i].getPosX()].setContID(pawnB[i].getID());

        knightW[i].setPosX(((BOARD_WIDTH - 8) / 2) + i);
        knightW[i].setPosY(BOARD_HEIGHT - 1);
        square[knightW[i].getPosY()][knightW[i].getPosX()].setContID(knightW[i].getID());
        knightB[i].setPosX(((BOARD_WIDTH - 8) / 2) + i);
        knightB[i].setPosY(0);
        square[knightB[i].getPosY()][knightB[i].getPosX()].setContID(knightB[i].getID());
    }

    // Main program loop. Runs until gameRunning is set to false
    while (gameRunning){
        // Message loop
        while (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)){
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&messages);
            /* Send message to WindowProcedure */
            DispatchMessage(&messages);
        }

        //____________________________Calculate Position of Highlight and Selection_____________________________

        if (square[0][0].cursorWithinBoard(p)){ // Check if cursor is within the chessboard
            for (int y = 0; y < BOARD_HEIGHT; ++y){  // If cursor is within the chessboard, iterate through chessboard squares
               for (int x = 0; x < BOARD_WIDTH; ++x){
                    if (square[y][x].cursorWithinSquare(p)){    // Check if the cursor is within the current chessboard square
                        if (!selection.validPos()){             // If the cursor is within the chessboard square, check if selection has a valid position
                            selection.setPosX(square[y][x].getPosX());            // If selection has an invalid position, set position of current chessboard square to selection
                            selection.setPosY(square[y][x].getPosY());
                        }

                        // If the cursor is within the chessboard square, set position of current chessboard square to highlight and set visibility of highlight to true
                        highlight.setPosX(square[y][x].getPosX());
                        highlight.setPosY(square[y][x].getPosY());
                        highlight.setVisible(true);
                    }
                }
            }
        } else {    // If cursor is not within the chessboard, set visibility of highlight to false
            highlight.setVisible(false);
        }

        // Calculate client area
        GetClientRect(gHwnd, &clientRect);

        winWidth  = clientRect.right  - clientRect.left;
        winHeight = clientRect.bottom + clientRect.left;

        //______________________________________________Start Drawing___________________________________________

        HDC hdc = GetDC(gHwnd);
            bmpWhiteSquare = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/square_white.bmp",   IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpBlackSquare = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/square_black.bmp",   IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpHighlight   = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/highlight.bmp",      IMAGE_BITMAP, 72, 72, LR_LOADFROMFILE);
            bmpHighlightM  = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/highlight_mask.bmp", IMAGE_BITMAP, 72, 72, LR_LOADFROMFILE);
            bmpSelection   = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/selection.bmp",      IMAGE_BITMAP, 72, 72, LR_LOADFROMFILE);
            bmpPawnW       = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/bauer_weiss.bmp",    IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpPawnB       = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/figur_schwarz.bmp",  IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpPawnM       = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/bauer_mask.bmp",     IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpKnightW     = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/turm_weiss.bmp",     IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpKnightB     = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/figur_schwarz.bmp",  IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpKnightM     = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/turm_mask.bmp",      IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            HDC     memHdc = CreateCompatibleDC(hdc);   // Image-Buffer
            HDC     bmpHdc = CreateCompatibleDC(hdc);   // Bitmap-Buffer
            HBITMAP memHbm = CreateCompatibleBitmap(hdc, winWidth, winHeight);  // Canvas

            HBITMAP hBitmap = (HBITMAP)SelectObject(bmpHdc, bmpWhiteSquare);
            HANDLE  hOld    = (HANDLE)SelectObject(memHdc, memHbm);

            //__________________________________________Draw Chess Board________________________________________

            for (int y = 0; y < BOARD_HEIGHT; ++y){  // Iterate through chessboard squares
                for (int x = 0; x < BOARD_WIDTH; ++x){
                    if (square[y][x].getColor()){   // Check square color (true = white | false = black)
                        square[y][x].drawBitmap(memHdc, bmpHdc, bmpWhiteSquare);    // If square color is white, draw white square to memory buffer
                    } else {
                        square[y][x].drawBitmap(memHdc, bmpHdc, bmpBlackSquare);    // If square color is black, draw black square to memory buffer
                    }
                }
            }

            //__________________________________________Draw Chessmen___________________________________________

            for (int i = 0; i < 8; ++i){
                pawnW[i].drawBitmap(memHdc, bmpHdc, bmpPawnW, bmpPawnM);
                pawnB[i].drawBitmap(memHdc, bmpHdc, bmpPawnB, bmpPawnM);

                knightW[i].drawBitmap(memHdc, bmpHdc, bmpKnightW, bmpKnightM);
                knightB[i].drawBitmap(memHdc, bmpHdc, bmpKnightB, bmpKnightM);
            }

            //__________________________________________Draw Highlight__________________________________________

            if (highlight.getVisible()){    // Check if highlight is visible
                highlight.drawBitmap(memHdc, bmpHdc, bmpHighlight, bmpHighlightM);      // If highlight is visible, draw to memory buffer
            }

            //__________________________________________Draw Selection__________________________________________

            if(selection.getVisible() && selection.getPosX() >= 0 && selection.getPosY() >= 0){   // Check if selection is visible and has valid coordinates
                selection.drawBitmap(memHdc, bmpHdc, bmpSelection, bmpHighlightM);      // If selection is visible and has a valid position, draw to memory buffer
            }

            //______________________________Copy the image in memHdc to screen__________________________________

            BitBlt(hdc, 0, 0, winWidth, winHeight, memHdc, 0, 0, SRCCOPY);

            //__________________________________________Clean-up________________________________________________

            SelectObject(memHdc, hOld);
            SelectObject(bmpHdc, hBitmap);
            DeleteDC(memHdc);
            DeleteDC(bmpHdc);
            DeleteObject(memHbm);
            DeleteObject(bmpWhiteSquare);
            DeleteObject(bmpBlackSquare);
            DeleteObject(bmpHighlight);
            DeleteObject(bmpHighlightM);
            DeleteObject(bmpSelection);
            DeleteObject(bmpPawnW);
            DeleteObject(bmpPawnB);
            DeleteObject(bmpPawnM);
            DeleteObject(bmpKnightW);
            DeleteObject(bmpKnightB);
            DeleteObject(bmpKnightM);
        ReleaseDC(gHwnd, hdc);

        Sleep(20);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}






/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message){                 /* handle the messages */
        case WM_CREATE:
            gHwnd = hwnd;             // Assign window handle to gHwnd when window is created
            break;
        case WM_MOUSEMOVE:
            GetCursorPos(&p);         // Get current cursor position in client area whenever the cursor moves
            ScreenToClient(hwnd, &p);

            printf("x: %ld y: %ld\n", p.x, p.y);    // Print coordinates of the cursor to the console
            break;
        case WM_LBUTTONUP:
            // On left mouse button up, check if cursor is within the chessboard
            if (board.cursorWithinBoard(p)){
                if (selection.getVisible()){
                    switch (square[selection.getPosY()][selection.getPosX()].getContID()){
                        case 111:
                            pawnW[checkMatch(111)].setPosX(highlight.getPosX());
                            pawnW[checkMatch(111)].setPosY(highlight.getPosY());
                            square[highlight.getPosY()][highlight.getPosX()].setContID(pawnW[checkMatch(111)].getID());
                            break;
                        case 112:
                            pawnW[checkMatch(112)].setPosX(highlight.getPosX());
                            pawnW[checkMatch(112)].setPosY(highlight.getPosY());
                            square[highlight.getPosY()][highlight.getPosX()].setContID(pawnW[checkMatch(112)].getID());
                            break;
                        case 113:
                            pawnW[checkMatch(113)].setPosX(highlight.getPosX());
                            pawnW[checkMatch(113)].setPosY(highlight.getPosY());
                            square[highlight.getPosY()][highlight.getPosX()].setContID(pawnW[checkMatch(113)].getID());
                            break;
                        case 114:
                            pawnW[checkMatch(114)].setPosX(highlight.getPosX());
                            pawnW[checkMatch(114)].setPosY(highlight.getPosY());
                            square[highlight.getPosY()][highlight.getPosX()].setContID(pawnW[checkMatch(114)].getID());
                            break;
                        case 115:
                            pawnW[checkMatch(115)].setPosX(highlight.getPosX());
                            pawnW[checkMatch(115)].setPosY(highlight.getPosY());
                            square[highlight.getPosY()][highlight.getPosX()].setContID(pawnW[checkMatch(115)].getID());
                            break;
                        case 116:
                            pawnW[checkMatch(116)].setPosX(highlight.getPosX());
                            pawnW[checkMatch(116)].setPosY(highlight.getPosY());
                            square[highlight.getPosY()][highlight.getPosX()].setContID(pawnW[checkMatch(116)].getID());
                            break;
                        case 117:
                            pawnW[checkMatch(117)].setPosX(highlight.getPosX());
                            pawnW[checkMatch(117)].setPosY(highlight.getPosY());
                            square[highlight.getPosY()][highlight.getPosX()].setContID(pawnW[checkMatch(117)].getID());
                            break;
                        case 118:
                            pawnW[checkMatch(118)].setPosX(highlight.getPosX());
                            pawnW[checkMatch(118)].setPosY(highlight.getPosY());
                            square[highlight.getPosY()][highlight.getPosX()].setContID(pawnW[checkMatch(118)].getID());
                            break;
                        default:
                            break;
                    }

                    selection.setVisible(false);
                } else {
                    // If cursor is within the chessboard, set selection visibility to true and invalidate position, so a new position can be assigned
                    selection.setVisible(true);
                    selection.invalidatePos();
                }
            } else {    // If cursor is not within the chessboard, set selection visibility to false
                selection.setVisible(false);
            }
            break;
        case WM_RBUTTONUP:
            selection.setVisible(false);    // On right mouse button up, set selection visibility to false
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
