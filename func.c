// func.c
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include "game.h"


// ���� ���� ����
Piece board[BOARD_SIZE][BOARD_SIZE];

GameState gameState = { 0, 0 };

int BKp[2] = { 0, 4 }, WKp[2] = {7, 4}; //y, x

const int R_Q[4][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
const int N[8][2] = { {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2} };
const int B_Q[4][2] = { {1, 1}, {-1, 1}, {-1, -1}, {1, -1} };
const int K[8][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {-1, 1}, {-1, -1}, {1, -1} };

void initTest() { //�׽�Ʈ ����, �ǽ����� �߰��ϰ� �׽�Ʈ �ϼ���
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = (Piece){ ' ', ' ', 0, 0 }; //ü���� ����
        }
    }
   
    board[4][0] = (Piece){ 'K', 'B', 0, 0, 0 }; // ��� ŷ
    board[4][7] = (Piece){ 'B', 'W', 0, 0, 0 }; // ��� ��
   
    board[6][4] = (Piece){ 'P', 'B', 0, 0, 0 }; // ��� ŷ
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
    // �⺻���� �ʱ�ȭ (��: �� ��ġ)
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[1][i] = (Piece){ 'P', 'B', 0, 0, 0 }; // ��� ��
        board[6][i] = (Piece){ 'P', 'W', 0, 0, 0 }; // ��� ��
    }
    board[0][0] = board[0][7] = (Piece){ 'R', 'B', 0, 0, 0 }; // ��� ��
    board[0][1] = board[0][6] = (Piece){ 'N', 'B', 0, 0, 0 }; // ��� ����Ʈ
    board[0][2] = board[0][5] = (Piece){ 'B', 'B', 0, 0, 0 }; // ��� ���
    board[0][3] = (Piece){ 'Q', 'B', 0, 0, 0 }; // ��� ��
    board[0][4] = (Piece){ 'K', 'B', 0, 0, 0 }; // ��� ŷ
    board[7][0] = board[7][7] = (Piece){ 'R', 'W', 0, 0, 0 }; // ��� ��
    board[7][1] = board[7][6] = (Piece){ 'N', 'W', 0, 0, 0 }; // ��� ����Ʈ
    board[7][2] = board[7][5] = (Piece){ 'B', 'W', 0, 0, 0 }; // ��� ���
    board[7][3] = (Piece){ 'Q', 'W', 0, 0, 0 }; // ��� ��
    board[7][4] = (Piece){ 'K', 'W', 0, 0, 0 }; // ��� ŷ
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = (Piece){ ' ', ' ', 0, 0, 0 }; // ������
        }
    }
}
// ���� �Լ��� ü���� �ʱ�ȭ�� ���
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
// ���� �Լ��� ŷ�� �������� Ư�� ���ο� Ư�� �⹰�� �ִ��� Ȯ���ϴ� �Լ�
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

    check = identifyCheck(board[a_y][a_x].color); //üũ�� 1, �ƴϸ� 0

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

    check = identifyCheck(board[a_y][a_x].color); //üũ�� 1, �ƴϸ� 0
    
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
                printf("\n�������� ���θ���� �Ͻðڽ��ϱ�?(R = ��, N = ����Ʈ, B = ���, Q = ��)");
                scanf(" %c", &type);
                rewind(stdin);
                if (strchr(set, type) != NULL) {
                    break;
                }
                else {
                    printf("\n�߸��Է��ϼ̽��ϴ�.\n");
                }
            }
            board[a_y][a_x].type = type;  // �°�
            printf("\nP�� %c�� �°ݵǾ����ϴ�!\n", type);
        }
    }
}


// �⹰ �̵� ���� �Լ�

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
    // ���� üũ �� �⺻ �̵� ����  s = start
    int s_y = 56 - (int)select_piece[1], s_x = (int)select_piece[0] - 97; //start���� 0~7�� ��ȯ
    int s_len = strlen(select_piece); //���� ����
    char color[2] = { 'W', 'B' };
    if (s_len == 2 && -1 < s_x && s_x < 8 && -1 < s_y && s_y < 8) {
        if (color[gameState.turn % 2] == board[s_y][s_x].color) {
            along_type(s_y, s_x);
            if (board[s_y][s_x].arrival_point_count == 0) {
                printf("\n���� ������ �� ���� �⹰�Դϴ�.\n\n");
                return 0;
            }
            printBoard();
            return 1;
        }
        else if (board[s_y][s_x].color == ' ') {
            printf("\n�ش� ĭ�� ��ĭ�Դϴ�.\n\n");
            return 0;
        }
        else {
            printf("\n���� �Ͽ� ������ �� ���� ���Դϴ�.\n\n");
            return 0;
        }
    }
    else {
        printf("\n������ ������ �ƴմϴ�.\n\n");
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
                    printf("\n%s�� ĳ����!\n", board[s_y][s_x].color == 'B' ? "���" : "���");
                }
                else if (a_x - s_x == -2) {
                    board[s_y][s_x - 1] = board[s_y][0];
                    board[s_y][0] = (Piece){' ', ' '};
                    board[s_y][s_x - 1].moving_count = 1;
                    board[s_y][0].moving_count = 0;
                    printf("\n%s�� ĳ����!\n", board[s_y][s_x].color == 'B' ? "���" : "���");
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
            printf("\n�⹰�� �̵��߽��ϴ�!\n\n");
            if (board[a_y][a_x].type == 'P' && abs(a_y - s_y) == 2) {
                board[a_y][a_x].in_passing_check = 1;
            }
            promote_pawn(a_y, a_x);
            return 1;
        }
        else {
            printf("\n�ش� �⹰�� �� �� ���� ���Դϴ�.\n\n");
        }
    }
    else {
         printf("\n������ ������ �ƴմϴ�.\n\n");
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
        printf("\nüũ����Ʈ!\n%s�� �¸��Ͽ����ϴ�! \n\n", color == 'B' ? "���" : "���");
    }
    else {
        printf("\n%s�� ������ �� �ִ� �⹰�� �����ϴ�.\n�����ϸ���Ʈ, ���º�! \n\n", color == 'B' ? "���" : "���");
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
        printf("\nüũ����Ʈ�� �Ͼ �� �����ϴ�.\n���º�!\n\n");
    }
}
