#include <board.h>
#include <movegen.h>

void generate_knight_table(int square){
    U64 knight_board = 0ULL;
    set_bit(knight_board, square);
    knight_board = 
        nonoeast(knight_board)   | nonowest(knight_board)  |
        sosoeast(knight_board)   | sosowest(knight_board)  |
        eaeanorth(knight_board)  | eaeasouth(knight_board) |
        wewenorth(knight_board)  | wewesouth(knight_board) ;
    knight_attacks[square] = knight_board; 
}

void generate_king_table(int square){
    U64 king_board = 0ULL;
    set_bit(king_board, square);
    king_board = 
        north(king_board) | south(king_board) |
        east(king_board)  | west(king_board)  |
        north_east(king_board)  | north_west(king_board) |
        south_east(king_board)  | south_west(king_board) ;
    king_attacks[square] = king_board; 
}

void generate_rook_table(int square){
    U64 rook_board = 0ULL;

    U64 rank = rank_masks[square / 8];
    U64 file = file_masks[square % 8];

    rook_board = rank ^ file;
    rook_table[square] = rook_board;
}

void generate_bishop_table(int square){
    U64 bishop_board = 0ULL;

    U64 diag_square = diagonal_masks[square];
    U64 antidiag_square = antidiagonal_masks[square];

    bishop_board = diag_square ^ antidiag_square;
    bishop_table[square] = bishop_board;
}

void generate_queen_table(int square){
    U64 queen_board = 0ULL;

    queen_board = rook_table[square] | bishop_table[square];
    queen_table[square] = queen_board;
}

void generate_pawn_table(int square){
    U64 white_pawn_table = 0ULL;
    U64 black_pawn_table = 0ULL;

    set_bit(white_pawn_table, square);  
    set_bit(black_pawn_table, square);  
        
    pawn_table[WHITE][square] = north_east(white_pawn_table) | north_west(white_pawn_table);
    pawn_table[BLACK][square] = south_east(black_pawn_table) | south_west(black_pawn_table);
    
}

void init_attack_tables(){
    for(int square = 0; square < 64; square++)}  
        generate_knight_table(square);
        generate_king_table(square);
        generate_rook_table(square);
        generate_bishop_table(square);
        generate_queen_table(square);
        generate_pawn_table(square);
}

void generate_moves(){

}

