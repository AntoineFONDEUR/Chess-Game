#ifndef PIECE_H
#define PIECE_H

#define noir 0
#define blanc 1

#include <iostream>

struct Piece{
    int color;
    char symbole;
    bool moved=false;
    int** deplacements_default;
    bool a_bouge = false;
    Piece(int col){color = col;}
    //~Piece();
    virtual int getHeight() {return 0;}
    virtual int getWidth() {return 0;}
    virtual int getPixelSize() {return 0;}
    virtual int getSize() const {return 0;}
    virtual Piece* clone(){return nullptr;}
    virtual int** getImageArray() {return deplacements_default;}
    virtual int** getDeplacements() {return deplacements_default;}
};

struct Pion : public Piece{
    static const int size = 4;
    static const int height = 36;
    static const int width = 23;
    static const int pixel_size = 2;
    int** image_array = new int*[height*width];
    int** deplacements = new int*[size];
    Pion(int col);
    ~Pion();
    virtual int getHeight() {return height;}
    virtual int getWidth() {return width;}
    virtual int getPixelSize() {return pixel_size;}
    virtual int getSize() const override {return size;}
    virtual Piece* clone(){return new Pion(*this);}
    virtual int** getImageArray() {return image_array;}
    virtual int** getDeplacements() {return deplacements;}
};

struct Tour : public Piece{
    static const int size = 28;
    static const int height = 49;
    static const int width = 23;
    static const int pixel_size = 2;
    int** image_array = new int*[height*width];
    int** deplacements = new int*[size];
    Tour(int col);
    ~Tour();
    virtual int getHeight() {return height;}
    virtual int getWidth() {return width;}
    virtual int getPixelSize() {return pixel_size;}
    virtual int getSize() const override {return size;}
    virtual Piece* clone(){return new Tour(*this);}
    virtual int** getImageArray() {return image_array;}
    virtual int** getDeplacements() {return deplacements;}
};

struct Cavalier : public Piece{
    static const int size = 28;
    static const int height = 56;
    static const int width = 23;
    static const int pixel_size = 2;
    int** image_array = new int*[height*width];
    int** deplacements = new int*[size];
    Cavalier(int col);
    ~Cavalier();
    virtual int getHeight() {return height;}
    virtual int getWidth() {return width;}
    virtual int getPixelSize() {return pixel_size;}
    virtual int getSize() const override {return size;}
    virtual Piece* clone(){return new Cavalier(*this);}
    virtual int** getImageArray() {return image_array;}
    virtual int** getDeplacements() {return deplacements;}
};

struct Fou : public Piece{
    static const int size = 28;
    static const int height = 61;
    static const int width = 27;
    static const int pixel_size = 2;
    int** image_array = new int*[height*width];
    int** deplacements = new int*[size];
    Fou(int col);
    ~Fou();
    virtual int getHeight() {return height;}
    virtual int getWidth() {return width;}
    virtual int getPixelSize() {return pixel_size;}
    virtual int getSize() const override {return size;}
    virtual Piece* clone(){return new Fou(*this);}
    virtual int** getImageArray() {return image_array;}
    virtual int** getDeplacements() {return deplacements;}
};

struct Roi : public Piece{
    static const int size = 10;
    static const int height = 61;
    static const int width = 27;
    static const int pixel_size = 2;
    int** image_array = new int*[height*width];
    int** deplacements = new int*[size];
    Roi(int col);
    ~Roi();
    virtual int getHeight() {return height;}
    virtual int getWidth() {return width;}
    virtual int getPixelSize() {return pixel_size;}
    virtual int getSize() const override {return size;}
    virtual Piece* clone(){return new Roi(*this);}
    virtual int** getImageArray() {return image_array;}
    virtual int** getDeplacements() {return deplacements;}
};

struct Dame : public Piece{
    static constexpr int size = 56;
    static const int height = 61;
    static const int width = 27;
    static const int pixel_size = 2;
    int** image_array = new int*[height*width];
    int** deplacements = new int*[size];
    Dame(int col);
    ~Dame();
    virtual int getHeight() {return height;}
    virtual int getWidth() {return width;}
    virtual int getPixelSize() {return pixel_size;}
    virtual int getSize() const override {return size;}
    virtual Piece* clone(){return new Dame(*this);}
    virtual int** getImageArray() {return image_array;}
    virtual int** getDeplacements() {return deplacements;}
};

struct Vide : public Piece{
    static const int size = 0;
    int** deplacements = new int*[size];
    Vide() : Piece(2){symbole = ' ';}
    ~Vide(){delete[] deplacements;}
    virtual Piece* clone(){return new Vide(*this);}
    virtual int getSize() const override {return size;}
    virtual int** getDeplacements() {return deplacements;}
};

#endif