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

void init_attack_tables();
void generate_moves();

#endif
