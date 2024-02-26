#include "move.h"
#include "board.h"

Move::Move(const Board& board, char uci_move[5], bool is_pseudo){

    for(int i=0; i<5; i++){this->uci_move[i]=uci_move[i];}
    
    int coup_1 = uci_move[1];
    int coup_3 = uci_move[3];

    vec_move[0]=get_index(uci_move[2],&ALPHABET_LOWER[0],8)-get_index(uci_move[0],&ALPHABET_LOWER[0],8);
    vec_move[1]=coup_3-coup_1;

    case_initiale_char[0] = this->uci_move[0];
    case_initiale_char[1] = this->uci_move[1];
    case_finale_char[0] = this->uci_move[2];
    case_finale_char[1] = this->uci_move[3];
    case_initiale_int=char_case_to_index(case_initiale_char);
    case_finale_int=char_case_to_index(case_finale_char);
    x_initial = case_initiale_int%8;
    y_initial = (case_initiale_int - x_initial)/8;
    x_final = case_finale_int%8;
    y_final = (case_finale_int - x_final)/8;

    if (majuscule(board.pos[case_initiale_int]->symbole) == 'P'){
        if ((board.turn == blanc && y_initial == 1) || (board.turn == noir && y_initial == 6)){
            is_promotion = true;
            this->uci_move[4] = 'Q';
            promotion_piece = majuscule(uci_move[4]);
        }
        case_voisine_int = case_initiale_int + vec_move[0];
        if ((vec_move[0] == -1 || vec_move[0] == 1) && board.pos[case_finale_int]->symbole == ' ' && majuscule(board.pos[case_voisine_int]->symbole) == 'P' && board.pos[case_voisine_int]->color == 1-board.turn){
            if (board.turn == blanc && y_initial == 3 || board.turn == noir && y_initial == 4){
                is_en_passant = true;
            }
        }
    }

    if (majuscule(board.pos[case_initiale_int]->symbole) == 'K'){
        if (vec_move[0] == 2 || vec_move[0] == -2){
            if (case_initiale_char[0] == 'e' && case_initiale_char[1] == '1' && board.turn == blanc){
                is_castling = true;
                is_castling_queen = case_finale_char[0] == 'c' ? true : false;
            }
            else if (case_initiale_char[0] == 'e' && case_initiale_char[1] == '8' && board.turn == noir){
                is_castling = true;
                is_castling_queen = case_finale_char[0] == 'c' ? true : false;
            }
        }
    }

    this->is_pseudo = is_pseudo;
    this->is_reversed = is_reversed;

}

Move::Move(const Board& board, const int case_initiale_int, int vec_move[2], bool is_pseudo){
    //if move not on board raise error

    this->vec_move[0] = vec_move[0];
    this->vec_move[1] = vec_move[1];
    this->case_initiale_int=case_initiale_int;
    this->case_finale_int=case_initiale_int+vec_move[0]+8*vec_move[1];
    x_initial = case_initiale_int%8;
    y_initial = (case_initiale_int - x_initial)/8;
    x_final = case_finale_int%8;
    y_final = (case_finale_int - x_final)/8;

    index_to_char_case(this->case_initiale_int,case_initiale_char);
    index_to_char_case(this->case_finale_int,case_finale_char);

    this->uci_move[0]=case_initiale_char[0];
    this->uci_move[1]=case_initiale_char[1];
    this->uci_move[2]=case_finale_char[0];
    this->uci_move[3]=case_finale_char[1];

    if (majuscule(board.pos[case_initiale_int]->symbole) == 'P'){
        if ((board.turn == blanc && y_initial == 1) || (board.turn == noir && y_initial == 6)){
            is_promotion = true;
            promotion_piece = majuscule(uci_move[4]);
        }
        case_voisine_int = case_initiale_int + vec_move[0];
        if ((vec_move[0] == -1 || vec_move[0] == 1) && board.pos[case_finale_int]->symbole == ' ' && majuscule(board.pos[case_voisine_int]->symbole) == 'P' && board.pos[case_voisine_int]->color == 1-board.turn){
            if (board.turn == blanc && y_initial == 3 || board.turn == noir && y_initial == 4){
                    is_en_passant = true;
            }
        }
    }

    if (majuscule(board.pos[case_initiale_int]->symbole) == 'K'){
        if (vec_move[0] == 2 || vec_move[0] == -2){
            if (case_initiale_char[0] == 'e' && case_initiale_char[1] == '1' && board.turn == blanc){
                is_castling = true;
                is_castling_queen = case_finale_char[0] == 'c' ? true : false;
            }
            else if (case_initiale_char[0] == 'e' && case_initiale_char[1] == '8' && board.turn == noir){
                is_castling = true;
                is_castling_queen = case_finale_char[0] == 'c' ? true : false;
            }
        }
    }

    this->is_pseudo = is_pseudo;
    this->is_reversed = is_reversed;

}


void Move::reverse(){
    char case_initiale_char_temp[2] = {case_initiale_char[0], case_initiale_char[1]};
    case_initiale_char[0] = case_finale_char[0];
    case_initiale_char[1] = case_finale_char[1];
    case_finale_char[0] = case_initiale_char_temp[0];
    case_finale_char[1] = case_initiale_char_temp[1];
    
    this->uci_move[0]=case_initiale_char[0];
    this->uci_move[1]=case_initiale_char[1];
    this->uci_move[2]=case_finale_char[0];
    this->uci_move[3]=case_finale_char[1];

    case_initiale_int=char_case_to_index(case_initiale_char);
    case_finale_int=char_case_to_index(case_finale_char);

    vec_move[0] *= -1;
    vec_move[1] *= -1;

    int x_initial_temp = x_initial;
    int y_initial_temp = y_initial;
    x_initial = x_final;
    y_initial = y_final;
    x_final = x_initial_temp;
    y_final = y_initial_temp;

    is_reversed = !is_reversed;
} 

int Move::get_index(char c, char* my_array, int size){
    for (int i=0; i<size; i++){
        if (c==my_array[i]){return i;}
    }
    return -1;
}

int Move::char_case_to_index(char ma_case[2]){
    int row = 8 - (ma_case[1] - 48);
    int col = get_index(majuscule(ma_case[0]),&ALPHABET[0],8);
    return 8*row + col;
}

void Move::index_to_char_case(int case_num, char case_finale[2]){
    int x = case_num%8;
    int y = 7-(case_num-x)/8;
    case_finale[0] = ALPHABET_LOWER[x];
    case_finale[1] = NUMBERS_CHAR[y];
}

char Move::majuscule(char c){
    return c >= 91 ? c - 32 : c;
}