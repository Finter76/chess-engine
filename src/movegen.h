#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include <stdint.h>

extern U64 knight_table[64];
extern U64 king_table[64];
extern U64 rook_table[64];
extern U64 bishop_table[64];
extern U64 queen_table[64];
extern U64 pawn_table[2][64];

typedef struct {
    Move moves[218];
    int count;
} MoveList;

void init_moves();
void init_attack_tables();

void generate_moves(Board *board, MoveList *list);

void   make_move(Board *dst, Board *src, Move move);
void unmake_move(Board *dst, Board *src);

int is_in_check(Board *board, int side);

U64 rook_attacks(U64 occupancy, int square);

U64 ray_attacks(U64 occupancy, int square, U64 mask);
#endif
