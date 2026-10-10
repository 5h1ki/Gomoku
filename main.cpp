#include <fstream>
#include <graphics.h>
#include <algorithm>
#include <cstring>
#include <cmath>

#define WIDTH 806
#define HEIGHT 806
#define PADDING 25
#define SIDEBAR_WIDTH 240

using namespace std;

struct Position
{
    int x = 0;
    int y = 0;
};

void drawInitialScreen()
{
    setinitmode(0);
    initgraph(WIDTH + SIDEBAR_WIDTH, HEIGHT, INIT_RENDERMANUAL);
    setbkcolor(EGERGB(222, 184, 135));
    cleardevice();
}

void drawTriangle(int gap, int cnt)
{
    int x = WIDTH + 55;
    int y;
    if (cnt % 2)
        y = PADDING + 0.5 * gap;
    else
        y = PADDING + 6 * gap;
    int size = 25;
    ege_point tri1[3] = {{x, y}, {x - size, y - size / 2}, {x - size, y + size / 2}};

    setfillcolor(EGERGB(222, 184, 135));
    ege_fillpoly(3, tri1);

    x = WIDTH + 50;
    if (cnt % 2)
        y = PADDING + 6 * gap;
    else
        y = PADDING + 0.5 * gap;
    size = 20;
    ege_point tri2[3] = {{x, y}, {x - size, y - size / 2}, {x - size, y + size / 2}};

    setfillcolor(RED);
    ege_fillpoly(3, tri2);
}

void drawGrid(int gap)
{
    for (int i = 0; i < 15; ++i)
    {
        setlinecolor(BLACK);
        line(PADDING + gap * i, PADDING, PADDING + gap * i, PADDING + 14 * gap);
    }
    for (int i = 0; i < 15; ++i)
    {
        setlinecolor(BLACK);
        line(PADDING, PADDING + gap * i, PADDING + 14 * gap, PADDING + gap * i);
    }

    setfillcolor(BLACK);
    ege_fillcircle(PADDING + 3 * gap, PADDING + 3 * gap, 5);
    ege_fillcircle(WIDTH - (PADDING + 3 * gap), PADDING + 3 * gap, 5);
    ege_fillcircle(WIDTH - (PADDING + 3 * gap), HEIGHT - (PADDING + 3 * gap), 5);
    ege_fillcircle(PADDING + 3 * gap, HEIGHT - (PADDING + 3 * gap), 5);
    ege_fillcircle(WIDTH / 2, HEIGHT / 2, 5);
}

void drawSidebar(int gap, int font_height)
{
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setfont(font_height, 0, "黑体");
    setcolor(EGERGB(178, 34, 34));
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2, PADDING + 0.5 * gap, "玩 家 1");
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2, PADDING + 6 * gap, "玩 家 2");

    setfillcolor(BLACK);
    ege_fillcircle(WIDTH + SIDEBAR_WIDTH / 2, PADDING + 0.5 * gap + font_height + 50, gap / 2 - 10);
    setfillcolor(WHITE);
    ege_fillcircle(WIDTH + SIDEBAR_WIDTH / 2, PADDING + 6 * gap + font_height + 50, gap / 2 - 10);
}

void drawRecord(int gap, int font_height, int count[])
{
    setbkmode(OPAQUE);
    settextjustify(LEFT_TEXT, TOP_TEXT);
    setfont(font_height, 0, "宋体");
    setcolor(RED);
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2 - 3 * font_height, PADDING + 0.5 * gap + font_height + 120, "%2d胜", count[0]);
    setcolor(BLACK);
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2 - font_height, PADDING + 0.5 * gap + font_height + 120, "%2d负", count[1]);
    setcolor(EGERGB(128, 128, 128));
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2 + font_height, PADDING + 0.5 * gap + font_height + 120, "%2d平", count[2]);

    setcolor(RED);
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2 - 3 * font_height, PADDING + 6 * gap + font_height + 120, "%2d胜", count[1]);
    setcolor(BLACK);
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2 - font_height, PADDING + 6 * gap + font_height + 120, "%2d负", count[0]);
    setcolor(EGERGB(128, 128, 128));
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2 + font_height, PADDING + 6 * gap + font_height + 120, "%2d平", count[2]);
}

void drawButtons(int gap)
{
    setcolor(EGERGB(114, 81, 33));
    setfillcolor(EGERGB(158, 59, 59));
    fillroundrect(WIDTH + 55, PADDING + 11 * gap, WIDTH + SIDEBAR_WIDTH - 55, PADDING + 12 * gap, 10, 10);
    fillroundrect(WIDTH + 55, PADDING + 12.6 * gap, WIDTH + SIDEBAR_WIDTH - 55, PADDING + 13.6 * gap, 10, 10);

    setbkmode(TRANSPARENT);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setfont(30, 0, "黑体");
    setcolor(EGERGB(222, 184, 135));
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2, PADDING + 11.5 * gap, "存  盘");
    xyprintf(WIDTH + SIDEBAR_WIDTH / 2, PADDING + 13.1 * gap, "复  盘");
}

void drawBorder(int gap)
{
    setfillcolor(EGERGB(180, 90, 80));
    ege_fillrect(0, 0, WIDTH + SIDEBAR_WIDTH, 2);
    ege_fillrect(0, 0, 2, HEIGHT);
    ege_fillrect(0, HEIGHT - 2, WIDTH + SIDEBAR_WIDTH, 2);
    ege_fillrect(WIDTH + SIDEBAR_WIDTH - 2, 0, 2, HEIGHT);
    ege_fillrect(WIDTH, 25, 1, HEIGHT - 50);
}

void drawToken(int gap, int grid[][15], int cnt)
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (grid[i][j] == 0)
                continue;
            if (grid[i][j] == 1)
                setfillcolor(BLACK);
            else if (grid[i][j] == 2)
                setfillcolor(WHITE);
            ege_fillcircle(PADDING + j * gap, PADDING + i * gap, gap / 2 - 10);
        }
    }
    drawTriangle(gap, cnt);
}

void drawEntry(int gap, int font_height, int count[])
{
    drawGrid(gap);
    drawSidebar(gap, font_height);
    drawRecord(gap, font_height, count);
    drawButtons(gap);
    drawBorder(gap);

    setfillcolor(EGEARGB(128, 0, 0, 0));
    ege_fillrect(0, 0, WIDTH, HEIGHT);

    setfillcolor(EGERGB(250, 248, 240));
    fillroundrect(200, 100, 600, 400, 15, 15);
    setcolor(EGERGB(158, 59, 59));
    setfillcolor(EGERGB(158, 59, 59));
    fillroundrect(200, 100, 600, 130, 15, 15);
    setfillcolor(EGERGB(250, 248, 240));
    ege_fillrect(201, 117, 398, 20);

    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setfont(40, 0, "宋体");
    setcolor(EGERGB(200, 150, 0));
    xyprintf(WIDTH / 2, 200, "继续游戏");
    line(WIDTH / 2 - 80, 225, WIDTH / 2 + 80, 225);
    xyprintf(WIDTH / 2, 300, "新游戏");
    line(WIDTH / 2 - 60, 325, WIDTH / 2 + 60, 325);
}

void drawBoard(int gap, int font_height, int count[], int cnt)
{
    setfillcolor(EGERGB(222, 184, 135));
    ege_fillrect(0, 0, WIDTH, HEIGHT);
    drawGrid(gap);
    drawRecord(gap, font_height, count);
    drawBorder(gap);
    drawTriangle(gap, cnt);
}

void drawCard(int count[], int winner)
{
    setfillcolor(EGEARGB(128, 0, 0, 0));
    ege_fillrect(0, 0, WIDTH, HEIGHT);

    setfillcolor(EGERGB(250, 248, 240));
    fillroundrect(200, 100, 600, 400, 15, 15);
    setcolor(EGERGB(158, 59, 59));
    setfillcolor(EGERGB(158, 59, 59));
    fillroundrect(200, 100, 600, 130, 15, 15);
    setfillcolor(EGERGB(250, 248, 240));
    ege_fillrect(201, 117, 398, 20);

    setbkmode(TRANSPARENT);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    setfont(40, 0, "黑体");
    setcolor(EGERGB(200, 150, 0));
    if (winner == 1)
    {
        count[0]++;
        xyprintf(WIDTH / 2, 200, "黑棋获胜(๑•̀ㅂ•́)و✧");
    }
    else if (winner == 2)
    {
        count[1]++;
        xyprintf(WIDTH / 2, 200, "白棋获胜(｡•̀ᴗ-)✧");
    }
    else
    {
        count[2]++;
        xyprintf(WIDTH / 2, 200, "黑白平局(￣▽￣)\"");
    }

    ege_point tri[3] = {{290, 300}, {276, 292}, {276, 308}};
    setfillcolor(EGERGB(180, 180, 180));
    ege_fillpoly(3, tri);
    setfont(20, 0, "宋体");
    setcolor(EGERGB(64, 64, 64));
    xyprintf(WIDTH / 2, 300, "点击屏幕任意处继续...");
}

bool gameOver(bool &game_over, int &winner, int grid[][15], int cnt)
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j + 4 < 15; j++)
        {
            if (grid[i][j] == 1 && grid[i][j + 1] == 1 && grid[i][j + 2] == 1 && grid[i][j + 3] == 1 && grid[i][j + 4] == 1)
            {
                game_over = true;
                winner = 1;
                return true;
            }
            else if (grid[i][j] == 2 && grid[i][j + 1] == 2 && grid[i][j + 2] == 2 && grid[i][j + 3] == 2 && grid[i][j + 4] == 2)
            {
                game_over = true;
                winner = 2;
                return true;
            }
        }
    }
    for (int i = 0; i + 4 < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (grid[i][j] == 1 && grid[i + 1][j] == 1 && grid[i + 2][j] == 1 && grid[i + 3][j] == 1 && grid[i + 4][j] == 1)
            {
                game_over = true;
                winner = 1;
                return true;
            }
            else if (grid[i][j] == 2 && grid[i + 1][j] == 2 && grid[i + 2][j] == 2 && grid[i + 3][j] == 2 && grid[i + 4][j] == 2)
            {
                game_over = true;
                winner = 2;
                return true;
            }
        }
    }
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (i + 4 < 15 && j + 4 < 15)
            {
                if (grid[i][j] == 1 && grid[i + 1][j + 1] == 1 && grid[i + 2][j + 2] == 1 && grid[i + 3][j + 3] == 1 && grid[i + 4][j + 4] == 1)
                {
                    game_over = true;
                    winner = 1;
                    return true;
                }
                else if (grid[i][j] == 2 && grid[i + 1][j + 1] == 2 && grid[i + 2][j + 2] == 2 && grid[i + 3][j + 3] == 2 && grid[i + 4][j + 4] == 2)
                {
                    game_over = true;
                    winner = 2;
                    return true;
                }
            }
            if (i + 4 < 15 && j - 4 >= 0)
            {
                if (grid[i][j] == 1 && grid[i + 1][j - 1] == 1 && grid[i + 2][j - 2] == 1 && grid[i + 3][j - 3] == 1 && grid[i + 4][j - 4] == 1)
                {
                    game_over = true;
                    winner = 1;
                    return true;
                }
                else if (grid[i][j] == 2 && grid[i + 1][j - 1] == 2 && grid[i + 2][j - 2] == 2 && grid[i + 3][j - 3] == 2 && grid[i + 4][j - 4] == 2)
                {
                    game_over = true;
                    winner = 2;
                    return true;
                }
            }
        }
    }
    if (cnt == 225 && !game_over)
    {
        game_over = true;
        return true;
    }

    return false;
}

void saveGame(int grid[][15], Position memory[], int count[], int cnt)
{
    ofstream ofs1("grid_data.txt");
    ofstream ofs2("memory_data.txt");
    ofstream ofs3("count_data.txt");
    ofs1 << cnt << '\n';
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
            ofs1 << grid[i][j] << ' ';
        ofs1 << '\n';
    }
    for (int i = 0; i < 225; i++)
        ofs2 << memory[i].x << ' ' << memory[i].y << '\n';
    for (int i = 0; i < 3; i++)
        ofs3 << count[i] << ' ';

    ofs1.close();
    ofs2.close();
    ofs3.close();
}

void loadGame(int grid[][15], Position memory[], int count[], int &cnt)
{
    ifstream ifs1("grid_data.txt");
    ifstream ifs2("memory_data.txt");
    ifstream ifs3("count_data.txt");
    ifs1 >> cnt;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
            ifs1 >> grid[i][j];
    }
    for (int i = 0; i < 225; i++)
        ifs2 >> memory[i].x >> memory[i].y;
    for (int i = 0; i < 3; i++)
        ifs3 >> count[i];

    ifs1.close();
    ifs2.close();
    ifs3.close();
}

int main()
{
    drawInitialScreen();
    ege_enable_aa(true);

    int gap = (WIDTH - 2 * PADDING) / 14;
    int font_height = 30;
    int grid[15][15] = {};
    Position memory[225];
    for (int i = 0; i < 225; i++)
    {
        memory[i].x = -1;
        memory[i].y = -1;
    }
    int count[3] = {};
    int cnt = 0;

    drawEntry(gap, font_height, count);

    // 选择继续游戏或是新游戏
    bool game_start = false;
    for (; is_run() && !game_start; delay_fps(60))
    {
        while (mousemsg())
        {
            mouse_msg msg = getmouse();
            if (msg.is_left() && msg.is_down())
            {
                int x = msg.x;
                int y = msg.y;
                if (x >= WIDTH / 2 - 80 && x <= WIDTH / 2 + 80 && y >= 160 && y <= 225)
                {
                    loadGame(grid, memory, count, cnt);
                    game_start = true;
                }
                else if (x >= WIDTH / 2 - 60 && x <= WIDTH / 2 + 60 && y >= 260 && y <= 325)
                {
                    saveGame(grid, memory, count, cnt);
                    game_start = true;
                }
            }
        }
    }

    while (is_run())
    {
        // 每局开始或者复盘结束
        drawBoard(gap, font_height, count, cnt);
        drawToken(gap, grid, cnt);

        bool game_over = false;
        int winner = 0;
        bool is_store = false, is_review = false;

        // 开始下棋
        for (; is_run() && !game_over; delay_fps(60))
        {
            // 存盘若是终局，则break
            if (gameOver(game_over, winner, grid, cnt))
                break;

            int pos_x = 0, pos_y = 0;
            bool is_click = false, is_found = false;

            // 检测鼠标消息
            while (mousemsg())
            {
                mouse_msg msg = getmouse();
                if (msg.is_left() && msg.is_down())
                {
                    int x = msg.x;
                    int y = msg.y;
                    if (x >= WIDTH + 55 && y >= PADDING + 11 * gap && x <= WIDTH + SIDEBAR_WIDTH - 55 && y <= PADDING + 12 * gap)
                        is_store = true;
                    else if (x >= WIDTH + 55 && y >= PADDING + 12.6 * gap && x <= WIDTH + SIDEBAR_WIDTH - 55 && y <= PADDING + 13.6 * gap)
                        is_review = true;

                    for (int i = 0; i < 15; i++)
                    {
                        for (int j = 0; j < 15; j++)
                        {
                            int point_x = PADDING + j * gap;
                            int point_y = PADDING + i * gap;
                            double r = gap / 2;
                            double dis = sqrt(pow(point_x - x, 2) + pow(point_y - y, 2));
                            if (r - dis > 1e-9)
                            {
                                pos_x = j;
                                pos_y = i;
                                is_found = true;
                                break;
                            }
                        }
                        if (is_found)
                            break;
                    }
                    is_click = true;
                }
            }
            // 存盘
            if (is_store)
            {
                saveGame(grid, memory, count, cnt);
                break;
            }
            // 复盘break
            else if (is_review)
                break;
            // 没有有效消息就继续下一次循环
            else if (!is_click || !is_found)
                continue;

            // 落子
            int k;
            for (k = 0; k < cnt; k++)
            {
                if (pos_x == memory[k].x && pos_y == memory[k].y)
                    break;
            }
            if (k == cnt)
            {
                memory[cnt++] = {pos_x, pos_y};
                grid[pos_y][pos_x] = (cnt % 2) ? 1 : 2;
            }
            else
                continue;

            drawToken(gap, grid, cnt);
            gameOver(game_over, winner, grid, cnt);
        }
        // 存盘break
        if (is_store)
            break;

        // 复盘
        if (is_review)
        {
            drawBoard(gap, font_height, count, 0);
            int cnt_tmp = 0;

            for (; is_run() && cnt_tmp < cnt; delay_fps(60))
            {
                while (mousemsg())
                {
                    mouse_msg msg = getmouse();
                    if (msg.is_left() && msg.is_down())
                    {
                        if (cnt_tmp >= cnt)
                            break;
                        if (!(cnt_tmp % 2))
                            setfillcolor(BLACK);
                        else
                            setfillcolor(WHITE);
                        ege_fillcircle(PADDING + memory[cnt_tmp].x * gap, PADDING + memory[cnt_tmp].y * gap, gap / 2 - 10);
                        cnt_tmp++;
                    }
                }
                drawTriangle(gap, cnt_tmp);
            }
            continue;
        }

        // 绘制胜利卡片
        drawCard(count, winner);

        // 等待下一次点击，重新显示终局棋盘
        while (true)
        {
            mouse_msg msg = getmouse();
            if (msg.is_left() && msg.is_down())
            {
                drawBoard(gap, font_height, count, cnt);
                drawToken(gap, grid, cnt);
                break;
            }
        }

        is_store = false;
        is_review = false;
        for (; is_run(); delay_fps(60))
        {
            // 检测鼠标消息
            bool is_click = false;
            while (mousemsg())
            {
                mouse_msg msg = getmouse();
                if (msg.is_left() && msg.is_down())
                {
                    if (msg.x >= WIDTH + 55 && msg.y >= PADDING + 11 * gap && msg.x <= WIDTH + SIDEBAR_WIDTH - 55 && msg.y <= PADDING + 12 * gap)
                        is_store = true;
                    else if (msg.x >= WIDTH + 55 && msg.y >= PADDING + 12.6 * gap && msg.x <= WIDTH + SIDEBAR_WIDTH - 55 && msg.y <= PADDING + 13.6 * gap)
                        is_review = true;
                    is_click = true;
                }
            }
            if (is_click)
                break;
        }
        // 存盘
        if (is_store)
        {
            saveGame(grid, memory, count, cnt);
            break;
        }
        // 复盘
        if (is_review)
        {
            drawBoard(gap, font_height, count, 0);
            int cnt_tmp = 0;

            for (; is_run() && cnt_tmp < cnt; delay_fps(60))
            {
                while (mousemsg())
                {
                    mouse_msg msg = getmouse();
                    if (msg.is_left() && msg.is_down())
                    {
                        if (cnt_tmp >= cnt)
                            break;
                        if (!(cnt_tmp % 2))
                            setfillcolor(BLACK);
                        else
                            setfillcolor(WHITE);
                        ege_fillcircle(PADDING + memory[cnt_tmp].x * gap, PADDING + memory[cnt_tmp].y * gap, gap / 2 - 10);
                        cnt_tmp++;
                    }
                }
                drawTriangle(gap, cnt_tmp);

                if (cnt_tmp == cnt)
                {
                    for (; is_run(); delay_fps(60))
                    {
                        // 复盘后继续检测鼠标消息，看是否需要存盘
                        bool is_click = false;
                        while (mousemsg())
                        {
                            mouse_msg msg = getmouse();
                            if (msg.is_left() && msg.is_down())
                            {
                                if (msg.x >= WIDTH + 55 && msg.y >= PADDING + 11 * gap && msg.x <= WIDTH + SIDEBAR_WIDTH - 55 && msg.y <= PADDING + 12 * gap)
                                    is_store = true;
                                is_click = true;
                            }
                        }
                        if (is_click)
                            break;
                    }
                    // 复盘后存盘
                    if (is_store)
                    {
                        saveGame(grid, memory, count, cnt);
                        break;
                    }
                }
            }
        }

        if (is_store)
            break;

        // 程序走到这里一局必定下完，重新设置grid，memory和cnt的值
        memset(grid, 0, sizeof(grid));
        for (int i = 0; i < 225; i++)
        {
            memory[i].x = -1;
            memory[i].y = -1;
        }
        cnt = 0;
    }
    return 0;
}