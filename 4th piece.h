#ifndef PIECE_H
#define PIECE_H

typedef struct {
    char type;  // 'K' (King), 'Q' (Queen), 'R' (Rook), 'B' (Bishop), 'N' (Knight), 'P' (Pawn)
    char color; // 'W' (White), 'B' (Black)
    int warning;
    int arrival_point_count;
    int moving_count;
    int in_passing_check;
} Piece;
