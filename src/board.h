#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdio.h>

typedef uint64_t U64;

/* BOARD REPRESENTATION
"a8",  "b8",  "c8",  "d8",  "e8",  "f8",  "g8",  "h8",
"a7",  "b7",  "c7",  "d7",  "e7",  "f7",  "g7",  "h7",
"a6",  "b6",  "c6",  "d6",  "e6",  "f6",  "g6",  "h6",
"a5",  "b5",  "c5",  "d5",  "e5",  "f5",  "g5",  "h5",
"a4",  "b4",  "c4",  "d4",  "e4",  "f4",  "g4",  "h4",
"a3",  "b3",  "c3",  "d3",  "e3",  "f3",  "g3",  "h3",
"a2",  "b2",  "c2",  "d2",  "e2",  "f2",  "g2",  "h2",
"a1",  "b1",  "c1",  "d1",  "e1",  "f1",  "g1",  "h1"
*/

enum {
    a8,  b8,  c8,  d8,  e8,  f8,  g8,  h8,
    a7,  b7,  c7,  d7,  e7,  f7,  g7,  h7,
    a6,  b6,  c6,  d6,  e6,  f6,  g6,  h6,
    a5,  b5,  c5,  d5,  e5,  f5,  g5,  h5,
    a4,  b4,  c4,  d4,  e4,  f4,  g4,  h4,
    a3,  b3,  c3,  d3,  e3,  f3,  g3,  h3,
    a2,  b2,  c2,  d2,  e2,  f2,  g2,  h2,
    a1,  b1,  c1,  d1,  e1,  f1,  g1,  h1
};

enum {
    WHITE, BLACK, BOTH
};

// RANKS CONST

#define FILE_A 72340172838076673ULL
#define FILE_B 144680345676153346ULL
#define FILE_C 289360691352306692ULL
#define FILE_D 578721382704613384ULL
#define FILE_E 1157442765409226768ULL
#define FILE_F 2314885530818453536ULL
#define FILE_G 4629771061636907072ULL
#define FILE_H 9259542123273814144ULL

// FILES CONST

#define RANK_1 255ULL
#define RANK_2 65280ULL
#define RANK_3 16711680ULL
#define RANK_4 4278190080ULL
#define RANK_5 1095216660480ULL
#define RANK_6 280375465082880ULL
#define RANK_7 71776119061217280ULL
#define RANK_8 18374686479671623680ULL

// OTHER SQUARE CONST

#define WHITE_SQUARES 6172840429334713770ULL
#define BLACK_SQUARES 12273903644374837845ULL

#define ODD_FILES 12297829382473034410ULL
#define EVEN_FILES 6148914691236517205ULL

// CASTLING BITMASK 
#define WHITE_KINGSIDE  1  // 0001
#define WHITE_QUEENSIDE 2  // 0010
#define BLACK_KINGSIDE  4  // 0100
#define BLACK_QUEENSIDE 8  // 1000


typedef struct {
    U64 pieces[12]; // P, N, B, R, Q, K 0-5 white, 6-11 black
    U64 occupancies[3]; // WHITE, BLACK, BOTH
    int side; // WHITE or BLACK
    int enpassant; // -1 if not available
    int castling; // bitmask
    int halfmove;
    int fullmove;
} Board;


#define get_square(rank, file) ((rank) * 8 + (file))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

void print_bitboard(U64 bitboard);

void fen_parser(Board *board, const char *fen);

#endif
