#include "board.h"
#include <string.h>

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

void fen_parser(Board *board, const char *fen){
    // TODO: error handling
    if(!board) return;
    if(!fen) return;

    memset(board, 0, sizeof(Board));

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
                break;
            case 'N':
                set_bit(board->pieces[1], square);
                set_bit(board->occupancies[WHITE], square);
                break;
            case 'B':
                set_bit(board->pieces[2], square);
                set_bit(board->occupancies[WHITE], square);
                break;
            case 'R':
                set_bit(board->pieces[3], square);
                set_bit(board->occupancies[WHITE], square);
                break;
            case 'Q':
                set_bit(board->pieces[4], square);
                set_bit(board->occupancies[WHITE], square);
                break;
            case 'K':
                set_bit(board->pieces[5], square);
                set_bit(board->occupancies[WHITE], square);
                break;
            case 'p':
                set_bit(board->pieces[6], square);
                set_bit(board->occupancies[BLACK], square);
                break;
            case 'n':
                set_bit(board->pieces[7], square);
                set_bit(board->occupancies[BLACK], square);
                break;
            case 'b':
                set_bit(board->pieces[8], square);
                set_bit(board->occupancies[BLACK], square);
                break;
            case 'r':
                set_bit(board->pieces[9], square);
                set_bit(board->occupancies[BLACK], square);
                break;
            case 'q':
                set_bit(board->pieces[10], square);
                set_bit(board->occupancies[BLACK], square);
                break;
            case 'k':
                set_bit(board->pieces[11], square);
                set_bit(board->occupancies[BLACK], square);
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
    while((c = fen[i++]) != ' '){
        if(c == '-') board-> castling = -1;
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
    // En Passant
    while((c = fen[i++]) != ' '){
        if(c == '-') board-> enpassant = -1;

    }
    // Halfmove Clock
    int acc = 0;
    while(fen[i] != ' ' && fen[i] != '\0'){
        acc = acc * 10 + (fen[i] - '0');
        i++;
    }
    board->halfmove = acc;
    // Fullmove Number
    acc = 0;
    while(fen[i] != ' ' && fen[i] != '\0'){
        acc = acc * 10 + (fen[i] - '0');
        i++;
    }
    board->fullmove = acc;
}
