#include <stdio.h>
#include <stdlib.h>
#include "src/board.h"

int main(){
    U64 bitboard = 0ULL;
   
    /*
    for(int rank = 8; rank > 0; rank--){
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;

            printf("\"%c%d\",  ", 'a'+ file, rank);
        }
        printf("\n");
    }
    */

    char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    print_bitboard(bitboard); 

    Board *board = malloc(sizeof(Board));
    fen_parser(board, fen);

    print_bitboard(board->occupancies[BOTH]);
    print_bitboard(board->occupancies[WHITE]);
    print_bitboard(board->occupancies[BLACK]);
    print_bitboard(board->pieces[0]);

    free(board);

    return 0;
}
