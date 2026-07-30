#include <windows.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;

const int BULLET_COUNT = 20;
const int ENEMY_COUNT = 5;

const int HEIGHT = 20;
const int WIDTH = 30;

struct Object {
    int x;
    int y;
    bool active;
};

int PlayerX = WIDTH / 2;
int score = 0;

Object bullets[BULLET_COUNT];
Object enemies[ENEMY_COUNT];

void gotoXY(int x, int y) {
    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        pos
    );
}

void init() {
    for (int i = 0; i<BULLET_COUNT; ++i) {
        bullets[i].active = false;
    }

    for (int i = 0; i<ENEMY_COUNT; ++i) {
        enemies[i].active = true;
        enemies[i].x = rand() % WIDTH;
        enemies[i].y = rand() % 5;
    }
}

void draw() {

    gotoXY(0,0);

    char screen[HEIGHT][WIDTH];

    for (int y = 0; y<HEIGHT;++y) {
        for (int x = 0; x<WIDTH; ++x) {
            screen[y][x] = ' ';
        }
    }

    screen[HEIGHT-1][PlayerX] = 'A';

    for (int i = 0; i<BULLET_COUNT;++i) {
        if (bullets[i].active) {
            screen[bullets[i].y][bullets[i].x] = '|';
        }
    }

    for (int i = 0; i<ENEMY_COUNT;i++) {
        if (enemies[i].active)
            screen[enemies[i].y][enemies[i].x] = 'X';
    }

    for (int y = 0; y<HEIGHT;++y) {
        for (int x = 0; x<WIDTH;++x) {
            cout << screen[y][x];
        }

        cout << "\n";
    }

    cout << "Score: " << score;
}

void shoot() {
    for (int i = 0; i<BULLET_COUNT;i++) {
        if (!bullets[i].active){ 
            bullets[i].active = true;
            bullets[i].x = PlayerX;
            bullets[i].y = HEIGHT - 2;

            break;
        }
    }
}

void input() {
    if (_kbhit()) {
        char key = _getch();

        if (key == 'q' && PlayerX >0) {
            PlayerX--;
        }

        if (key == 'd' && PlayerX < WIDTH - 1) {
            PlayerX++;
        }

        if (key == ' ') {
            shoot();
        }
    }
}

void update() {

    for (int i = 0; i<BULLET_COUNT; ++i) {
        if (bullets[i].active) {
            bullets[i].y--;

            if (bullets[i].y<0) {
                bullets[i].active = false;
            }
        }
    }

    for (int i = 0; i<ENEMY_COUNT; ++i) {
        enemies[i].y++;

        if (enemies[i].y >=HEIGHT) {
            enemies[i].y = 0;
            enemies[i].x = rand() % WIDTH;
        }
            
    }

    for (int b = 0; b<BULLET_COUNT;++b) {
        if (!bullets[b].active) {
            continue;
        }

        for (int e = 0; e<ENEMY_COUNT;++e) {
            if (bullets[b].x == enemies[e].x && bullets[b].y == enemies[e].y) {
                score++;
                bullets[b].active = false;
                enemies[e].x = rand() % WIDTH;
                enemies[e].y = 0;
            }
        }

    }
}

int main() {
    srand(time(0));
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 1;
    cursor.bVisible = false;

    SetConsoleCursorInfo(
        GetStdHandle(STD_OUTPUT_HANDLE),
        &cursor
    );

    init();

    while(true) {
        draw();
        input();
        update();

        Sleep(16);
    }

    return 0;

}