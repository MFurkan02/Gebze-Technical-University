#ifndef CHESS_H
#define CHESS_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>


#define ROWS 8
#define COLUMNS 8

using namespace std;

// Enum to represent piece types
enum class PieceType {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY};

// Enum to represent player colors
enum class Color { EMPTY,WHITE, BLACK };

class Pieces {

    public:

    // getter functions
    PieceType getType() const {
        return type;
    }

    Color getColor() const {
        return color;
    }

    // setter functions
    void setType(PieceType newType) {
        type = newType;
    }

    void setColor(Color newColor) {
        color = newColor;
    }

    private:
        PieceType type;
        Color color;

       
    
};

class Board {
   
    public:

        Board() : chess_board(8, vector<Pieces> (8) ) {
            create_board();
        }

        void create_board ();

        void save_to_file_board () const;

        void read_from_file_board();

        void score();

        double get_white_score() const ;

        double get_black_score() const ;

        void next_move(int turn); 

        void move_piece(string move,int turn);

        void play(); 

        void print_board() const ; 

        bool check_danger(int row,int column,int turn) const ; 

        bool checkmate(int turn) const ;


    private:
        vector < vector<Pieces> > chess_board; // 2D vector in which the pieces are kept.
        double white_score=0;
        double black_score=0;
};


#endif 