#include <stdio.h>
#include <stdlib.h>
#include "src/board.h"

int main(){
    U64 bitboard = 0ULL;

    char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; 

    print_bitboard(bitboard); 

    Board *board = malloc(sizeof(Board)); 

    fen_parser(board, fen);

    free(board);

    return 0;
}
