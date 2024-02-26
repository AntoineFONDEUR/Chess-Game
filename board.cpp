#include "board.h"

Board::Board () {
    pos[0] = new Tour(noir);
    pos[1] = new Cavalier(noir);
    pos[2] = new Fou(noir);
    pos[3] = new Dame(noir);
    pos[4] = new Roi(noir);
    pos[5] = new Fou(noir);
    pos[6] = new Cavalier(noir);
    pos[7] = new Tour(noir);
    for (int i=0; i<8; i++){pos[8+i] = new Pion(noir);}
    for (int i=0; i<8; i++){pos[48+i] = new Pion(blanc);}
    for (int i=0; i<32; i++){pos[16+i] = new Vide();}
    pos[56] = new Tour(blanc);
    pos[57] = new Cavalier(blanc);
    pos[58] = new Fou(blanc);
    pos[59] = new Dame(blanc);
    pos[60] = new Roi(blanc);
    pos[61] = new Fou(blanc);
    pos[62] = new Cavalier(blanc);
    pos[63] = new Tour(blanc);

    pos[64] = new Vide();
}

Board::~Board(){
    for (int i=0; i<65; i++){
        delete pos[i];
    }
}

void operator<<(std::ostream &o, Board const &board){
        o << ' ';
        o << ' ';
        for (int i=0; i<8; i++){
            o <<  board.ALPHABET[i];
            o << ' ';
        };
        o << '\n';
        for (int i=0; i<8; i++){
            o << 8-i;
            o << ' ';
            for (int j=0; j<8; j++){
                o << board.pos[i*8+j]->symbole;
                o << ' ';
            }
            o << std::endl;
        }
        o << '\n';
}

void Board::move(int case_dep_int, int case_ari_int){
    delete pos[case_ari_int];
    pos[case_ari_int] = pos[case_dep_int]->clone();
    delete pos[case_dep_int];
    pos[case_dep_int] = new Vide();
}

void Board::move(Move my_move){
    move(my_move.case_initiale_int, my_move.case_finale_int);
}

void Board::deplacement(Move my_move){
    if (my_move.is_pseudo && !my_move.is_reversed){move(my_move.case_finale_int,64);} //suppression & stockage de la case d'arrivée si pseudo_move direct

    move(my_move);
    
    if (my_move.is_promotion){
        if (!my_move.is_reversed){
            int color_promotion = pos[my_move.case_finale_int]->color;
            switch (my_move.promotion_piece){
                case 'R': 
                    delete pos[my_move.case_finale_int];
                    pos[my_move.case_finale_int] = new Tour(color_promotion);
                    break;
                case 'B':
                    delete pos[my_move.case_finale_int];
                    pos[my_move.case_finale_int] = new Fou(color_promotion);
                    break;
                case 'N':
                    delete pos[my_move.case_finale_int];
                    pos[my_move.case_finale_int] = new Cavalier(color_promotion);
                    break;
                default:
                    delete pos[my_move.case_finale_int];
                    pos[my_move.case_finale_int] = new Dame(color_promotion);
                    break;
            }
        }
        else{
            int color_promotion = pos[my_move.case_finale_int]->color;
            delete pos[my_move.case_finale_int];
            pos[my_move.case_finale_int] = new Pion(color_promotion);
            pos[my_move.case_finale_int]->a_bouge=true;
        }
    }

    if (my_move.is_castling){ //déplacement de la tour lors des roques
        if (!my_move.is_castling_queen){move(my_move.case_finale_int+1,my_move.case_finale_int-1);} //roque côté roi
        else {move(my_move.case_finale_int-2,my_move.case_finale_int+1);} //roque côté dame
    }

    if (my_move.is_reversed && !my_move.is_en_passant){
        move(64,my_move.case_initiale_int);
    }

    if (my_move.is_en_passant){
        if (!my_move.is_reversed && my_move.is_pseudo){
            move(my_move.case_voisine_int, 64);
        }
        if (my_move.is_reversed && my_move.is_pseudo){
            move(64, my_move.case_voisine_int);
        }
        if (!my_move.is_pseudo){
            delete pos[my_move.case_voisine_int];
            pos[my_move.case_voisine_int] = new Vide();
        }
    }

    if (!my_move.is_pseudo){
        pos[my_move.case_finale_int]->a_bouge=true;
        if (majuscule(pos[my_move.case_finale_int]->symbole) == 'P' && (my_move.vec_move[1] == 2 || my_move.vec_move[1] == -2)){ //màj case target en passant
            int sens = turn == 1 ? 1 : -1;
            int_to_case(my_move.case_finale_int + 8*sens,en_passant_target);
        }
        else{
            en_passant_target[0] = '-';
            en_passant_target[1] = '-';
        }
        turn = 1-turn;
    }
}

int Board::char_case_to_index(char case_1[2]){
    int coup_1 = case_1[1] - 48;
    return 8*(8-coup_1) + get_index(case_1[0]);
}

void Board::int_to_case(int case_num, char case_finale[2]){
    int x = case_num%8;
    int y = 7-(case_num-x)/8;
    case_finale[0] = ALPHABET_LOWER[x];
    case_finale[1] = NUMBERS_CHAR[y];
}

int Board::get_index(char c){
    char c_maj = majuscule(c);
    for (int i=0; i<8; i++){
        if (c_maj==ALPHABET[i]){
            return i;
        }
    }
    return -1;
}

char Board::majuscule(char c){
    return c >= 91 ? c - 32 : c;
}

bool Board::is_pseudo_legal(Move my_move){
    //Vérifie qu'il n'y a pas de pièce de la même couleur sur la case finale
    if (pos[my_move.case_finale_int]->color == turn){return false;}

    //Vérifie que le champ est libre si le mouvement est diagonal
    if (my_move.vec_move[0] == my_move.vec_move[1] || my_move.vec_move[0] == -my_move.vec_move[1]){
        int dep_abs = my_move.vec_move[0] > 0 ? my_move.vec_move[0] : -my_move.vec_move[0];
        int signe_x = my_move.vec_move[0]/dep_abs;
        int signe_y = my_move.vec_move[1]/dep_abs;
        for (int i=1; i<dep_abs;i++){
            if (pos[my_move.case_initiale_int + signe_x*i + signe_y*i*8]->symbole != ' '){
                return false;
            }
        }
    }

    //Vérifie que le champ est libre si le mouvement est vertical
    else if(my_move.vec_move[0] == 0){ 
        int dep_abs = my_move.vec_move[1] > 0 ? my_move.vec_move[1] : -my_move.vec_move[1];
        int signe_y = my_move.vec_move[1]/dep_abs;
        for (int i=1; i<dep_abs;i++){
            if (pos[my_move.case_initiale_int + signe_y*i*8]->symbole != ' '){return false;}
        }
    }

    //Vérifie que le champ est libre si le mouvement est horizontal
    else if (my_move.vec_move[1] == 0){
        int dep_abs = my_move.vec_move[0] > 0 ? my_move.vec_move[0] : -my_move.vec_move[0];
        int signe_x = my_move.vec_move[0]/dep_abs;
        for (int i=1; i<dep_abs;i++){
            if (pos[my_move.case_initiale_int + signe_x*i]->symbole != ' '){return false;}
        }
    }

    //Gestion des pions (hors promotion)
    if (majuscule(pos[my_move.case_initiale_int]->symbole) == 'P'){
        //Empêche d'avancer de 2 si le pion a déjà bougé
        if (my_move.vec_move[0] == 0 && (my_move.vec_move[1] == 2 || my_move.vec_move[1] == -2) && pos[my_move.case_initiale_int]->a_bouge){return false;}
        //Empêche d'avancer s'il y a une pièce devant
        if (my_move.vec_move[0] == 0 && pos[my_move.case_finale_int]->symbole != ' '){return false;}
        //Si le coup est un en-passant, vérifie sa validité
        if (my_move.is_en_passant){
            int en_passant_target_int = char_case_to_index(en_passant_target);
            if (en_passant_target_int == my_move.case_voisine_int){return false;}
            else {
                return true;
            }
        }
        //Si le coup est une tentative de capture, vérifie sa validité
        if ((my_move.vec_move[0] == 1 || my_move.vec_move[0] == -1) && pos[my_move.case_finale_int]->symbole == ' '){return false;}
    }

    return true;
}

bool Board::is_attacked(int ma_case){
    int my_case=0;

    if (ma_case==-1){//Récupérer la case du roi
        for (int i=0; i<64; i++){
            if (pos[i]->color == turn && majuscule(pos[i]->symbole) == 'K'){
                my_case = i;
                break;
            }
        }
    }
    else{my_case=ma_case;}
    char my_case_char[2];
    int_to_case(my_case, my_case_char);
    //Vérifier que le roi n'est pas mis en échec par une dame, une tour, un fou ou l'autre roi
    int directions[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
    for (int dir=0; dir<8; dir++){
        for (int i=1; i<8; i++){
            int dep[2]={directions[dir][0]*i,directions[dir][1]*i};
            if (!is_on_board(my_case, dep)){continue;}
            int case_finale = my_case + dep[0] + dep[1]*8;
            if (pos[case_finale]->symbole == ' '){continue;}
            if (pos[case_finale]->color == turn){break;}
            if (dep[0] == dep[1] || dep[0] == -dep[1]){
                if (majuscule(pos[case_finale]->symbole) == 'B' || majuscule(pos[case_finale]->symbole) == 'Q'){return true;}
                if (i==1 && majuscule(pos[case_finale]->symbole) == 'K'){return true;}
                if (pos[case_finale]->color == 1-turn){break;}
            }
            if (dep[0] == 0 || dep[1] == 0){
                if (majuscule(pos[case_finale]->symbole) == 'R' || majuscule(pos[case_finale]->symbole) == 'Q'){return true;}
                if (i==1 && majuscule(pos[case_finale]->symbole) == 'K'){return true;}
                if (pos[case_finale]->color == 1-turn){break;}
            }
        }   
    }

    //Vérifier que le roi n'est pas mis en échec par un cavalier
    Cavalier cavalier_temp(turn);
    int** deplacements_cavaliers = cavalier_temp.getDeplacements();
    for (int i=0; i<cavalier_temp.getSize(); i++){
        int case_finale = my_case + deplacements_cavaliers[i][0] + deplacements_cavaliers[i][1]*8;
        if (!is_on_board(my_case, deplacements_cavaliers[i])){continue;}
        if (pos[case_finale]->symbole == ' ' || pos[case_finale]->color == turn){continue;}
        if (majuscule(pos[case_finale]->symbole) == 'N'){
            return true;}
    }

    //Vérifier que le roi n'est pas mis en échec par un pion
    int sens_attaque_pion = turn == blanc ? -1 : 1;
    int dep[2][2]={{-1,sens_attaque_pion},{1,sens_attaque_pion}};
    for (int i=0; i<2; i++){
        if (is_on_board(my_case,dep[i])){
            if (majuscule(pos[my_case + dep[i][0] + dep[i][1]*8]->symbole) == 'P' && pos[my_case + dep[i][0] + dep[i][1]*8]->color == 1-turn){return true;}
        }
    }
    return false;
}

bool Board::is_on_board(int case_dep_int, int vec[2]){
    int dx=vec[0];
    int dy=vec[1];
    int x_final = case_dep_int%8 + dx;//4%8+1=5
    int y_final = (case_dep_int-case_dep_int%8)/8 + dy;//7-()
    return !(x_final > 7 || x_final < 0 || y_final > 7 || y_final < 0);
}

void Board::coup_legaux(int case_dep_int, Move** liste_coups, int i_liste){
    int** deplacements_possibles=pos[case_dep_int]->getDeplacements();
    for (int j=0; j<pos[case_dep_int]->getSize(); j++){
        if (!is_on_board(case_dep_int,deplacements_possibles[j])){continue;}
        Move my_move(*this,case_dep_int,deplacements_possibles[j],true);
        
        if (my_move.is_castling){
            int case_rook_int = my_move.is_castling_queen ? my_move.case_initiale_int-4 : my_move.case_initiale_int + 3;
            if (pos[my_move.case_initiale_int]->a_bouge || pos[case_rook_int]->a_bouge){}
            else{
                if (pos[case_rook_int]->color == 1-turn || majuscule(pos[case_rook_int]->symbole) != 'R'){}
                else{
                    if (my_move.is_castling_queen){
                        if (is_attacked(my_move.case_initiale_int) || is_attacked(my_move.case_initiale_int-1) || is_attacked(my_move.case_initiale_int-2) || is_attacked(my_move.case_initiale_int-3) || is_attacked(my_move.case_initiale_int-4)){}
                        else {
                            if (pos[my_move.case_initiale_int-1]->symbole != ' ' || pos[my_move.case_initiale_int-2]->symbole != ' ' || pos[my_move.case_initiale_int-3]->symbole != ' '){}
                            else{
                                delete[] liste_coups[i_liste];
                                char uci_move_castling[5] = {'e', turn == blanc ? '1' : '8', 'c', turn == blanc ? '1' : '8','-'};
                                liste_coups[i_liste] = new Move(*this, uci_move_castling, false);
                                i_liste ++;
                            }
                        }
                    }
                    if (!my_move.is_castling_queen){
                        if (is_attacked(my_move.case_initiale_int) || is_attacked(my_move.case_initiale_int+1) || is_attacked(my_move.case_initiale_int+2) || is_attacked(my_move.case_initiale_int+3)){}
                        else {
                            if (pos[my_move.case_initiale_int+1]->symbole != ' ' || pos[my_move.case_initiale_int+2]->symbole != ' '){}
                            else{
                                delete[] liste_coups[i_liste];
                                char uci_move_castling[5] = {'e', turn == blanc ? '1' : '8', 'g', turn == blanc ? '1' : '8','-'};
                                liste_coups[i_liste] = new Move(*this, uci_move_castling, false);
                                i_liste ++;
                            }
                        }
                    }
                }
            }
            continue;
        }

        if (is_pseudo_legal(my_move)){
            deplacement(my_move);
            if (is_attacked()){
                my_move.reverse();
                deplacement(my_move);
                my_move.reverse();

            }
            else{
                my_move.reverse();
                deplacement(my_move);
                my_move.reverse();

                if (my_move.is_promotion){
                    for (int j=0; j<4; j++){
                        delete[] liste_coups[i_liste];
                        char uci_move_promotion[5] = {my_move.uci_move[0], my_move.uci_move[1], my_move.uci_move[2], my_move.uci_move[3],majuscule(PIECES_CHAR[j])};
                        liste_coups[i_liste] = new Move(*this, uci_move_promotion, false);
                        i_liste ++;
                    }
                }
                else{
                    delete[] liste_coups[i_liste];
                    liste_coups[i_liste] = new Move(*this, my_move.uci_move, false);
                    i_liste ++;
                }
            }
        }
    }
}

void Board::coup_legaux(Move** liste_coups){
    int i_liste = 0;
    for (int i=0; i<64; i++){
        if (pos[i]->color == turn){coup_legaux(i,liste_coups,i_liste);}
    }
}

bool Board::is_legal(Move my_move){
    int max_lenght = 30;
    Move** liste_coups = new Move*[max_lenght];
    for (int i=0; i<max_lenght; i++){liste_coups[i] = new Move();}
    if (pos[my_move.case_initiale_int]->color == turn && my_move.case_finale_int != my_move.case_initiale_int){coup_legaux(my_move.case_initiale_int,liste_coups,0);}
    else{return false;}
    for (int j=0; j<pos[my_move.case_initiale_int]->getSize(); j++){
        if (my_move == *liste_coups[j]){
            for (int i=0; i<max_lenght; i++){
                delete[] liste_coups[i];
            }
            delete[] liste_coups;
            return true;
        }
    }
    for (int i=0; i<max_lenght; i++){
        delete[] liste_coups[i];
    }
    delete[] liste_coups;
    return false;
}

bool Board::is_insufficient(){
    int nb_nights = 0;
    int nb_bishops = 0;
    int non_king_pieces = 0;
    int is_insufficient_color = 0;
    for (int color=0; color<2; color++){
        for (int i=0; i<64; i++){
            if (pos[i]->color == color){
                char symb = majuscule(pos[i]->symbole);
                if (symb == 'N'){
                    nb_nights ++;
                    non_king_pieces ++;
                }
                else if (symb == 'B'){
                    nb_bishops ++;
                    non_king_pieces++;
                }
                else if (symb == 'Q'){non_king_pieces++;}
                else if (symb == 'P'){non_king_pieces++;}
                else if (symb == 'R'){non_king_pieces++;} 
            }
        }
        if ((non_king_pieces == 1 && (nb_nights == 1 || nb_bishops == 1)) || (non_king_pieces == 0)){is_insufficient_color++;}
    }
    if (is_insufficient_color == 2){return true;}
    else {return false;}
}

bool Board::is_checkmate(int case_roi){
    if (is_attacked(case_roi)){
        return is_stalemate(case_roi);
    }
    return false;
}

bool Board::is_stalemate(int case_roi){
    int max_lenght = 300;
    Move** liste_coups = new Move*[max_lenght];
    for (int i=0; i<max_lenght; i++){liste_coups[i] = new Move();}
    coup_legaux(liste_coups);
    if (liste_coups[0]->default_move){
        for (int i=0; i<max_lenght; i++){
            delete[] liste_coups[i];
        }
        delete[] liste_coups;
        return true;
    }
    else{
        for (int i=0; i<max_lenght; i++){
            delete[] liste_coups[i];
        }
        delete[] liste_coups;
        return false;
    }
}

bool Board::is_game_over(){
    int case_roi = 0;
    for (int i=0; i<64; i++){
        if (pos[i]->color == turn && majuscule(pos[i]->symbole) == 'K'){
            case_roi = i;
            break;
        }
    }
    if (is_checkmate(case_roi)){
        std::string gagnant = 1-turn == blanc ? "blancs" : "noirs";
        std::cout << "Echec et mat, les " << gagnant << " gagnent" << std::endl;
        return true;
    }
    if (is_stalemate(case_roi)){
        std::cout << "Pat, il y a égalité" << std::endl;
        return true;
    }
    if (is_insufficient()){
        std::cout << "Insuffisance materielle" << std::endl;
        return true;
    }
    return false;
    
}