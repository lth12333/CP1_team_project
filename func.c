// func.c
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "game.h"


// 전역 변수 정의
Piece board[BOARD_SIZE][BOARD_SIZE];

GameState gameState = { 0, 0 };

int BKp[2] = { 0, 4 }, WKp[2] = {7, 4}; //y, x

const int R_Q[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
const int N[8][2] = { {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2} };
const int B_Q[4][2] = { {1, 1}, {-1, 1}, {-1, -1}, {1, -1} };
const int K[8][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1} };

void initTest() { //테스트 보드, 피스들을 추가하고 테스트 하세요
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = (Piece){ ' ', ' ', 0, 0 }; //체스판 비우기
        }
    }
   
    board[4][0] = (Piece){ 'K', 'B', 0, 0, 0 }; // 흑색 킹
    board[4][7] = (Piece){ 'B', 'W', 0, 0, 0 }; // 백색 룩
   
    board[6][4] = (Piece){ 'P', 'B', 0, 0, 0 }; // 백색 킹
    board[1][1] = (Piece){ 'P', 'W', 0, 0, 0 };
   
}
void in_passing_reset() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].color == (gameState.turn % 2 == 0 ? 'W' : 'B')) {
                board[i][j].in_passing_check = 0;
            }
        }
    }
}
void board_reset() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j].warning = 0;
            if (board[i][j].type == '"') {
                board[i][j].type = ' ';
            }
        }
    }
}

void initBoard() {
    // 기본적인 초기화 (예: 폰 배치)
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[1][i] = (Piece){ 'P', 'B', 0, 0, 0 }; // 흑색 폰
        board[6][i] = (Piece){ 'P', 'W', 0, 0, 0 }; // 백색 폰
    }
    board[0][0] = board[0][7] = (Piece){ 'R', 'B', 0, 0, 0 }; // 흑색 룩
    board[0][1] = board[0][6] = (Piece){ 'N', 'B', 0, 0, 0 }; // 흑색 나이트
    board[0][2] = board[0][5] = (Piece){ 'B', 'B', 0, 0, 0 }; // 흑색 비숍
    board[0][3] = (Piece){ 'Q', 'B', 0, 0, 0 }; // 흑색 퀸
    board[0][4] = (Piece){ 'K', 'B', 0, 0, 0 }; // 흑색 킹
    board[7][0] = board[7][7] = (Piece){ 'R', 'W', 0, 0, 0 }; // 백색 룩
    board[7][1] = board[7][6] = (Piece){ 'N', 'W', 0, 0, 0 }; // 백색 나이트
    board[7][2] = board[7][5] = (Piece){ 'B', 'W', 0, 0, 0 }; // 백색 비숍
    board[7][3] = (Piece){ 'Q', 'W', 0, 0, 0 }; // 백색 퀸
    board[7][4] = (Piece){ 'K', 'W', 0, 0, 0 }; // 백색 킹
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = (Piece){ ' ', ' ', 0, 0, 0 }; // 미지정
        }
    }
}
// 위의 함수는 체스판 초기화를 담당
void printBoard() {
    for (int i = 0; i < BOARD_SIZE + 1; i++) {
        if (i == 8) {
            printf("---|---|---|---|---|---|---|---|---|\n   | a | b | c | d | e | f | g | h |\n");
        }
        else {
            printf("---|---|---|---|---|---|---|---|---|\n %c |", 56 - i);
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j].warning == 1) {
                    printf("\033[0;31m%c%c\033[0m |", board[i][j].color, board[i][j].type);
                }
                else {
                    if (board[i][j].color == 'B') {
                        printf("\033[0;36m%c%c\033[0m |", board[i][j].color, board[i][j].type);
                    }
                    else if(board[i][j].color == 'W') {
                        printf("\033[0;33m%c%c\033[0m |", board[i][j].color, board[i][j].type);
                    }
                    else {
                        printf("%c%c |", board[i][j].color, board[i][j].type);
                    }
                }
            }
        }
        printf("\n");
    }

}
int lineCheck(char color, int row, int col, char set[]) {
    int y = (color == 'B' ? BKp[0] : WKp[0]);
    int x = (color == 'B' ? BKp[1] : WKp[1]);
    while (1) {
        y += row;
        x += col;
        if (0 > y || y > 7 || 0 > x || x > 7 || color == board[y][x].color) {
            return 0;
        }
        else {
            if(strchr(set, board[y][x].type) != NULL) {
                return 1;
            }
            else if (board[y][x].type != '"' && board[y][x].type != ' ') {
                return 0;
            }
        }
    }
}
// 위의 함수는 킹을 시작으로 특정 라인에 특정 기물이 있는지 확인하는 함수
int identifyCheck(char color) {
    for (int i = 0; i < 4; i++) {
        if (lineCheck(color, R_Q[i][0], R_Q[i][1], "RQ") || lineCheck(color, B_Q[i][0], B_Q[i][1], "BQ")){
            return 1;
        }
    }
    for (int i = 0; i < 8; i++) {
        int y = (color == 'B' ? BKp[0] + N[i][0] : WKp[0] + N[i][0]);
        int x = (color == 'B' ? BKp[1] + N[i][1] : WKp[1] + N[i][1]);
        if (0 > y || y > 7 || 0 > x || x > 7) {
            continue;
        }
        if (color != board[y][x].color && board[y][x].type == 'N') {
            return 1;
        }
    } 
    for (int i = 0; i < 8; i++) {
        int y = (color == 'B' ? BKp[0] + K[i][0] : WKp[0] + K[i][0]);
        int x = (color == 'B' ? BKp[1] + K[i][1] : WKp[1] + K[i][1]);
        if (0 > y || y > 7 || 0 > x || x > 7) {
            continue;
        }
        if (color != board[y][x].color && board[y][x].type == 'K') {
            return 1;
        }
    }
    if (color == 'B') {
        int y = BKp[0] + 1;
        if (y <= 7) {
            int x = BKp[1] - 1;
            if (x >= 0 && board[y][x].type == 'P' && board[y][x].color == 'W') {
                return 1;
            }
            x = BKp[1] + 1;
            if (x <= 7 && board[y][x].type == 'P' && board[y][x].color == 'W') {
                return 1;
            }
        }
    }
    if (color == 'W') {
        int y = WKp[0] - 1;
        if (y >= 0) {
            int x = WKp[1] - 1;
            if (x >= 0 && board[y][x].type == 'P' && board[y][x].color == 'B') {
                return 1;
            }
            x = WKp[1] + 1;
            if (x <= 7 && board[y][x].type == 'P' && board[y][x].color == 'B') {
                return 1;
            }
        }

    }
    return 0;
}
int in_passing(int s_y, int s_x, int a_y, int a_x) {
    int check;
    char type_temp, color_temp;

    type_temp = board[s_y][a_x].type;
    color_temp = board[s_y][a_x].color;
    board[a_y][a_x].type = board[s_y][s_x].type;
    board[a_y][a_x].color = board[s_y][s_x].color;
    board[s_y][s_x].type = board[s_y][a_x].type = ' ';
    board[s_y][s_x].color = board[s_y][a_x].type = ' ';

    check = identifyCheck(board[a_y][a_x].color); //체크면 1, 아니면 0

    board[s_y][s_x].type = board[a_y][a_x].type;
    board[s_y][s_x].color = board[a_y][a_x].color;
    board[a_y][a_x].type = ' ';
    board[a_y][a_x].color = ' ';
    board[s_y][a_x].type = type_temp;
    board[s_y][a_x].color = color_temp;

    if (check == 0) {
        board[a_y][a_x].type = '"';
        board[s_y][s_x].arrival_point_count += 1;

    }
}

int point_pv(int s_y, int s_x, int a_y, int a_x) {
    int check, in_passing = 0;
    char type_temp, color_temp;

    if (board[s_y][s_x].type == 'K') {
        if (board[s_y][s_x].color == 'B') {
            BKp[0] = a_y;
            BKp[1] = a_x;
        }
        else {
            WKp[0] = a_y;
            WKp[1] = a_x;
        }
    }

    type_temp = board[a_y][a_x].type;
    color_temp = board[a_y][a_x].color;

    board[a_y][a_x].type = board[s_y][s_x].type;
    board[a_y][a_x].color = board[s_y][s_x].color;
    board[s_y][s_x].type = ' ';
    board[s_y][s_x].color = ' ';

    check = identifyCheck(board[a_y][a_x].color); //체크면 1, 아니면 0
    
    if (board[a_y][a_x].type == 'K') {
        if (board[a_y][a_x].color == 'B') {
            BKp[0] = s_y;
            BKp[1] = s_x;
        }
        else {
            WKp[0] = s_y;
            WKp[1] = s_x;
        }
    }
    
    board[s_y][s_x].type = board[a_y][a_x].type;
    board[s_y][s_x].color = board[a_y][a_x].color;
    board[a_y][a_x].type = type_temp;
    board[a_y][a_x].color = color_temp;

    if (check == 0) {
        if (board[a_y][a_x].type == ' ') {
            board[a_y][a_x].type = '"';
            board[s_y][s_x].arrival_point_count += 1;
            return 0;
        }
        else {
            board[a_y][a_x].warning = 1;
            board[s_y][s_x].arrival_point_count += 1;
            return 1;
        }
    }
    else {
        if(board[a_y][a_x].type == ' ') {
            return 0;
        }
        return 1;
    }
}
void path_pv(int s_y, int s_x, int row, int col) {
    int y = s_y, x = s_x;
    while (1) {
        y += row;
        x += col;
        if (0 > y || y > 7 || 0 > x || x > 7){
            return;
        }
        else {
            if (board[s_y][s_x].color == board[y][x].color) {
                return;
            }
            else if (board[s_y][s_x].color != board[y][x].color) {
                if (point_pv(s_y, s_x, y, x)) {
                    return;
                }
            }
        }
    }
}

void pawn_pv(int s_y, int s_x) {
    if (board[s_y][s_x].color == 'B') {
        if (board[s_y + 1][s_x].type == ' ') {
            point_pv(s_y, s_x, s_y + 1, s_x);
            if (board[s_y + 2][s_x].type == ' ' && s_y == 1) {
                point_pv(s_y, s_x, s_y + 2, s_x);
            }
        }
        if (s_x - 1 > -1){
            if (board[s_y + 1][s_x - 1].color == 'W') {
                point_pv(s_y, s_x, s_y + 1, s_x - 1);
            }
            else if(board[s_y][s_x - 1].type == 'P' && board[s_y][s_x - 1].color == 'W' && board[s_y][s_x - 1].in_passing_check == 1){
                in_passing(s_y, s_x, s_y + 1, s_x - 1);
            }
        }
        if (s_x + 1 < 8) {
            if (board[s_y + 1][s_x + 1].color == 'W') {
                point_pv(s_y, s_x, s_y + 1, s_x + 1);
            }
            else if(board[s_y][s_x + 1].type == 'P' && board[s_y][s_x + 1].color == 'W' && board[s_y][s_x + 1].in_passing_check == 1) {
                in_passing(s_y, s_x, s_y + 1, s_x + 1);
            }
        }
    }
    else if (board[s_y][s_x].color == 'W') {
        if (board[s_y - 1][s_x].type == ' ') {
            point_pv(s_y, s_x, s_y - 1, s_x);
            if (board[s_y - 2][s_x].type == ' ' && s_y == 6) {
                point_pv(s_y, s_x, s_y - 2, s_x);
            }
        }
        if (s_x - 1 > -1) {
            if (board[s_y - 1][s_x - 1].color == 'B') {
                point_pv(s_y, s_x, s_y - 1, s_x - 1);
            }
            else if(board[s_y][s_x - 1].type == 'P' && board[s_y][s_x - 1].color == 'B' && board[s_y][s_x - 1].in_passing_check == 1){
                in_passing(s_y, s_x, s_y - 1, s_x - 1);
            }
        }
        if (s_x + 1 < 8) {
            if (board[s_y - 1][s_x + 1].color == 'B') {
                point_pv(s_y, s_x, s_y - 1, s_x + 1);
            }
            else if(board[s_y][s_x + 1].type == 'P' && board[s_y][s_x + 1].color == 'B' && board[s_y][s_x + 1].in_passing_check == 1) {
                in_passing(s_y, s_x, s_y - 1, s_x + 1);
            }
        }
    }
}
void rook_pv(int s_y, int s_x) { 
    for (int i = 0; i < 4; i++) {
        path_pv(s_y, s_x, R_Q[i][0], R_Q[i][1]);
    }
}
void knight_pv(int s_y, int s_x) {
    int y, x;
    for (int i = 0; i < 8; i++) {
        y = s_y + N[i][0];
        x = s_x + N[i][1];
        if (-1 < y && y < 8 && -1 < x && x < 8) {
            if (board[s_y][s_x].color != board[y][x].color) {
                point_pv(s_y, s_x, y, x);
            }
        }
    }
}
void bishop_pv(int s_y, int s_x) {
    for (int i = 0; i < 4; i++) {
        path_pv(s_y, s_x, B_Q[i][0], B_Q[i][1]);
    }
}
void queen_pv(int s_y, int s_x) {
    for (int i = 0; i < 8; i++) {
        path_pv(s_y, s_x, K[i][0], K[i][1]);
    }
}
void king_pv(int s_y, int s_x) {
    int x, y;
    for (int i = 0; i < 8; i++) {
        y = s_y + K[i][0];
        x = s_x + K[i][1];
        if (-1 < y && y < 8 && -1 < x && x < 8) {
            if (board[s_y][s_x].color != board[y][x].color) {
                point_pv(s_y, s_x, y, x);
            }
        }
    }
    char king_color = board[s_y][s_x].color;
    if (identifyCheck(king_color) != 1 && board[s_y][s_x].moving_count == 0) {
        if (board[s_y][5].type == '"' && board[s_y][6].type == ' ' &&
            board[s_y][7].type == 'R' && board[s_y][7].moving_count == 0) {
            if (king_color == 'B') {
                BKp[1] += 2;
                if (identifyCheck(king_color) != 1) {
                    board[s_y][6].type = '"';
                }
                BKp[1] -= 2;
            }
            else {
                WKp[1] += 2;
                if (identifyCheck(king_color) != 1) {
                    board[s_y][6].type = '"';
                }
                WKp[1] -= 2;
            }
        }
        if (board[s_y][3].type == '"' && board[s_y][2].type == ' ' &&
            board[s_y][0].type == 'R' && board[s_y][0].moving_count == 0) {
            if (king_color == 'B') {
                BKp[1] -= 2;
                if (identifyCheck(king_color) != 1) {
                    board[s_y][2].type = '"';
                }
                BKp[1] += 2;
            }
            else {
                WKp[1] -= 2;
                if (identifyCheck(king_color) != 1) {
                    board[s_y][6].type = '"';
                }
                WKp[1] += 2;
            }
        }
    }
}
void promote_pawn(int a_y, int a_x) {
    char type;
    char set[] = "RNBQ";
    if (board[a_y][a_x].type == 'P') {
        if ((board[a_y][a_x].color == 'B' && a_y == 7) || (board[a_y][a_x].color == 'W' && a_y == 0)) {
            board_reset();
            while(1) {
                printBoard();
                printf("\n무엇으로 프로모션을 하시겠습니까?(R = 룩, N = 나이트, B = 비숍, Q = 퀸)");
                scanf(" %c", &type);
                rewind(stdin);
                if (strchr(set, type) != NULL) {
                    break;
                }
                else {
                    printf("\n잘못입력하셨습니다.\n");
                }
            }
            board[a_y][a_x].type = type;  // 승격
            printf("\nP가 %c로 승격되었습니다!\n", type);
        }
    }
}


// 기물 이동 관리 함수

void along_type(int s_y, int s_x) {
    board[s_y][s_x].arrival_point_count = 0;
    switch (board[s_y][s_x].type) {
    case 'P':
        pawn_pv(s_y, s_x);
        return;
    case 'R':
        rook_pv(s_y, s_x);
        return;
    case 'N':
        knight_pv(s_y, s_x);
        return;
    case 'B':
        bishop_pv(s_y, s_x);
        return;
    case 'Q':
        queen_pv(s_y, s_x);
        return;
    case 'K':
        king_pv(s_y, s_x);
        return;
    };
}

int show_path_of_piece(char select_piece[10]) {
    // 범위 체크 및 기본 이동 조건  s = start
    int s_y = 56 - (int)select_piece[1], s_x = (int)select_piece[0] - 97; //start지점 0~7로 변환
    int s_len = strlen(select_piece); //문자 길이
    char color[2] = { 'W', 'B' };
    if (s_len == 2 && -1 < s_x && s_x < 8 && -1 < s_y && s_y < 8) {
        if (color[gameState.turn % 2] == board[s_y][s_x].color) {
            along_type(s_y, s_x);
            if (board[s_y][s_x].arrival_point_count == 0) {
                printf("\n현재 움직일 수 없는 기물입니다.\n\n");
                return 0;
            }
            printBoard();
            return 1;
        }
        else if (board[s_y][s_x].color == ' ') {
            printf("\n해당 칸은 빈칸입니다.\n\n");
            return 0;
        }
        else {
            printf("\n현재 턴에 움직일 수 없는 색입니다.\n\n");
            return 0;
        }
    }
    else {
        printf("\n지정된 범위가 아닙니다.\n\n");
        return 0;
    }
    
}

int move_along_path(char select_piece[10], char move_position[10]) {
    int s_y = 56 - (int)select_piece[1], s_x = (int)select_piece[0] - 97; 
    int a_y = 56 - (int)move_position[1], a_x = (int)move_position[0] - 97; 
    int a_len = strlen(move_position);
    if (a_len == 2 && -1 < a_x && a_x < 8 && -1 < a_y && a_y < 8) {
        if (board[a_y][a_x].type == '"' || board[a_y][a_x].warning == 1) {
            if (board[s_y][s_x].type == 'K') {
                if (a_x - s_x == 2) {
                    board[s_y][s_x + 1] = board[s_y][7];
                    board[s_y][7] = (Piece){' ', ' '};
                    board[s_y][s_x + 1].moving_count = 1;
                    board[s_y][7].moving_count = 0;
                    printf("\n%s의 캐슬링!\n", board[s_y][s_x].color == 'B' ? "흑색" : "백색");
                }
                else if (a_x - s_x == -2) {
                    board[s_y][s_x - 1] = board[s_y][0];
                    board[s_y][0] = (Piece){' ', ' '};
                    board[s_y][s_x - 1].moving_count = 1;
                    board[s_y][0].moving_count = 0;
                    printf("\n%s의 캐슬링!\n", board[s_y][s_x].color == 'B' ? "흑색" : "백색");
                }

                if (board[s_y][s_x].color == 'B') {
                    BKp[0] = a_y;
                    BKp[1] = a_x;
                }
                else {
                    WKp[0] = a_y;
                    WKp[1] = a_x;
                }
            }
            if (board[s_y][s_x].type == 'P' && abs(a_x - s_x) == 1 && board[a_y][a_x].type == '"') {
                board[s_y][a_x] = (Piece){ ' ', ' ', 0, 0, 0 };
            }
            board[a_y][a_x] = board[s_y][s_x];
            board[s_y][s_x] = (Piece){ ' ', ' ', 0, 0, 0 };
            board[a_y][a_x].moving_count += 1;
            printf("\n기물이 이동했습니다!\n\n");
            if (board[a_y][a_x].type == 'P' && abs(a_y - s_y) == 2) {
                board[a_y][a_x].in_passing_check = 1;
            }
            promote_pawn(a_y, a_x);
            return 1;
        }
        else {
            printf("\n해당 기물이 갈 수 없는 곳입니다.\n\n");
        }
    }
    else {
         printf("\n지정된 범위가 아닙니다.\n\n");
    }
}

int identifyMate() {
    char color = gameState.turn % 2 == 0 ? 'W' : 'B';
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(board[i][j].color == color){
                along_type(i, j);
                board_reset();
                if (board[i][j].arrival_point_count > 0) {
                    return 1;
                }
            }
        }
    }
    gameState.game_over = 1;
    if (identifyCheck(color) == 1) {
        printf("\n체크메이트!\n%s이 승리하였습니다! \n\n", color == 'B' ? "백색" : "흑색");
    }
    else {
        printf("\n%s이 움직일 수 있는 기물이 없습니다.\n스테일메이트, 무승부! \n\n", color == 'B' ? "흑색" : "백색");
    }
}

void anotherDraw() {
    int BB = 0, BN = 0, WB = 0, WN = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].type != 'K' && board[i][j].type != ' ') {
                if(board[i][j].type == 'B'){
                    if (board[i][j].color == 'B') {
                        BB += 1;
                    }
                    else {
                        WB += 1;
                    }
                }
                else if (board[i][j].type == 'N') {
                    if (board[i][j].color == 'B') {
                        BN += 1;
                    }
                    else {
                        WN += 1;
                    }
                }
                else {
                    return;
                }
            }
        }
    }
    if ((BN == 1 && WN == 1 ) || (BB == 1 && WB == 1) || (BN + WN == 1 && BB + WB == 0) || (BN + WN == 0 && BB + WB == 1)) {
        gameState.game_over = 1;
        printf("\n체크메이트가 일어날 수 없습니다.\n무승부!\n\n");
    }
}
