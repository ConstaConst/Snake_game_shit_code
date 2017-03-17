#include <iostream>
#include <zconf.h>
#include <termios.h>
#include <stropts.h>
#include <asm/ioctls.h>

using namespace std;
bool  gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirecton {STOP, LEFT, RIGHT, UP, DOWN};
eDirecton dir;

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}
void Draw() {
    system("clear");
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#";

            if (i == y && j == x)
                cout << "O";
            else
                if (i == fruitY && j == fruitX)
                    cout << "F";
                else {
                    bool print = false;
                    for(int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o";
                            print = true;
                        }
                    }
                    if (!print)
                        cout << " ";
                }
            if (j == width - 1)
                cout << "#";
        }
        cout << endl;
    }

     for(int i = 0; i < width + 2; i++)
         cout << "#";
    cout << endl;
    cout << "Score:" << score << endl;
}
void Input() {
    if(_kbhit()) {
        switch (cin.get()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
            default:
                break;
        }
    }
}
void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
        case LEFT:
            if (x - 1 != tailX[1])
                x--;
            break;
        case RIGHT:
            if (x + 1 != tailX[1])
                x++;
            break;
        case UP:
            if (y - 1 != tailY[1])
            y--;
            break;
        case DOWN:
            if (y - 1 != tailY[1])
            y++;
            break;
        default:
            break;
    }
    //if (x > width - 1 || x < 0 || y > height - 1 || y < 0)
        //gameOver = true;
    if (x <= 0)
        x = width - 1;
    else
        if (x > width - 1)
            x = 0;
    if (y <= 0)
        y = height - 1;
    else
    if (y > height - 1)
        y = 0;

    for(int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if ( x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}
int main() {
    Setup();
    while(!gameOver) {
        Draw();
        Input();
        Logic();
        usleep(200000);

    }
    return 0;
}