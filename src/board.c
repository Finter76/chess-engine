#include "board.h"
#include <string.h>

// INIT RANK & FILE MASKS ARRAY
void init_board_masks(){
    rank_masks[0] = RANK_1;
    rank_masks[1] = RANK_2;
    rank_masks[2] = RANK_3;
    rank_masks[3] = RANK_4;
    rank_masks[4] = RANK_5;
    rank_masks[5] = RANK_6;
    rank_masks[6] = RANK_7;
    rank_masks[7] = RANK_8;

    file_masks[0] = FILE_A;
    file_masks[1] = FILE_B;
    file_masks[2] = FILE_C;
    file_masks[3] = FILE_D;
    file_masks[4] = FILE_E;
    file_masks[5] = FILE_F;
    file_masks[6] = FILE_G;
    file_masks[7] = FILE_H;

    for(int square = 0; square < 64; square++){
        int rank = square / 8;
        int file = square % 8;
        int diff = rank - file;
        int sum  = rank + file;
        for(int j = 0; j < 64; j++){
            int rank_candidate = j / 8;  
            int file_candidate = j % 8;  
            int diff_candidate = rank_candidate - file_candidate;
            int sum_candidate = rank_candidate + file_candidate;
            if(diff == diff_candidate)
                set_bit(diagonal_masks[square], j);
            if(sum == sum_candidate)
                set_bit(antidiagonal_masks[square], j);
        }
    }
}

void print_bitboard(U64 bitboard){
    printf("\n");
    for(int rank = 7; rank >= 0; rank--){
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;

            if(!file) printf("  %d  ", rank + 1);

            printf("%u ", get_bit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n");
    printf("     a b c d e f g h\n");
    printf("\n");
    printf("    Bitboard: %lu\n", bitboard);
}

void print_board(Board *board){
    if(!board) return;

    const char *piece_chars = "PNBRQKpnbrqk.";

    for(int rank = 0; rank < 8; rank++){
        printf(" %d  ", rank+1);
        for(int file = 0; file < 8; file++){
            int square = rank * 8 + file;
            printf("%c ", piece_chars[board->piece_on[square]]);
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");

}

/* -------------------  FEN PARSER --------------------------
<Piece Placement> <Side to Move> <Castling> <En Passant> <Halfmove Clock> <Fullmove Number>

<Piece Placement> : rank 8->1 (divided by /)
                    written left to right (a -> h)
                    CAPS: white
                    lower: black

<Side to Move> : w white
                 b black

<Castling> : K white short castling
             k black short castling
             Q white long  castling
             q black long  castling
             if not available: -

<En Passant> : target square for en passant: ex. e3
               if not avalable: -

<Halfmove Clock> : counts the number of half-moves (plies) since the last pawn move or capture
                   used for the 50-move draw rule

<Fullmove Number> : the move number in the game, starting from 1
                    increments after black's move

Example: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
---------------------------------------------------------- */
int square_from_coords(char file, char rank){
    return ((rank - '1') * 8 + (file - 'a'));
}

void fen_parser(Board *board, const char *fen){
    // TODO: error handling
    if(!board) return;
    if(!fen) return;

    memset(board, 0, sizeof(Board));
    for(int s = 0; s < 64; s++)
        board->piece_on[s] = EMPTY;

    // Piece Placement
    int rank = 7;
    int file = 0;
   
    int i = 0; 
    char c = 0;
    while((c = fen[i++]) != ' '){
        if(c == '/'){
            rank--;
            file = 0;
            continue;
        } 
        // Consecutives empty squares
        if(c >= '1' && c <= '8'){
            file += c - '0';
            continue;
        }
        // Pieces
        int square = get_square(rank, file);
        switch(c){
            case 'P':
                set_bit(board->pieces[0], square);
                set_bit(board->occupancies[WHITE], square);
                board->piece_on[square] = P;
                break;
            case 'N':
                set_bit(board->pieces[1], square);
                set_bit(board->occupancies[WHITE], square);
                board->piece_on[square] = N;
                break;
            case 'B':
                set_bit(board->pieces[2], square);
                set_bit(board->occupancies[WHITE], square);
                board->piece_on[square] = B;
                break;
            case 'R':
                set_bit(board->pieces[3], square);
                set_bit(board->occupancies[WHITE], square);
                board->piece_on[square] = R;
                break;
            case 'Q':
                set_bit(board->pieces[4], square);
                set_bit(board->occupancies[WHITE], square);
                board->piece_on[square] = Q;
                break;
            case 'K':
                set_bit(board->pieces[5], square);
                set_bit(board->occupancies[WHITE], square);
                board->piece_on[square] = K;
                break;
            case 'p':
                set_bit(board->pieces[6], square);
                set_bit(board->occupancies[BLACK], square);
                board->piece_on[square] = p;
                break;
            case 'n':
                set_bit(board->pieces[7], square);
                set_bit(board->occupancies[BLACK], square);
                board->piece_on[square] = n;
                break;
            case 'b':
                set_bit(board->pieces[8], square);
                set_bit(board->occupancies[BLACK], square);
                board->piece_on[square] = b;
                break;
            case 'r':
                set_bit(board->pieces[9], square);
                set_bit(board->occupancies[BLACK], square);
                board->piece_on[square] = r;
                break;
            case 'q':
                set_bit(board->pieces[10], square);
                set_bit(board->occupancies[BLACK], square);
                board->piece_on[square] = q;
                break;
            case 'k':
                set_bit(board->pieces[11], square);
                set_bit(board->occupancies[BLACK], square);
                board->piece_on[square] = k;
                break;
            default: // Error handling
                return;
                break;

        }
        file++;
    }
    board->occupancies[BOTH] = board->occupancies[WHITE] | board->occupancies[BLACK];
    // Side to Move
    c = fen[i++];
    board->side = (c == 'w') ? WHITE : BLACK;
    // Castling
    if((c = fen[i++]) == '-'){
        board-> castling = 0;
        i++;
    } else {
        while((c = fen[i++]) != ' '){
            switch(c){
                case 'K':
                    board->castling |= WHITE_KINGSIDE;
                    break;
                case 'Q':
                    board->castling |= WHITE_QUEENSIDE;
                    break;
                case 'k':
                    board->castling |= BLACK_KINGSIDE;
                    break;
                case 'q':
                    board->castling |= BLACK_QUEENSIDE;
                    break;
                default: // Error handling
                    return;
                    break;
            }
        }
    }

    // En Passant
    if(fen[i] == '-') {
        board->enpassant = NO_SQUARE;
        i++;
    } else {
        board->enpassant = square_from_coords(fen[i], fen[i+1]);
        i += 2;
    }

    //Halfmove Clock
    int acc = 0;
    while(fen[i] != ' ' && fen[i] != '\0'){
        acc = acc * 10 + (fen[i] - '0');
        i++;
    }
    board->halfmove = acc;
    i++;

    // Fullmove Number
    acc = 0;
    while(fen[i] != ' ' && fen[i] != '\0'){
        acc = acc * 10 + (fen[i] - '0');
        i++;
    }
    board->fullmove = acc;
}
