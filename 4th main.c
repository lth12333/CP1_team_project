#include <stdio.h> 

#include<string.h>
#include "game.h"

void gameLoop(int testing) { // 1 : Test, 0 : game
    char select_piece[10];
    char move_position[10];
    int success_check = 0;
    if (testing == 1) {
        initTest();
    }
    else {
        initBoard();
    }
    gameState.turn = 0; // 백색으로 시작
    gameState.game_over = 0; // 게임이 아직 끝나지 않음
    printBoard();
    while (!gameState.game_over) {
        // 기물 위치 입력
        success_check = 0;
        while (success_check == 0) {
            printf("\n현재 차례: %s\n\n", gameState.turn % 2 == 0 ? "백색" : "흑색");
            printf("기물 위치를 입력하세요 (예시:a1): ");
            scanf("%s", select_piece);
            if (show_path_of_piece(select_piece)) {
                printf("이동할 위치를 입력하세요 (예시:b2): ");
                scanf("%s", move_position);
                success_check = move_along_path(select_piece, move_position);
            }

            if(success_check == 1){
                gameState.turn += 1;
                identifyMate();
                anotherDraw();
                in_passing_reset();
            }
            board_reset();
            printBoard();
        }
    }
}

int main() {
    gameLoop(0);
    return 0;
}
