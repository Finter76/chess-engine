#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "src/board.h"
#include "src/movegen.h"

/*
void main_loop(){
    Gamestate state;
    memset(state, 0, sizeof(Gamestate));
    init_game(state);

    while(1){
        command = read_from_GUI()

        switch(command){
            case "position":
                state = setup_position(command.fen, command.moves);
                break;
            case "go":
                Move best_move = search(state);
                send_to_GUI("bestmove", best_move);
                break;
            case "quit:
                exit();
        }
    }
}
*/

int main(){
    // LOOP
    // main_loop();

    init_board_masks();
    init_attack_tables();
    

    //char* fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; 

    U64 bitboard = knight_table[e4];
    print_bitboard(bitboard); 

    bitboard = king_table[a1];
    print_bitboard(bitboard);

    bitboard = queen_table[h1];
    print_bitboard(bitboard);
    
    bitboard = pawn_table[WHITE][e4];
    print_bitboard(bitboard);

    bitboard = pawn_table[BLACK][d4];
    print_bitboard(bitboard);
    
    bitboard = rook_table[e5];
    print_bitboard(bitboard);
    /*
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            printf("\"%c%d\", ", 'a'+j, i+1);
        }
        printf("\n");
    }
    */
    /*
    Board *board = malloc(sizeof(Board)); 

    fen_parser(board, fen);

    print_board(board);

    free(board);
    */
    return 0;
}

