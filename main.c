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
    init_board_masks();
    init_attack_tables();
    
    Board board;
    fen_parser(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    for(int depth = 1; depth <= 5; depth++){
        U64 nodes = perft(&board, depth);
        printf("perft(%d) = %llu\n", depth, nodes);
    }
    
    return 0;
}
