#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "piece.h"
#include "move.h"

#define noir 0
#define blanc 1

struct Board{
    Piece* pos[65];
    const char ALPHABET[8] = {'A','B','C','D','E','F','G','H'};
    const char ALPHABET_LOWER[8] = {'a','b','c','d','e','f','g','h'};
    const char NUMBERS_CHAR[8] = {'1','2','3','4','5','6','7','8'};
    const char PIECES_CHAR[8] = {'q','r','b','n'};
    int turn = 1;
    char en_passant_target[2] = {'-','-'};

    Board();
    //Board(const Board& other);
    ~Board();
    void move(int case_dep_int, int case_ari_int);
    void move(Move my_move);
    void deplacement(Move my_move);
    int get_index(char c);
    char majuscule(char c);
    int char_case_to_index(char case_1[2]);
    void int_to_case(int case_num, char case_finale[2]);
    bool is_pseudo_legal(Move my_move);
    bool is_attacked(int ma_case=-1);
    void coup_legaux(Move** liste_coups);
    void coup_legaux(int case_dep_int,Move** liste_coups, int i_liste=0);
    bool is_on_board(int case_dep_int, int vec[2]);
    bool is_legal(Move my_move);
    bool is_game_over();
    bool is_stalemate(int case_roi);
    bool is_checkmate(int case_roi);
    bool is_insufficient();
    friend void operator<<(std::ostream &o, Board const &board);
};

#endif