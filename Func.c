// func.c
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "game.h"

// 전역 변수 정의
Piece board[BOARD_SIZE][BOARD_SIZE];
GameState gameState = { 0, 0 };

// 체스판 초기화 함수
void initBoard() {
    // 기본적인 초기화 (예: 폰 배치)
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[1][i] = (Piece){ 'P', 'B' }; // 흑색 폰
        board[6][i] = (Piece){ 'P', 'W' }; // 백색 폰
    }
    board[0][0] = board[0][7] = (Piece){ 'R', 'B' }; // 흑색 룩
    board[0][1] = board[0][6] = (Piece){ 'N', 'B' }; // 흑색 나이트
    board[0][2] = board[0][5] = (Piece){ 'B', 'B' }; // 흑색 비숍
    board[0][3] = (Piece){ 'Q', 'B' }; // 흑색 퀸
    board[0][4] = (Piece){ 'K', 'B' }; // 흑색 킹
    board[7][0] = board[7][7] = (Piece){ 'R', 'W' }; // 백색 룩
    board[7][1] = board[7][6] = (Piece){ 'N', 'W' }; // 백색 나이트
    board[7][2] = board[7][5] = (Piece){ 'B', 'W' }; // 백색 비숍
    board[7][3] = (Piece){ 'Q', 'W' }; // 백색 퀸
    board[7][4] = (Piece){ 'K', 'W' }; // 백색 킹
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = (Piece){ ' ', ' ' }; // 미지정
        }
    }
    
    // 다른 기물 초기화 코드 추가 가능
}

// 체스판 출력 함수
void printBoard() {
    for (int i = 0; i < BOARD_SIZE + 1; i++) {
        if (i == 8) {
            printf("---|---|---|---|---|---|---|---|---|\n   | a | b | c | d | e | f | g | h |\n");
        }
        else {
            printf("---|---|---|---|---|---|---|---|---|\n %c |", 56 - i);
            for (int j = 0; j < BOARD_SIZE; j++) {
                printf("%c%c |", board[i][j].color, board[i][j].type);
            }
        }
        printf("\n");
    }
}

//기물 위치 변경 함수
int move_piece(Piece* start, Piece* arrival) {
    arrival->color = start->color;
    arrival->type = start->type;
    start->type = ' ';
    start->color = ' ';
    return 1;
}

//각 기물 이동 함수
int pawn(int s_y, int s_x, int a_y, int a_x) {
    if (board[s_y][s_x].color == 'B') {
        if (a_y - s_y == 1) {
            if (abs(a_x - s_x) == 1 && board[a_y][a_x].color == 'W') {
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
            else if (a_x == s_x && board[a_y][a_x].color == ' ') {
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
        }
    }
    if (a_y - s_y == -1) {
        if (abs(a_x - s_x) == 1 && board[a_y][a_x].color == 'B') {
            return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
        }
        else if (a_x == s_x && board[a_y][a_x].color == ' ') {
            return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
        }
    }
    return 0;
}
int rook(int s_y, int s_x, int a_y, int a_x) {
    if (board[s_y][s_x].color != board[a_y][a_x].color) {
        if (s_y == a_y) {
            if (s_x > a_x) {
                for (int i = s_x - 1; i > a_x; i--) {
                    if (board[a_y][i].color != ' ') {
                        return 0;
                    }
                }
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
            else {
                for (int i = s_x + 1; i < a_x; i++) {
                    if (board[a_y][i].color != ' ') {
                        return 0;
                    }
                }
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
        }
        else if (s_x == a_x) {
            if (s_y > a_y) {
                for (int i = s_y - 1; i > a_y; i--) {
                    if (board[i][a_x].color != ' ') {
                        return 0;
                    }
                }
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
            else {
                for (int i = s_y + 1; i < a_y; i++) {
                    if (board[i][a_x].color != ' ') {
                        return 0;
                    }
                }
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
        }
    }
    return 0;
}
int knight(int s_y, int s_x, int a_y, int a_x) {
    if (board[s_y][s_x].color != board[a_y][a_x].color) {
        if ((abs(a_y - s_y) == 2 && abs(a_x - s_x) == 1) || 
            (abs(a_y - s_y) == 1 && abs(a_x - s_x) == 2)) {
            return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
        }
    }
    return 0;
}
int bishop(int s_y, int s_x, int a_y, int a_x) {
    if (board[s_y][s_x].color != board[a_y][a_x].color && abs(a_y - s_y) == abs(a_x - s_x)) {
        if (a_y > s_y) {
            if (a_x > s_x) {
                for (int i = 1; i < a_y - s_y; i++) {
                    if (board[s_y + i][s_x + i].color != ' ') {
                        return 0;
                    }
                }
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
            else {
                for (int i = 1; i < a_y - s_y; i++) {
                    if (board[s_y + i][s_x - i].color != ' ') {
                        return 0;
                    }
                }
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);                
            }
        }
        else {
            if (a_x > s_x) {
                for (int i = 1; i < s_y - a_y; i++) {
                    if (board[s_y - i][s_x + i].color != ' ') {
                        return 0;
                    }
                }
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
            else {
                for (int i = 1; i < s_y - a_y; i++) {
                    if (board[s_y - i][s_x - i].color != ' ') {
                        return 0;
                    }
                }
                return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
            }
        }
    }        
    return 0;
}
int queen(int s_y, int s_x, int a_y, int a_x) {
    if (rook(s_y, s_x, a_y, a_x) || bishop(s_y, s_x, a_y, a_x)) {
        return 1;
    }
    return 0;
}
int king(int s_y, int s_x, int a_y, int a_x) {
    if (board[s_y][s_x].color != board[a_y][a_x].color && (abs(a_y - s_y) == 1 || abs(a_x - s_x) == 1)) {
        return move_piece(&board[s_y][s_x], &board[a_y][a_x]);
    }
    return 0;
}


// 기물 이동 관리 함수
int select_move_Piece(char select_piece[3], char arrival_point[3]) {
    // 범위 체크 및 기본 이동 조건  s = start, a = arrival
    int s_y = 56 - (int)select_piece[1], s_x = (int)select_piece[0] - 97; //start지점 0~7로 변환
    int a_y = 56 - (int)arrival_point[1], a_x = (int)arrival_point[0] - 97; //arrival지점 0~7로 변환
    int len = strlen(select_piece); //문자 길이
    char color[2] = { 'W', 'B' };
    if (len == 2 && -1 < s_x && s_x < 8 && -1 < s_y && s_y < 8 && (s_x != a_x || s_y != a_y) &&
        -1 < a_x && a_x < 8 && -1 < a_y && a_y < 8 && color[gameState.turn] == board[s_y][s_x].color) 
    {
        switch (board[s_y][s_x].type) {
        case 'P':
            return pawn(s_y, s_x, a_y, a_x);
        case 'R':
            return rook(s_y, s_x, a_y, a_x);
        case 'N':
            return knight(s_y, s_x, a_y, a_x);
        case 'B':
            return bishop(s_y, s_x, a_y, a_x);
        case 'Q':
            return queen(s_y, s_x, a_y, a_x);
        case 'K':
            return king(s_y, s_x, a_y, a_x);
        default:
            return 0;
        };
    }
    else {
        return 0;
    }
}
