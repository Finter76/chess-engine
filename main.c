#include <stdio.h>
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

    print_bitboard(bitboard); 

    return 0;
}
