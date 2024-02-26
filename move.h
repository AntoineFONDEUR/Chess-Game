#ifndef MOVE_H
#define MOVE_H

#include <iostream>

#define noir 0
#define blanc 1

struct Board;

struct Move{
    char ALPHABET[8] = {'A','B','C','D','E','F','G','H'};
    char ALPHABET_LOWER[8] = {'a','b','c','d','e','f','g','h'};
    char NUMBERS_CHAR[8] = {'1','2','3','4','5','6','7','8'};
    char uci_move[5];
    int vec_move[2];
    char case_initiale_char[2];
    char case_finale_char[2];
    int case_initiale_int;
    int case_finale_int;
    int x_initial;
    int y_initial;
    int x_final;
    int y_final;
    bool is_promotion = false;
    char promotion_piece = false;
    bool is_en_passant = false;
    int case_voisine_int;
    bool is_castling = false;
    bool is_castling_queen;
    bool is_pseudo;
    bool is_reversed=false;
    bool default_move=false;
    //Board* board;

    Move(){default_move=true;}
    Move(const Board& board, char uci_move[5], bool is_pseudo);
    Move(const Board& board, const int case_initiale_int, int vec_move[2], bool is_pseudo);
    //~Move();

    void reverse();
    bool operator==(const Move& other) const {
        if (!this->is_promotion){
            return (this->uci_move[0] == other.uci_move[0] && this->uci_move[1] == other.uci_move[1] && this->uci_move[2] == other.uci_move[2] && this->uci_move[3] == other.uci_move[3]);
        }
        else{
            return (this->uci_move[0] == other.uci_move[0] && this->uci_move[1] == other.uci_move[1] && this->uci_move[2] == other.uci_move[2] && this->uci_move[3] == other.uci_move[3] && this->uci_move[4] == other.uci_move[4]);
        }
    }

    int get_index(char c, char* my_array, int size);
    int char_case_to_index(char ma_case[2]);
    void index_to_char_case(int case_num, char case_finale[2]);
    char majuscule(char c);
};

#endif