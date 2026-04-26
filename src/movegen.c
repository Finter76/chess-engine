#include "board.h"
#include "movegen.h"

U64 king_table[64];
U64 knight_table[64];
U64 pawn_table[2][64];

void generate_knight_table(int square){
    U64 knight_board = 0ULL;
    set_bit(knight_board, square);
    knight_board = 
        nonoeast(knight_board)   | nonowest(knight_board)  |
        sosoeast(knight_board)   | sosowest(knight_board)  |
        eaeanorth(knight_board)  | eaeasouth(knight_board) |
        wewenorth(knight_board)  | wewesouth(knight_board) ;
    knight_table[square] = knight_board; 
}

void generate_king_table(int square){
    U64 king_board = 0ULL;
    set_bit(king_board, square);
    king_board = 
        north(king_board) | south(king_board) |
        east(king_board)  | west(king_board)  |
        north_east(king_board)  | north_west(king_board) |
        south_east(king_board)  | south_west(king_board) ;
    king_table[square] = king_board; 
}
/* DISMISSED
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
*/
// Hyperbola Quintessence
U64 ray_attacks(U64 occupancy, int square, U64 mask){
    U64 attacks = 0ULL;

    U64 r = 1ULL << square;
    
    U64 o = occupancy & mask;
    U64 o2 = bswap64(o);
    U64 r2 = bswap64(r);
    
    attacks = ((o - 2*r) ^ bswap64(o2 - 2*r2)) & mask;

    return attacks;
}

U64 rook_attacks(U64 occupancy, int square){
    U64 rook_board = 0ULL;   

    int rank = square / 8;
    int file = square % 8; 

    U64 north = square < 63 ? file_masks[file] & (~((1ULL << (square + 1)) - 1)) : 0ULL;
    U64 south = square > 0 ? file_masks[file] & ((1ULL << square) - 1) : 0ULL;
    U64 east = square < 63 ? rank_masks[rank] & (~((1ULL << (square + 1)) - 1)) : 0ULL;
    U64 west = square > 0 ? rank_masks[rank] & ((1ULL << square) - 1) : 0ULL;

    rook_board = 
        ray_attacks(occupancy, square, north) | 
        ray_attacks(occupancy, square, south) |
        ray_attacks(occupancy, square, east) | 
        ray_attacks(occupancy, square, west) ;

    return rook_board;
}

U64 bishop_attacks(U64 occupancy, int square){
    U64 bishop_board = 0ULL;
    
    int rank = square / 8;
    int file = square % 8; 

    U64 ne = square < 63 ? diagonal_masks[square] & (~((1ULL << (square + 1)) - 1)) : 0ULL;
    U64 sw = square > 0 ? diagonal_masks[square] & ((1ULL << square) - 1) : 0ULL;
    U64 nw = square < 63 ? antidiagonal_masks[square] & (~((1ULL << (square + 1)) - 1)) : 0ULL;
    U64 se = square > 0 ? antidiagonal_masks[square] & ((1ULL << square) - 1) : 0ULL;

    bishop_board = 
        ray_attacks(occupancy, square, ne) | 
        ray_attacks(occupancy, square, sw) |
        ray_attacks(occupancy, square, nw) | 
        ray_attacks(occupancy, square, se) ;

    return bishop_board;
}

U64 queen_attacks(U64 occupancy, int square){
    U64 queen_board = 0ULL;

    queen_board = rook_attacks(occupancy, square) | bishop_attacks(occupancy, square);
    return queen_board;
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
    for(int square = 0; square < 64; square++){  
        generate_knight_table(square);
        generate_king_table(square);
        //generate_rook_table(square);
        //generate_bishop_table(square);
        //generate_queen_table(square);
        generate_pawn_table(square);
    }    
}

void init_movelist(MoveList *move){
    //memset(move, 0, sizeof(MoveList));
    move->count = 0;
}

void generate_sliding_moves(Board *board, MoveList *list, int side, int piece_type, U64 (*attack_fn)(U64, int)){
    U64 bitboard = board->pieces[piece_type];

    while(bitboard){
        int square = pop_LS1(bitboard);
        U64 attacks = attack_fn(board->occupancies[BOTH], square);
        attacks &= ~board->occupancies[side]; // removes friend pieces
        while(attacks){
            int target = pop_LS1(attacks);

            // Move Encoding
            Flag flag = get_bit(board->occupancies[!side], target) ? CAPTURE : ;
            Move move = encode_move(flag, 0, square, target);
            list->moves[list->count++] = move; 

            pop_bit(attacks, target);
        }
        pop_bit(bitboard, square); 
    }
}

void generate_static_moves(Board *board, MoveList *list, int side, int piece_type, U64 *attack_table){
    U64 bitboard = board->pieces[piece_type];
    while(bitboard){
        int square = pop_LS1(bitboard);
        U64 attacks = attack_table[square];
        attacks &= ~board->occupancies[side]; // removes friend pieces
        while(attacks){
            int target = pop_LS1(attacks);
            // Move Encoding
            Flag flag = get_bit(board->occupancies[!side], target) ? CAPTURE : QUIET;
            Move move = encode_move(flag, 0, square, target);
            list->moves[list->count++] = move;
            pop_bit(attacks, target);
        }
        pop_bit(bitboard, square);
    }
}

void generate_pawn_moves(Board *board, Movelist *list, int side, U64 *attack_table){
    // Single Push
    U64 single_push = side ? north(board->pieces[P + side * 6]) : south(board->pieces[P + side * 6]);
    U64 double_push = side ? north(single_push & start_rank) : south(single_push & start_rank); 

    single_push &= ~(board->occupancies[BOTH]);

    while(single_push){
        int square = pop_LS1(single_push);
        
        int pos = side ? square + 8 : square - 8;

        Flag flag = side ? get_bit(RANK_1, square) : get_bit(RANK_8, square);

        Move move;
        if(!flag){
            move = encode_move(QUIET, 0, pos, square);
            list->moves[list->count++] = move;
        } else{
            move = encode_move(PROMOTION, PROMO_N, pos, square);
            list->moves[list->count++] = move;
            move = encode_move(PROMOTION, PROMO_B, pos, square);
            list->moves[list->count++] = move;
            move = encode_move(PROMOTION, PROMO_R, pos, square);
            list->moves[list->count++] = move;
            move = encode_move(PROMOTION, PROMO_Q, pos, square);
            list->moves[list->count++] = move;
        }
        pop_bit(single_push, square);
    }
    
    // Double Push
    U64 start_rank = side ? RANK_6 : RANK_3;
    double_push &= ~board->occupancies[BOTH];
    
    while(double_push){
        int square = pop_LS1(double_push);

        int pos = side ? square + 16 : square - 16;

        Move move = encode_move(QUIET, 0, pos, square);
        list->moves[list->count++] = move;
    
        pop_bit(double_push, square);
    }
    
    // Capture
    U64 bitboard = board->pieces[P + side * 6];
    while(bitboard){
        int square = pop_LS1(bitboard);
        U64 capture = pawn_table[side][square] & board->occupancies[!side];
        while(capture){
            int pos = pop_LS1(capture);

            // Move Encoding
            U64 rank = side ? RANK_1 : RANK_8;
            int is_promotion = get_bit(rank, pos);

            Move move;
            if(!is_promotion){
                move = encode_move(CAPTURE, 0, square, pos);
                list->moves[list->count++] = move;
            } else{
                move = encode_move(PROMO_CAPTURE, PROMO_N, square, pos);
                list->moves[list->count++] = move;
                move = encode_move(PROMO_CAPTURE, PROMO_B, square, pos);
                list->moves[list->count++] = move;
                move = encode_move(PROMO_CAPTURE, PROMO_R, square , pos);
                list->moves[list->count++] = move;
                move = encode_move(PROMO_CAPTURE, PROMO_Q, square, pos);
                list->moves[list->count++] = move;
            }
            


            pop_bit(capture, pos);
        }
        pop_bit(bitboard, square);
    }
    // En Passant
    if(board->enpassant != NO_SQUARE){
        U64 bitboard = board->pieces[P + side * 6];
        while(bitboard){
            int square = pop_LS1(bitboard);
            U64 enpassant = pawn_table[side][square] & (1ULL << board->enpassant);
            if(get_bit(enpassant, board->enpassant)){
                int pos = board->enpassant;

                // Move Encoding
                Move move = encode_move(ENPASSANT, 0, square, pos);
                list->moves[list->count++] = move;

            }
            pop_bit(bitboard, square);
        }
    }

    // Promotion
}

void generate_moves(Board *board, MoveList *list){
    int side = board->side;

    generate_sliding_moves(board, list, side, R + side * 6, rook_attacks);
    generate_sliding_moves(board, list, side, B + side * 6, bishop_attacks);
    generate_sliding_moves(board, list, side, Q + side * 6, queen_attacks);

    generate_static_moves(board, list, side, N + side * 6, knight_table);     
    generate_static_moves(board, list, side, K + side * 6, king_table);     

    generate_pawn_moves(board, list, side, P + side * 6, pawn_table[side]);
}

void make_move(Board *dst, Board *src, Move move){
    int from = get_from(move);
    int to = get_to(move);
    Flag flag = get_flag(move);
    int promotion = get_promotion(move);

    int piece = src->piece_on[from];
    int side = src->side;

    if(flag == CAPTURE || flag == PROMO_CAPTURE || piece == P || piece == p)
        dst->halfmove = 0;
    else
        dst->halfmove++;
    
    if(dst->side) dst->fullmove++;
    dst->side = !side;
    dst->enpassant = NO_SQUARE;

    if(flag == QUIET){
        dst->piece_on[from] = EMPTY;
        dst->piece_on[to] = piece;
        
        pop_bit(dst->pieces[piece], from);
        set_bit(dst->pieces[piece], to);
       
        pop_bit(dst->occupancy[side], from);
        set_bit(dst->occupancy[side], to);
    } else if(flag == CAPTURE){
        int captured = src->piece_on[to];

        dst->piece_on[from] = EMPTY;
        dst->piece_on[to] = piece;
        
        pop_bit(dst->pieces[piece], from);
        set_bit(dst->pieces[piece], to);

        pop_bit(dst->pieces[captured], to);
        pop_bit(dst->occupancies[!side], to);
       
        pop_bit(dst->occupancies[side], from);
        set_bit(dst->occupancies[side], to);
    } else if(flag == ENPASSANT){
  
    } else if(flag == CASTLING){
      
    } else if(flag == PROMOTION){

    } else if(flag == PROMO_CAPTURE){

    }
    
    dst->occupancy[BOTH] = dst->occupancy[WHITE] | dst->occupancy[BLACK];
}

void unmake_move(Board *dst, Board *src){
    memcpy(dst, src, sizeof(Board));
}
