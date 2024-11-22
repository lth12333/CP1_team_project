#include <stdio.h> 

#include<string.h>
#include "game.h"

void gameLoop() {
    char select_piece[3];
    char arrival_point[3];
    int success_check = 0;
    initBoard();
    gameState.turn = 0; // 백색으로 시작
    gameState.game_over = 0; // 게임이 아직 끝나지 않음
    printBoard();
    while (!gameState.game_over) {
        // 기물 위치 입력
        success_check = 0;
        while (success_check == 0) {
            printf("현재 차례: %s\n", gameState.turn == 0 ? "백색" : "흑색");
            printf("기물 위치를 입력하세요 (예시:a1): ");
            scanf("%s", select_piece);
            printf("이동할 위치를 입력하세요 (예시:b2): ");
            scanf("%s", arrival_point);
            success_check = select_move_Piece(select_piece, arrival_point);
            if (success_check == 0) {
                printf("오류!\n");
            }
            printBoard();
        }
        
        if (success_check == 1) {
            printf("기물이 이동했습니다!\n");
            gameState.turn = !gameState.turn; //턴 변경
        }
        

        // 간단한 게임 종료 조건
        if (gameState.turn > 10) { // 10턴 제한
            gameState.game_over = 1;
            printf("게임 종료!\n");
        }
    }
}

int main() {
    gameLoop();
    return 0;
}
