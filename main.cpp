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
#include "chessMan.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void moveChessMan(int);

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
highlight highlight, selection;                       // Object declarations for the highlight and selection squares

// Chessman objects
chessMan chessMen[32]   = {chessMan(WHITE, PAWN,   111), chessMan(WHITE, PAWN,   112), chessMan(WHITE, PAWN,   113), chessMan(WHITE, PAWN,   114),   // Object declarations for the white pawns
                           chessMan(WHITE, PAWN,   115), chessMan(WHITE, PAWN,   116), chessMan(WHITE, PAWN,   117), chessMan(WHITE, PAWN,   118),
                           chessMan(BLACK, PAWN,   211), chessMan(BLACK, PAWN,   212), chessMan(BLACK, PAWN,   213), chessMan(BLACK, PAWN,   214),   // Object declarations for the black pawns
                           chessMan(BLACK, PAWN,   215), chessMan(BLACK, PAWN,   216), chessMan(BLACK, PAWN,   217), chessMan(BLACK, PAWN,   218),
                           chessMan(WHITE, KNIGHT, 121), chessMan(WHITE, KNIGHT, 122), chessMan(WHITE, KNIGHT, 123), chessMan(WHITE, KNIGHT, 124),   // Object declarations for the white knights
                           chessMan(WHITE, KNIGHT, 125), chessMan(WHITE, KNIGHT, 126), chessMan(WHITE, KNIGHT, 127), chessMan(WHITE, KNIGHT, 128),
                           chessMan(BLACK, KNIGHT, 221), chessMan(BLACK, KNIGHT, 222), chessMan(BLACK, KNIGHT, 223), chessMan(BLACK, KNIGHT, 224),   // Object declarations for the black knights
                           chessMan(BLACK, KNIGHT, 225), chessMan(BLACK, KNIGHT, 226), chessMan(BLACK, KNIGHT, 227), chessMan(BLACK, KNIGHT, 228)};





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

    // Create window
    window.create(hThisInstance, szClassName, _T("Chess Demo"), (BOARD_WIDTH * 64) + (BOARD_EDGE + 30), (BOARD_HEIGHT * 64) + (BOARD_EDGE + 50));

    /* Make the window visible on the screen */
    if (window.show(nCmdShow)){
        puts("Failed to show window\n");
        return 0;
    }

    // Set position and color for chessboard squares
    bool color = WHITE;

    for (int y = 0; y < BOARD_HEIGHT; ++y){     // Iterate through chessboard squares
        for (int x = 0; x < BOARD_WIDTH; ++x){
            square[y][x].setPos(x, y);          // Set position of current square
            square[y][x].setColor(color);       // Set color of current square

            if (x < BOARD_WIDTH - 1 || BOARD_WIDTH % 2) color = !color; // If loop is at the end of the board or if BOARD_WIDTH is uneven, negate color
        }
    }

    // Set position for chessMen
    for (int i = 0; i < 32; ++i){   // Iterate through chessMen
        if (chessMen[i].getColor() == WHITE){   // Check if current chessMan is WHITE
            switch (chessMen[i].getType()){     // If current chessMan is WHITE, check for type
                case PAWN:
                    chessMen[i].setPos(((BOARD_WIDTH - 8) / 2) + i, BOARD_HEIGHT - 2);      // If current chessMan is PAWN, set position for current chessMan
                    break;
                case KNIGHT:
                    chessMen[i].setPos(((BOARD_WIDTH - 8) / 2) + i - 16, BOARD_HEIGHT - 1); // If current chessMan is KNIGHT, set position for current chessMan
                    break;
            }
        } else {
            switch (chessMen[i].getType()){     // If current chessMan is not WHITE, check for type
                case PAWN:
                    chessMen[i].setPos(((BOARD_WIDTH - 8) / 2) + (i - 8), 1);   // If current chessMan is PAWN, set position for current chessMan
                    break;
                case KNIGHT:
                    chessMen[i].setPos(((BOARD_WIDTH - 8) / 2) + (i - 24), 0);  // If current chessMan is KNIGHT, set position for current chessMan
                    break;
            }
        }

        chessMen[i].setVisible(true);       // Set visibility of current chessMan to true
        square[chessMen[i].getPosY()][chessMen[i].getPosX()].setContID(chessMen[i].getID());    // Set containedID of square under current chessMan to ID of current chessMan
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

        //____________________________Determine Position of Highlight and Selection_____________________________

        if (board.cursorWithinBoard(p)){    // Check if cursor is within the chessboard
            for (int y = 0; y < BOARD_HEIGHT; ++y){  // If cursor is within the chessboard, iterate through chessboard squares
               for (int x = 0; x < BOARD_WIDTH; ++x){
                    if (square[y][x].cursorWithinSquare(p)){    // Check if the cursor is within the current square
                        if (!selection.validPos()){             // If the cursor is within the current square, check if selection has a valid position
                            selection.setPos(square[y][x].getPosX(), square[y][x].getPosY()); // If selection has an invalid position, set position of current square to selection
                        }

                        // If the cursor is within the current square, set position of current square to highlight and set visibility of highlight to true
                        highlight.setPos(square[y][x].getPosX(), square[y][x].getPosY());
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
            bmpKnightW     = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/springer_weiss.bmp", IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpKnightB     = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/figur_schwarz.bmp",  IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            bmpKnightM     = (HBITMAP)LoadImage(GetModuleHandle(NULL), "bmp/springer_mask.bmp",  IMAGE_BITMAP, 64, 64, LR_LOADFROMFILE);
            HDC     memHdc = CreateCompatibleDC(hdc);   // Image-Buffer
            HDC     bmpHdc = CreateCompatibleDC(hdc);   // Bitmap-Buffer
            HBITMAP memHbm = CreateCompatibleBitmap(hdc, winWidth, winHeight);  // Canvas

            HBITMAP hBitmap = (HBITMAP)SelectObject(bmpHdc, bmpWhiteSquare);
            HANDLE  hOld    = (HANDLE)SelectObject(memHdc, memHbm);

            //__________________________________________Draw Chess Board________________________________________

            for (int y = 0; y < BOARD_HEIGHT; ++y){  // Iterate through chessboard squares
                for (int x = 0; x < BOARD_WIDTH; ++x){
                    if (square[y][x].getColor() == WHITE){   // Check if current square is WHITE
                        square[y][x].drawBitmap(memHdc, bmpHdc, bmpWhiteSquare);    // If current square is WHITE, draw white square to memory buffer
                    } else {
                        square[y][x].drawBitmap(memHdc, bmpHdc, bmpBlackSquare);    // If current square is not WHITE, draw black square to memory buffer
                    }
                }
            }

            //__________________________________________Draw Chessmen___________________________________________

            for (int i = 0; i < 32; ++i){   // Iterate through chessMen
                if (chessMen[i].getColor() == WHITE && chessMen[i].getVisible()){   // Check if current chessMen is WHITE and visible
                    switch (chessMen[i].getType()){     // If current chessMan is WHITE, check for type
                        case PAWN:
                            chessMen[i].drawBitmap(memHdc, bmpHdc, bmpPawnW, bmpPawnM);     // If current chessMan is PAWN, draw white pawn to memory buffer
                            break;
                        case KNIGHT:
                            chessMen[i].drawBitmap(memHdc, bmpHdc, bmpKnightW, bmpKnightM); // If current chessMan is KNIGHT, draw white knight to memory buffer
                            break;
                    }
                } else if (chessMen[i].getVisible()) {
                    switch (chessMen[i].getType()){     // If current chessMan is not WHITE but visible, check for type
                        case PAWN:
                            chessMen[i].drawBitmap(memHdc, bmpHdc, bmpPawnB, bmpPawnM);     // If current chessMan is PAWN, draw black pawn to memory buffer
                            break;
                        case KNIGHT:
                            chessMen[i].drawBitmap(memHdc, bmpHdc, bmpKnightB, bmpKnightM); // If current chessMan is KNIGHT, draw black knight to memory buffer
                            break;
                    }
                }
            }

            //__________________________________________Draw Highlight__________________________________________

            if (highlight.getVisible()){                                                // Check if highlight is visible
                highlight.drawBitmap(memHdc, bmpHdc, bmpHighlight, bmpHighlightM);      // If highlight is visible, draw to memory buffer
            }

            //__________________________________________Draw Selection__________________________________________

            if(selection.getVisible() && selection.validPos()){                         // Check if selection is visible and has a valid position
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

        Sleep(20);  // Sleep for 20 milliseconds to reduce CPU load
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
                int hPosX          = highlight.getPosX();   // Assign position of highlight and selection to variables for clarity
                int hPosY          = highlight.getPosY();
                int sPosX          = selection.getPosX();
                int sPosY          = selection.getPosY();

                bool hSquareHasID  = square[hPosY][hPosX].getHasID();   // Assign hasID and containedID of highlighted square and selected square to variables for clarity
                int  hSquareContID = square[hPosY][hPosX].getContID();

                bool sSquareHasID  = square[sPosY][sPosX].getHasID();
                int  sSquareContID = square[sPosY][sPosX].getContID();

                if (selection.getVisible() && sSquareHasID){        // Check if selection is visible and selected square has an ID
                    int hi = chessMen[0].getIndex(hSquareContID);   // Get index of chessman on highlighted square
                    int si = chessMen[0].getIndex(sSquareContID);   // Get index of chessman on selected square

                    if (hSquareHasID){ // Check if highlighted square has an ID
                        // If highlighted square has an ID, check if selected and highlighted chessMan have the same color
                        if (chessMen[si].getColor() != chessMen[hi].getColor()){
                            chessMen[hi].setVisible(false); // If selected and highlighted chessMan don't have the same color, set visibility of highlighted chessMan to false

                            moveChessMan(si);               // Move selected chessMan
                        }
                    } else {
                        moveChessMan(si);  // If highlighted square doesn't have an ID, move selected chessMan
                    }

                    selection.setVisible(false);    // Set visibility of selection to false
                } else {
                    // If selection is not visible and selected square doesn't have an ID, set selection visibility to true and invalidate position, so a new position can be assigned
                    selection.setVisible(true);
                    selection.invalidatePos();
                }
            } else {
                selection.setVisible(false);    // If cursor is not within the chessboard, set selection visibility to false
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

void moveChessMan(int i){
    chessMen[i].moveTo(highlight.getPosX(), highlight.getPosY());   // Move selected chessMan to highlighted square

    square[selection.getPosY()][selection.getPosX()].setContID(0);  // Set containedID of selected square to 0
    square[highlight.getPosY()][highlight.getPosX()].setContID(chessMen[i].getID());   // Set containedID of highlighted square to ID of moved chessman
}
