// game.h
#ifndef GAME_H
#define GAME_H

#include "piece.h"

#define BOARD_SIZE 8

// 전역 변수: 체스 보드 배열
extern Piece board[BOARD_SIZE][BOARD_SIZE];

// 게임 상태를 추적하기 위한 구조체
typedef struct {
    int turn;         // 현재 차례 (0 = 백색, 1 = 흑색)
    int game_over;    // 게임 종료 여부 (0 = 진행 중, 1 = 종료)
} GameState;

extern GameState gameState;

// 함수 선언
void initBoard();
void printBoard();
int select_move_Piece(char select_piece[3], char move_piece[3]);

#endif
#pragma once
