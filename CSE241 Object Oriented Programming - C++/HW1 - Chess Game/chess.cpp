#include "chess.h"

double Board::get_black_score() const { // function that returns the black score.
    return black_score;
}

double Board::get_white_score() const { // function that returns the white score.
    return white_score;
}

bool Board::checkmate(int turn) const{ // function that prints checkmate and prints the winner if checkmate.
    auto check=false;
    if(turn==1){ // if white turn
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(chess_board[i][j].getType()==PieceType::KING && chess_board[i][j].getColor()==Color::WHITE){ // if the white king is found
                    
                    if(check_danger(i,j,1)==true){ // if the king is in danger check message will be printed!
                        cout << "Check! White be careful!" << endl;
                    }
                    // all 8 possible moves of king will be checked if dangerous if all of them are dangerous CHECKMATE black will win. 
                    if(check_danger(i,j,1) && check_danger(i+1,j,1) && check_danger(i,j+1,1) && check_danger(i-1,j,1) && check_danger(i,j-1,1) &&
                        check_danger(i+1,j+1,1) && check_danger(i+1,j-1,1) && check_danger(i-1,j+1,1) && check_danger(i-1,j-1,1)     ){
                            cout << "CHECKMATE! BLACK WINS!" << endl;
                            check=true;
                    }
                }
            }
        }
    }
    else if(turn==0){ // if black turn
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(chess_board[i][j].getType()==PieceType::KING && chess_board[i][j].getColor()==Color::BLACK){ // if the black king is found
                    if(check_danger(i,j,0)==true){ // if the king is in danger check message will be printed!
                        cout << "Check! Black be careful!" << endl;
                    }
                    if(check_danger(i,j,0) && check_danger(i+1,j,0) && check_danger(i,j+1,0) && check_danger(i-1,j,0) && check_danger(i,j-1,0) &&
                        check_danger(i+1,j+1,0) && check_danger(i+1,j-1,0) && check_danger(i-1,j+1,0) && check_danger(i-1,j-1,0)     ){
                            cout << "CHECKMATE! WHITE WINS!" << endl;
                            check=true; // all 8 possible moves of king will be checked if dangerous if all of them are dangerous CHECKMATE black will win. 
                    }
                }
            }
        }
    }
    return check;
}

bool Board::check_danger(int row,int column,int turn) const { // function to check if the piece is in danger or not.Return 1 if in danger.
    auto danger=false;
    if(turn==0){ // if its the turn of the black
        // checking the border with row-1 > -1 && column -1 > -1 && row + 1 < 8 && column + 1 < 8 etc.
        // PAWN DANGER
        // if there is a white pawn crosswise of that piece it is in danger.
        if( row-1>-1 && column-1>-1  && chess_board[row-1][column-1].getType()==PieceType::PAWN && chess_board[row-1][column-1].getColor()==Color::WHITE) 
            danger=true;
        // if there is a white pawn crosswise of that piece it is in danger.
        else if( row-1 > -1 && column + 1 < 8 && chess_board[row-1][column+1].getType()==PieceType::PAWN && chess_board[row-1][column+1].getColor()==Color::WHITE) 
            danger=true; 


        // KNIGHT DANGER 
        // if there is a knight 'L' move close to that piece it is in danger.
        else if( row-2>-1 && column-1 > -1 && chess_board[row-2][column-1].getType()==PieceType::KNIGHT && chess_board[row-2][column-1].getColor()==Color::WHITE)
            danger=true;
        else if(  row-2 > -1 && column+1 < 8 && chess_board[row-2][column+1].getType()==PieceType::KNIGHT && chess_board[row-2][column+1].getColor()==Color::WHITE)
            danger=true;
        else if( row+2 < 8 && column-1 > -1 && chess_board[row+2][column-1].getType()==PieceType::KNIGHT && chess_board[row+2][column-1].getColor()==Color::WHITE)
            danger=true;
        else if( row+2 < 8 && column+1 < 8 && chess_board[row+2][column+1].getType()==PieceType::KNIGHT && chess_board[row+2][column+1].getColor()==Color::WHITE)
            danger=true;


        // ROOK DANGER
        decltype(turn) valid=1;
        // for upper row search for rook
        for(int i=row+1 ;i<8;i++){
            if(chess_board[i][column].getType()!=PieceType::EMPTY) // if there is no empty piece between the current piece and rook then it is not valid(not in danger).
                valid=0;
            if(valid==1 && (chess_board[i][column].getType()==PieceType::ROOK || chess_board[i][column].getType()==PieceType::QUEEN ) && chess_board[i][column].getColor()==Color::WHITE){ // if there is empty piece between the current piece and rook then it is valid( in danger).
                danger=true;
                break; // then breaking the loop if a valid rook is encountered.
            }
        }
        // for down row
        valid=1;
        for(int i=row-1 ;i>-1;i--){
            if(chess_board[i][column].getType()!=PieceType::EMPTY) // if there is no empty piece between the current piece and rook then it is not valid(not in danger).
                valid=0;
            if(valid==1 && (chess_board[i][column].getType()==PieceType::ROOK || chess_board[i][column].getType()==PieceType::QUEEN ) && chess_board[i][column].getColor()==Color::WHITE ){ // if there is empty piece between the current piece and rook then it is valid( in danger).
                danger=true;
                break; // then breaking the loop if a valid rook is encountered.
            }
        }
        valid=1;
        // for upper column
        for(int i=column+1;i<8;i++){
            if(chess_board[row][i].getType()!=PieceType::EMPTY) // if there is no empty piece between the current piece and rook then it is not valid(not in danger).
                valid=0;
            if(valid==1 && (chess_board[row][i].getType()==PieceType::ROOK || chess_board[row][i].getType()==PieceType::QUEEN ) && chess_board[i][column].getColor()==Color::WHITE){// if there is empty piece between the current piece and rook then it is valid( in danger).
                danger=true;
                break; // then breaking the loop if a valid rook is encountered.
            }
        }
        valid=1;
        // for down column
        for(int i=column-1 ;i>-1;i--){
            if(chess_board[row][i].getType()!=PieceType::EMPTY) // if there is no empty piece between the current piece and rook then it is not valid(not in danger).
                valid=0;
            if(valid==1 && (chess_board[row][i].getType()==PieceType::ROOK || chess_board[row][i].getType()==PieceType::QUEEN ) && chess_board[i][column].getColor()==Color::WHITE) // if there is empty piece between the current piece and rook then it is valid( in danger).
                danger=true; // then breaking the loop if a valid rook is encountered.
        }

        // BISHOP DANGER 
        int bivalid=1; // valid variables for all the crosswise of a piece to check if valid.
        int bivalid2=1;
        int bivalid3=1;
        int bivalid4=1;
        for(int i=1;i<8;i++){
            if( row+i>7 || row-i<0 || column+i>7 || column-i<0 ){ 
                break; // if row or column is out of the board break 
            }
            if(chess_board[row+i][column+i].getType()!=PieceType::EMPTY ) // right upper cross not empty
                bivalid=0;
            if(bivalid==1 && (chess_board[row+i][column+i].getType()==PieceType::BISHOP || chess_board[row+i][column+i].getType()==PieceType::QUEEN ) && chess_board[row+i][column+i].getColor()==Color::WHITE){ // if empty and bishop in danger
                danger=true;
                break; // then breaking the loop if a valid bishop is encountered
            }
            if(chess_board[row-i][column-i].getType()!=PieceType::EMPTY ) // left down cross not empty
                bivalid2=0;
            if(bivalid2==1 && (chess_board[row-i][column-i].getType()==PieceType::BISHOP || chess_board[row-i][column-i].getType()==PieceType::QUEEN ) && chess_board[row-i][column-i].getColor()==Color::WHITE){ // if empty and bishop in danger
                danger=true;
                break; // then breaking the loop if a valid bishop is encountered
            }

            if(chess_board[row+i][column-i].getType()!=PieceType::EMPTY ) // left down cross not empty
                bivalid3=0;
            if(bivalid3==1 && (chess_board[row+i][column-i].getType()==PieceType::BISHOP || chess_board[row+i][column-i].getType()==PieceType::QUEEN ) && chess_board[row+i][column-i].getColor()==Color::WHITE){ // if empty and bishop in danger
                danger=true;
                break; // then breaking the loop if a valid bishop is encountered
            }
            if(chess_board[row-i][column+i].getType()!=PieceType::EMPTY ) // left down cross not empty
                bivalid4=0;
            if(bivalid4==1 && (chess_board[row-i][column+i].getType()==PieceType::BISHOP || chess_board[row-i][column+i].getType()==PieceType::QUEEN ) && chess_board[row-i][column+i].getColor()==Color::WHITE){ // if empty and bishop in danger
                danger=true;
                break; // then breaking the loop if a valid bishop is encountered
            }
        }
    }


    else if(turn==1) { // if its the turn of the white
        // PAWN DANGER
        
        // if there is a black pawn crosswise of that piece it is in danger.
        if(row+1 < 8 && column+1 < 8 && chess_board[row+1][column+1].getType()==PieceType::PAWN && chess_board[row+1][column+1].getColor()==Color::BLACK) 
            danger=true;
        // if there is a black pawn crosswise of that piece it is in danger.
        else if( row+1 < 8 && column-1 > -1 && chess_board[row+1][column-1].getType()==PieceType::PAWN && chess_board[row+1][column-1].getColor()==Color::BLACK) 
            danger=true; 
        
        // KNIGHT DANGER 
        // if there is a knight 'L' move close to that piece it is in danger.
        else if(row-2>-1 && column-1 > -1 && chess_board[row-2][column-1].getType()==PieceType::KNIGHT && chess_board[row-2][column-1].getColor()==Color::BLACK)
            danger=true;
        else if( row-2 > -1 && column+1 < 8 && chess_board[row-2][column+1].getType()==PieceType::KNIGHT && chess_board[row-2][column+1].getColor()==Color::BLACK)
            danger=true;
        else if(  row+2 < 8 && column-1 > -1 && chess_board[row+2][column-1].getType()==PieceType::KNIGHT && chess_board[row+2][column-1].getColor()==Color::BLACK)
            danger=true;
        else if(row+2 < 8 && column+1 < 8 && chess_board[row+2][column+1].getType()==PieceType::KNIGHT && chess_board[row+2][column+1].getColor()==Color::BLACK)
            danger=true;

        // ROOK AND QUEEN DANGER
        int valid=1;
        // for upper row
        for(int i=row+1 ;i<8;i++){
            if(chess_board[i][column].getType()!=PieceType::EMPTY) // if there is no empty piece between the current piece and rook then it is not valid(not in danger).
                valid=0;
            if(valid==1 && (chess_board[i][column].getType()==PieceType::ROOK || chess_board[i][column].getType()==PieceType::QUEEN ) && chess_board[i][column].getColor()==Color::BLACK){ // if there is empty piece between the current piece and rook then it is valid( in danger).
                danger=true;
                break; // then breaking the loop if a valid rook is encountered
            }
        }
        // for down row
        for(int i=row-1 ;i>-1;i--){
            if(chess_board[i][column].getType()!=PieceType::EMPTY) // if there is no empty piece between the current piece and rook then it is not valid(not in danger).
                valid=0;
            if(valid==1 && (chess_board[i][column].getType()==PieceType::ROOK || chess_board[i][column].getType()==PieceType::QUEEN ) && chess_board[i][column].getColor()==Color::BLACK ){ // if there is empty piece between the current piece and rook then it is valid( in danger).
                danger=true;
                break; // then breaking the loop if a valid rook is encountered
            }
        }
        // for upper column
        for(int i=column +1 ;i<8;i++){
            if(chess_board[row][i].getType()!=PieceType::EMPTY) // if there is no empty piece between the current piece and rook then it is not valid(not in danger).
                valid=0;
            if(valid==1 && (chess_board[row][i].getType()==PieceType::ROOK || chess_board[row][i].getType()==PieceType::QUEEN ) && chess_board[i][column].getColor()==Color::BLACK){// if there is empty piece between the current piece and rook then it is valid( in danger).
                danger=true;
                break; // then breaking the loop if a valid rook is encountered
            }
        }
        // for down column
        for(int i=column-1 ;i>-1;i--){
            if(chess_board[row][i].getType()!=PieceType::EMPTY) // if there is no empty piece between the current piece and rook then it is not valid(not in danger).
                valid=0;
            if(valid==1 && (chess_board[row][i].getType()==PieceType::ROOK || chess_board[row][i].getType()==PieceType::QUEEN ) && chess_board[i][column].getColor()==Color::BLACK) {// if there is empty piece between the current piece and rook then it is valid( in danger).
                danger=true;
                break; // then breaking the loop if a valid rook is encountered
            }
        }

        // BISHOP AND QUEEN DANGER 
        int bivalid=1;
        int bivalid2=1;
        int bivalid3=1;
        int bivalid4=1;
        for(int i=1;i<8;i++){
            if( row+i>7 || row-i<0 || column+i>7 || column-i<0 ){ // if it is out of the board break
                break;
            }
            if(chess_board[row+i][column+i].getType()!=PieceType::EMPTY ) // right upper cross not empty
                bivalid=0;
            if(bivalid==1 && (chess_board[row+i][column+i].getType()==PieceType::BISHOP || chess_board[row+i][column+i].getType()==PieceType::QUEEN )&& chess_board[row+i][column+i].getColor()==Color::BLACK){// if empty and bishop in danger
                danger=true;
                break; // then breaking the loop if a valid bishop is encountered
            }
            if(chess_board[row-i][column-i].getType()!=PieceType::EMPTY ) // left down cross not empty
                bivalid2=0;
            if(bivalid2==1 && (chess_board[row-i][column-i].getType()==PieceType::BISHOP || chess_board[row-i][column-i].getType()==PieceType::QUEEN )&& chess_board[row-i][column-i].getColor()==Color::BLACK){ // if empty and bishop in danger
                danger=true;
                break; // then breaking the loop if a valid bishop is encountered
            }
            if(chess_board[row+i][column-i].getType()!=PieceType::EMPTY ) // left down cross not empty
                bivalid3=0;
            if(bivalid3==1 && ( chess_board[row+i][column-i].getType()==PieceType::BISHOP ||  chess_board[row+i][column-i].getType()==PieceType::QUEEN ) && chess_board[row+i][column-i].getColor()==Color::BLACK){ // if empty and bishop in danger
                danger=true;
                break; // then breaking the loop if a valid bishop is encountered
            }
            if(chess_board[row-i][column+i].getType()!=PieceType::EMPTY ) // left down cross not empty
                bivalid4=0;
            if(bivalid4==1 && (chess_board[row-i][column+i].getType()==PieceType::BISHOP && chess_board[row-i][column+i].getType()==PieceType::QUEEN )&& chess_board[row-i][column+i].getColor()==Color::BLACK) {// if empty and bishop in danger
                danger=true;
                break; // then breaking the loop if a valid bishop is encountered
            }
        }
    }

    return danger;
}

void Board::play(){

    int turn; // black or white turn variable
    string player_move;  // input string to move piece variable 
    while(true){ // loop until the game ends.

        cout << "[White's Turn]" << endl; 
        cout << "Enter your move: ";
        cin >> player_move ;

        turn=1; // turn of white

        
        
        if(player_move=="end"){ // if player enters 'end' game ends.
            cout << "Game has ended.Saving the final board to the file..." << endl;
            break;
        }
        else if(player_move=="suggest"){ // if player enters 'suggest' a move will be suggested and turn will be passed to other player.
            next_move(turn);
            print_board();
        }
        else if(player_move=="scores"){ // if player enters 'scores' scores of both players will be printed.
            score();
            cout << "Scores : "<< endl << "White : " << white_score << endl << "Black : " << black_score << endl ;
        }
        else{ // otherwise the next move will be printed if valid.
            move_piece(player_move,turn);
            if(checkmate(0)){
                break;
            }
            cout << "[Updated Board]" << endl;
            print_board();
        }

        checkmate(0);

        cout << "[Black's Turn]" << endl; 
        cout << "Enter your move: ";
        cin >> player_move ;
        
        turn=0; // turn of black

        if(player_move=="suggest"){ // if player enters 'suggest' a move will be suggested and turn will be passed to other player.
            next_move(turn);
            print_board();
        }
        
        else if(player_move=="end"){ // if player enters 'end' game ends.
            cout << "Game has ended.Saving the final board to the file..." << endl;
            break;
        }
        else if(player_move=="scores") // if player enters 'scores' scores of both players will be printed.
            cout << "Scores : "<< endl << "White : " << white_score << endl << "Black : " << black_score ;
        
        else{ // otherwise the next move will be printed if valid.
            move_piece(player_move,turn);
            if(checkmate(1)){
                break;
            }
            cout << "[Updated Board]" << endl;
            print_board();
        }

        checkmate(1);

    }
}

 void Board::move_piece(string move,int turn){

    if (move.length()!= 4) { // if input length of move string is more than 4.ERROR!
        cout << "Invalid input to move piece! You lost your turn! " << endl;
        return;
    }
    // converting the string move to integers according to asciis.
    int curr_row = static_cast<int>(move[1]) - 49 ;
    int curr_col = static_cast<int>(move[0]) - 97;

    int next_row = static_cast<int>(move[3]) - 49 ;
    int next_col = static_cast<int>(move[2]) - 97 ;
    
    if(curr_col>8 || curr_col < 0 || curr_row > 8 || curr_row < 0 ){ // if the input is not in the border of the board.ERROR!
        cout << "Invalid input to move piece! You lost your turn! " << endl;
        return;
    }
    Pieces& piece = chess_board[curr_row][curr_col] ;
    Pieces& next_piece = chess_board[next_row][next_col] ;

    if(piece.getType()==PieceType::EMPTY){ // if the piece that is wanted to be moved is an empty piece.ERROR!
        cout << "Invalid piece to move! The piece you want to move is empty('.')! You lost your turn! " << endl;
        return;
    }
    
    if(piece.getColor()==next_piece.getColor()  && next_piece.getType()!=PieceType::EMPTY ){ 
        // if the place you want to move your piece is the same color of your piece.ERROR!                                                                                    
        cout << "Invalid place to move! The place you want to move your piece is the same color piece of yours! You lost your turn! " << endl;
        return;
    }

    

    if(piece.getType()==PieceType::ROOK || piece.getType()==PieceType::QUEEN) { // since the move of rook and queen is the same.
        int valid=0;
        if(curr_row==next_row || curr_col==next_col)
            valid=1;

        if(curr_row==next_row){ // if curr and next move is on the same row
            if(curr_col < next_col){ // if curr col is less
                for(int i=curr_col+1 ; i < next_col ; i++){ // loop to go through the curr col to next col.
                    if(chess_board[curr_row][i].getType()!=PieceType::EMPTY)
                        valid=0; // if there is a piece between the current column and the next column move will not be valid.
                }
            }
            else{ // if curr col is more
                for(int i=next_col+1 ; i < curr_col ; i++){ // loop to go through the curr col to next col. 
                    if(chess_board[curr_row][i].getType()!=PieceType::EMPTY)
                        valid=0; // if there is a piece between the current column and the next column move will not be valid.
                }
            }
        }
        else if(curr_col==next_col){ // if curr and next move is on the same column
            if(curr_row < next_row){ // if curr row is less
                for(int i=curr_row+1 ; i < next_row ; i++){ // loop to go through the curr col to next row.
                    if(chess_board[i][curr_col].getType()!=PieceType::EMPTY)
                        valid=0; // if there is a piece between the current row and the next column move will not be valid.
                }
            }
            else{ // if curr row is more.
                for(int i=next_row+1 ; i < curr_row ; i++){ // loop to go through the curr col to next row.
                    if(chess_board[i][curr_col].getType()!=PieceType::EMPTY)
                        valid=0; // if there is a piece between the current row and the next column move will not be valid.
                }
            }
        }
        if(valid==0){ // if the move is not valid.ERROR!
            cout << "Invalid input! You lost your turn!" << endl; 
        }
        // if it is valid move the piece according to its type(queen or rook).
        else if(piece.getColor()==Color::WHITE && turn==1 && valid==1 ){
            if(piece.getType()==PieceType::ROOK){
                chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
                chess_board[next_row][next_col].setType(PieceType::ROOK);
                chess_board[next_row][next_col].setColor(Color::WHITE);
            }
            else if(piece.getType()==PieceType::QUEEN ){
                chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
                chess_board[next_row][next_col].setType(PieceType::QUEEN);
                chess_board[next_row][next_col].setColor(Color::WHITE);
            }
        }
        // if it is valid move the piece according to its type(queen or rook).
        else if(piece.getColor()==Color::BLACK && turn==0 && valid==1 ){
            if(piece.getType()==PieceType::ROOK){
                chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
                chess_board[next_row][next_col].setType(PieceType::ROOK);
                chess_board[next_row][next_col].setColor(Color::BLACK);
            }
            else if(piece.getType()==PieceType::QUEEN ){
                chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
                chess_board[next_row][next_col].setType(PieceType::QUEEN);
                chess_board[next_row][next_col].setColor(Color::BLACK);
            }
        }
    }


        
    if(piece.getType()==PieceType::KNIGHT && piece.getColor()==Color::WHITE && turn==1){ 
        // all the possible 'L' moves that knight could go.(8)
        // row+2 col+1 // row+2 col-1 // row-2 col+1 // row-2 col-1 
        // row+1 col+2 // row+1 col-2 // row-1 col+2 // row-1 col-2 
        if(curr_row+2==next_row && curr_col+1==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::WHITE);
            
        }
        else if(curr_row+2==next_row && curr_col-1==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::WHITE);
        }
        else if(curr_row-2==next_row && curr_col+1==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::WHITE);
        }   
        else if(curr_row-2==next_row && curr_col-1==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::WHITE);
        }

        else if(curr_row+1==next_row && curr_col+2==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::WHITE);
            
        }
        else if(curr_row+1==next_row && curr_col-2==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::WHITE);
        }
        else if(curr_row-1==next_row && curr_col+2==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::WHITE);
        }   
        else if(curr_row-1==next_row && curr_col-2==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::WHITE);
        }
        else{ // if the input is other than 'L' move input will be invalid.ERROR!
            cout << "Invalid input! You lost your turn!" << endl; 
        }
        
    }
    // if the turn is black.Same procedure but for black
    else if(piece.getType()==PieceType::KNIGHT && piece.getColor()==Color::BLACK && turn==0){ 
        // all the possible 'L' moves that knight could go.(8)
        // row+2 col+1 // row+2 col-1 // row-2 col+1 // row-2 col-1 
        // row+1 col+2 // row+1 col-2 // row-1 col+2 // row-1 col-2 
        if(curr_row+2==next_row && curr_col+1==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::BLACK);
            
        }
        else if(curr_row+2==next_row && curr_col-1==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::BLACK);
        }
        else if(curr_row-2==next_row && curr_col+1==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::BLACK);
        }   
        else if(curr_row-2==next_row && curr_col-1==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::BLACK);
        }


        else if(curr_row+1==next_row && curr_col+2==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::BLACK);
            
        }
        else if(curr_row+1==next_row && curr_col-2==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::BLACK);
        }
        else if(curr_row-1==next_row && curr_col+2==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::BLACK);
        }   
        else if(curr_row-1==next_row && curr_col-2==next_col){
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::KNIGHT);
            chess_board[next_row][next_col].setColor(Color::BLACK);
        }
        else{ // if the input is other than 'L' move input will be invalid.ERROR!
            cout << "Invalid input! You lost your turn!" << endl; 
        }
    }


    if(piece.getType()==PieceType::BISHOP || piece.getType()==PieceType::QUEEN ){ // since the bishop and queen has the same moves.
        int valid=0;
        int diff=next_row-curr_row;
        if(abs(diff)==abs(next_col-curr_col)){ // to check if the next move crosswise of the current move.
            valid=1;
        }
        // the 4 possibilities if curr row or curr col bigger or less than next row or next col 
        if(next_row > curr_row && next_col > curr_col){
            
            for(int i=1; i < diff ; i++){
                    if(chess_board[curr_row+i][curr_col+i].getType()!=PieceType::EMPTY) // to check if there is a piece between the curr and next move.
                        valid=0;
            }
        }
        else if(next_row < curr_row && next_col < curr_col){
            
            for(int i=1; i < diff ; i++){
                    if(chess_board[curr_row-i][curr_col-i].getType()!=PieceType::EMPTY) // to check if there is a piece between the curr and next move.
                        valid=0;
            }
        }

        else if(next_row > curr_row && next_col < curr_col){
            
            for(int i=1; i < diff ; i++){
                    if(chess_board[curr_row+i][curr_col-i].getType()!=PieceType::EMPTY) // to check if there is a piece between the curr and next move.
                        valid=0;
            }
        }

        else if(next_row < curr_row && next_col > curr_col){
            
            for(int i=1; i < diff ; i++){
                    if(chess_board[curr_row-i][curr_col+i].getType()!=PieceType::EMPTY) // to check if there is a piece between the curr and next move.
                        valid=0;
            }
        }
        if(valid==0){ // if the move is not valid player will lose his/her turn.
            cout << "Invalid input! You lost your turn!" << endl; 
        }
        if(piece.getColor()==Color::WHITE && turn==1 && valid==1 ){ // if the piece is white and the move is valid go there according to the type of the piece(bishop or queen)
            if(piece.getType()==PieceType::BISHOP){
                chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
                chess_board[next_row][next_col].setType(PieceType::BISHOP);
                chess_board[next_row][next_col].setColor(Color::WHITE);
            }
            else if(piece.getType()==PieceType::QUEEN ){
                chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
                chess_board[next_row][next_col].setType(PieceType::QUEEN);
                chess_board[next_row][next_col].setColor(Color::WHITE);
            }
        }
        else if(piece.getColor()==Color::BLACK && turn==0 && valid==1 ){ // if the piece is black and the move is valid go there according to the type of the piece(bishop or queen)
            if(piece.getType()==PieceType::BISHOP){
                chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
                chess_board[next_row][next_col].setType(PieceType::BISHOP);
                chess_board[next_row][next_col].setColor(Color::BLACK);
            }
            else if(piece.getType()==PieceType::QUEEN ){
                chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
                chess_board[next_row][next_col].setType(PieceType::QUEEN);
                chess_board[next_row][next_col].setColor(Color::BLACK);
            }
        }
    }
        


        if(piece.getType()==PieceType::KING){ // the move of king.
            if(turn==1){ // white
                int valid=1;
                if(check_danger(next_row,next_col,turn)){ // if the next move is dangerous king can not move there.
                    valid=0;
                }
                if(valid==0){ // if the move is not valid player will lose his/her turn.ERROR!
                    cout << "Invalid input! You lost your turn!" << endl; 
                }
                else{
                    if( abs(next_col-curr_col)==1 || abs(next_row-curr_row)==1 ){ // if the next move is 1 row or column further then king could move there.
                        piece.setType(PieceType::EMPTY);
                        next_piece.setType(PieceType::KING);
                        next_piece.setColor(Color::WHITE);
                    }
                }
            }
            if(turn==0){ // black
                int valid=1;
                if(check_danger(next_row,next_col,turn)){ // if the next move is dangerous king can not move there.
                    valid=0;
                }
                if(valid==0){ // if the move is not valid player will lose his/her turn.
                    cout << "Invalid input! You lost your turn!" << endl; 
                }
                else{
                    if( abs(next_col-curr_col)==1 || abs(next_row-curr_row)==1 ){ // if the next move is 1 row or column further then king could move there.
                        piece.setType(PieceType::EMPTY);
                        next_piece.setType(PieceType::KING);
                        next_piece.setColor(Color::BLACK);
                    }
                }
            }
        }



    if(piece.getType()==PieceType::PAWN && piece.getColor()==Color::BLACK && turn==0){
        if(curr_row-1==next_row && chess_board[next_row][next_col].getType()==PieceType::EMPTY){ // to move one row but if the next row is empty
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::PAWN);
            chess_board[next_row][next_col].setColor(Color::BLACK);
        }
        else if( curr_row==6  && curr_row-2==next_row ){ // if pawn is on the starting position it can move 2 rows
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::PAWN);
            chess_board[next_row][next_col].setColor(Color::BLACK);
        }
        
        else if( curr_row-1==next_row && curr_col+1==next_col && next_piece.getColor()==Color::WHITE ){ // if the next move is cross and there is a white piece it will eat it. 
            piece.setType(PieceType::EMPTY);
            next_piece.setType(PieceType::PAWN);
            next_piece.setColor(Color::BLACK);
        }
        else if( curr_row-1==next_row && curr_col-1==next_col && next_piece.getColor()==Color::WHITE ){ // if the next move is cross and there is a white piece it will eat it. 
            piece.setType(PieceType::EMPTY);
            next_piece.setType(PieceType::PAWN);
            next_piece.setColor(Color::BLACK);
        }
        else{// if the move is not valid player will lose his/her turn.
                    cout << "Invalid input! You lost your turn!" << endl; 
        }
    }

    else if(piece.getType()==PieceType::PAWN && piece.getColor()==Color::WHITE && turn==1){
        if(curr_row+1==next_row && chess_board[next_row][next_col].getType()==PieceType::EMPTY){ // to move one row but if the next row is empty
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::PAWN);
            chess_board[next_row][next_col].setColor(Color::WHITE);
        }
        else if( curr_row==1  && curr_row+2==next_row ){ // if pawn is on the starting position it can move 2 rows
            chess_board[curr_row][curr_col].setType(PieceType::EMPTY);
            chess_board[next_row][next_col].setType(PieceType::PAWN);
            chess_board[next_row][next_col].setColor(Color::WHITE);
        }
        else if( curr_row+1==next_row && curr_col+1==next_col && next_piece.getColor()==Color::BLACK ){ // if the next move is cross and there is a black piece it will eat it. 
            piece.setType(PieceType::EMPTY);
            next_piece.setType(PieceType::PAWN);
            next_piece.setColor(Color::WHITE);
        }
        else if( curr_row+1==next_row && curr_col-1==next_col && next_piece.getColor()==Color::BLACK ){ // if the next move is cross and there is a black piece it will eat it. 
            piece.setType(PieceType::EMPTY);
            next_piece.setType(PieceType::PAWN);
            next_piece.setColor(Color::WHITE);
        }
        else{// if the move is not valid player will lose his/her turn.
                    cout << "Invalid input! You lost your turn!" << endl; 
        }
    }

}

void Board::next_move(int turn){
    int end=0;
    string suggestion="null"; // string for suggestion while printing
    if(turn==0){ // black turn
        for(int i=0;i<8;i++){
                
            for(int j=0;j<8;j++){
                if(end)
                    break;
                // chess_board[i][j].getType()
                if(chess_board[i][j].getColor()==Color::BLACK){ // if the current piece of the board is black
                    
                    if(chess_board[i][j].getType()==PieceType::PAWN ){ // if the current piece is pawn and it is not in danger
                   
                        if(chess_board[i-1][j].getType()==PieceType::EMPTY && check_danger(i-1,j,0)==0){ // if next row of pawn is empty and not dangerous move there.
                            chess_board[i][j].setType(PieceType::EMPTY);
                            chess_board[i-1][j].setType(PieceType::PAWN);
                            chess_board[i-1][j].setColor(Color::BLACK);

                            suggestion[0]='a' + j; // converting the integers to string for suggestion string.
                            suggestion[1]='1' + i;
                            suggestion[2]='a' + j;
                            suggestion[3]='1' + i-1;

                            cout << "Suggestion is " << suggestion << endl ;
                            end=1;
                        }
                    }
                    else if(chess_board[i][j].getType()==PieceType::QUEEN){
                        if(chess_board[i-1][j].getType()==PieceType::EMPTY && check_danger(i-1,j,0)==0){ // if next row of pawn is empty and not dangerous move there.
                            chess_board[i][j].setType(PieceType::EMPTY);
                            chess_board[i-1][j].setType(PieceType::QUEEN);
                            chess_board[i-1][j].setColor(Color::BLACK);

                            suggestion[0]='a' + j; // converting the integers to string for suggestion string.
                            suggestion[1]='1' + i;
                            suggestion[2]='a' + j;
                            suggestion[3]='1' + i-1;

                            cout << "Suggestion is " << suggestion << endl ;
                            end=1;
                        }
                    }
                    else if(chess_board[i][j].getType()==PieceType::KING){
                        if(chess_board[i-1][j].getType()==PieceType::EMPTY && check_danger(i-1,j,0)==0){ // if next row of pawn is empty and not dangerous move there.
                            chess_board[i][j].setType(PieceType::EMPTY);
                            chess_board[i-1][j].setType(PieceType::KING);
                            chess_board[i-1][j].setColor(Color::BLACK);

                            suggestion[0]='a' + j; // converting the integers to string for suggestion string.
                            suggestion[1]='1' + i;
                            suggestion[2]='a' + j;
                            suggestion[3]='1' + i-1;

                            cout << "Suggestion is " << suggestion << endl ;
                            end=1;
                        }
                    }
                }
            
            }
        }
    }
    else if(turn==1){ // white turn
        for(int i=0;i<8;i++){
            for(int j=0;j<8;j++){
                if(end)
                    break;
                if(chess_board[i][j].getColor()==Color::WHITE){ // if the current piece of the board is white
                    
                    if(chess_board[i][j].getType()==PieceType::PAWN ){ // if the current piece is pawn and it is not in danger
                        
                        if(chess_board[i+1][j].getType()==PieceType::EMPTY && check_danger(i+1,j,1)==0){ // if next row of pawn is empty and not dangerous move there.    
                            chess_board[i][j].setType(PieceType::EMPTY);
                            chess_board[i+1][j].setType(PieceType::PAWN);
                            chess_board[i+1][j].setColor(Color::WHITE);

                            suggestion[0]='a' + j; // converting the integers to string for suggestion string.
                            suggestion[1]='1' + i;
                            suggestion[2]='a' + j;
                            suggestion[3]='1' + i+1;

                            cout << "Suggestion is " << suggestion << endl ;
                            end=1;
                        }
                    }
                    else if(chess_board[i][j].getType()==PieceType::QUEEN){
                        if(chess_board[i+1][j].getType()==PieceType::EMPTY && check_danger(i+1,j,0)==0){ // if next row of pawn is empty and not dangerous move there.
                            chess_board[i][j].setType(PieceType::EMPTY);
                            chess_board[i+1][j].setType(PieceType::QUEEN);
                            chess_board[i+1][j].setColor(Color::WHITE);

                            suggestion[0]='a' + j; // converting the integers to string for suggestion string.
                            suggestion[1]='1' + i;
                            suggestion[2]='a' + j;
                            suggestion[3]='1' + i+1;

                            cout << "Suggestion is " << suggestion << endl ;
                            end=1;
                        }
                    }
                    else if(chess_board[i][j].getType()==PieceType::KING){
                        if(chess_board[i+1][j].getType()==PieceType::EMPTY && check_danger(i+1,j,0)==0){ // if next row of pawn is empty and not dangerous move there.
                            chess_board[i][j].setType(PieceType::EMPTY);
                            chess_board[i+1][j].setType(PieceType::KING);
                            chess_board[i+1][j].setColor(Color::WHITE);

                            suggestion[0]='a' + j; // converting the integers to string for suggestion string.
                            suggestion[1]='1' + i;
                            suggestion[2]='a' + j;
                            suggestion[3]='1' + i+1;

                            cout << "Suggestion is " << suggestion << endl ;
                            end=1;
                        }
                    }
                }
            
            }
        }
    }

}

void Board::score(){ // function to calculate the score of the pieces.
    int turn;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){

            if(chess_board[i][j].getColor()==Color::WHITE) // if the color of the piece is white.
                    turn=1;
            else {// if the color of the piece is black. 
                    turn=0;
            }
            switch(chess_board[i][j].getType()){ // switch case to determine the type of the piece.
                // adding the value of the piece according to its type,color and danger status. 
                case PieceType::BISHOP : // 3
                    if(check_danger(i,j,turn)){
                        if(!turn)
                            black_score+=3/2;
                        else
                            white_score+=3/2;
                    }
                    else{
                        if(!turn)
                            black_score+=3;
                        else
                            white_score+=3;
                    }
                    break;
                case PieceType::KNIGHT : // 3
                    if(check_danger(i,j,turn)){
                        if(!turn)
                            black_score+=3/2;
                        else
                            white_score+=3/2;
                    }
                    else{
                        if(!turn)
                            black_score+=3;
                        else
                            white_score+=3;
                    }
                    break;
                case PieceType::ROOK : // 5 
                    if(check_danger(i,j,turn)){
                        if(!turn)
                            black_score+=5/2;
                        else
                            white_score+=5/2;
                    }
                    else{
                        if(!turn)
                            black_score+=5;
                        else
                            white_score+=5;
                    }
                    break;
                case PieceType::QUEEN : // 9
                    if(check_danger(i,j,turn)){
                        if(!turn)
                            black_score+=9/2;
                        else
                            white_score+=9/2;
                    }
                    else{
                        if(!turn)
                            black_score+=9;
                        else
                            white_score+=9;
                    }
                    break;
                case PieceType::PAWN : // 1 
                    if(check_danger(i,j,turn)){
                        if(!turn)
                            black_score+1/2;
                        else
                            white_score+=1/2;
                    }
                    else{
                        if(!turn)
                            black_score+=1;
                        else
                            white_score+=1;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}



void Board::save_to_file_board () const { // function to save the final board to the file.
    ofstream board_to_file("chess_board.txt");

    if (!board_to_file.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    int i,j;
    for (i = 7; i > -1; --i)
    {
        board_to_file << i << " | "; // first writing the 8 | or 7 | etc. values to file. 
        for (j = 0; j < 8; ++j)
        {
            // writing the character of the piece according to its value and color to file.
            if(chess_board[i][j].getType()==PieceType::BISHOP && chess_board[i][j].getColor()==Color::BLACK )
                board_to_file << " b " ;
            else if(chess_board[i][j].getType()==PieceType::BISHOP && chess_board[i][j].getColor()==Color::WHITE )
                board_to_file << " B " ;
            else if(chess_board[i][j].getType()==PieceType::ROOK && chess_board[i][j].getColor()==Color::WHITE )
                board_to_file << " R " ;
            else if(chess_board[i][j].getType()==PieceType::ROOK && chess_board[i][j].getColor()==Color::BLACK )
                board_to_file << " r " ;
            else if(chess_board[i][j].getType()==PieceType::KNIGHT && chess_board[i][j].getColor()==Color::WHITE )
                board_to_file << " N " ;
            else if(chess_board[i][j].getType()==PieceType::KNIGHT && chess_board[i][j].getColor()==Color::BLACK )
                board_to_file << " n " ;
            else if(chess_board[i][j].getType()==PieceType::KING && chess_board[i][j].getColor()==Color::WHITE )
                board_to_file << " K " ;
            else if(chess_board[i][j].getType()==PieceType::KING && chess_board[i][j].getColor()==Color::BLACK )
                board_to_file << " k " ;
            else if(chess_board[i][j].getType()==PieceType::QUEEN && chess_board[i][j].getColor()==Color::WHITE )
                board_to_file << " Q " ;
            else if(chess_board[i][j].getType()==PieceType::QUEEN && chess_board[i][j].getColor()==Color::BLACK )
                board_to_file << " q " ;
            else if(chess_board[i][j].getType()==PieceType::PAWN && chess_board[i][j].getColor()==Color::WHITE )
                board_to_file << " P " ;
            else if(chess_board[i][j].getType()==PieceType::PAWN && chess_board[i][j].getColor()==Color::BLACK )
                board_to_file << " p " ;
            else if(chess_board[i][j].getType()==PieceType::EMPTY)
                board_to_file << " . " ;
        }
        board_to_file << endl; // ending the line after a row finishes.
    }
    board_to_file << "     -----------------------\n     a  b  c  d  e  f  g  h\n"; // finally writng the column letters to file.
    

    board_to_file.close();

}

void Board::read_from_file_board() {
    // Opening the file for reading
    ifstream board_from_file("chess_board.txt");
    char piece;
    if (!board_from_file.is_open()) {
        cerr << "Error opening file." << endl;
        return;
    }

    // Read the chess board from the file
    for(int i=0 ; i<8 ; ++i){
        // first reading the values like '8 |' before the pieces from the file.   
        board_from_file >> piece;
        board_from_file >> piece;
        for(int j=0; j<8;++j){
            board_from_file >> piece; // reading the current piece from the file.
            // then checking the type and color of the piece and storing that piece to our board.  
            if(piece=='r'){
                chess_board[7-i][j].setType(PieceType::ROOK);
                chess_board[7-i][j].setColor(Color::BLACK);
            }
            else if(piece=='R'){
                chess_board[7-i][j].setType(PieceType::ROOK);
                chess_board[7-i][j].setColor(Color::WHITE);
            }

            else if(piece=='.')
                chess_board[7-i][j].setType(PieceType::EMPTY);

            else if(piece=='p'){
                chess_board[7-i][j].setType(PieceType::PAWN);
                chess_board[7-i][j].setColor(Color::BLACK);
            }
            else if(piece=='P'){
                chess_board[7-i][j].setType(PieceType::PAWN);
                chess_board[7-i][j].setColor(Color::WHITE);
            }

            else if(piece=='n'){
                chess_board[7-i][j].setType(PieceType::KNIGHT);
                chess_board[7-i][j].setColor(Color::BLACK);
            }
            else if(piece=='N'){
                chess_board[7-i][j].setType(PieceType::KNIGHT);
                chess_board[7-i][j].setColor(Color::WHITE);
            }

            else if(piece=='b'){
                chess_board[7-i][j].setType(PieceType::BISHOP);
                chess_board[7-i][j].setColor(Color::BLACK);
            }
            else if(piece=='B'){
                chess_board[7-i][j].setType(PieceType::BISHOP);
                chess_board[7-i][j].setColor(Color::WHITE);
            }

            else if(piece=='k'){
                chess_board[7-i][j].setType(PieceType::KING);
                chess_board[7-i][j].setColor(Color::BLACK);
            }
            else if(piece=='K'){
                chess_board[7-i][j].setType(PieceType::KING);
                chess_board[7-i][j].setColor(Color::WHITE);
            }

            else if(piece=='q'){
                chess_board[7-i][j].setType(PieceType::QUEEN);
                chess_board[7-i][j].setColor(Color::BLACK);
            }
            else if(piece=='Q'){
                chess_board[7-i][j].setType(PieceType::QUEEN);
                chess_board[7-i][j].setColor(Color::WHITE);
            }
        }   
    }

    // Closing the file
    board_from_file.close();
}

void Board::print_board() const { // function to print the boad to terminal. 

    int i,j;
    for (i = 8; i > 0; --i)
    {
        cout << i << " | "; // first printing the values like 8 | , 7 | etc.
        for (j = 1; j < 9; ++j)
            
        {   
            Pieces piece = chess_board[i-1][j-1]; // taking the current piece of the board.
            // then printing the char according to its type and color.(empty '.' or Queen black 'q' or Queen wihte 'Q' etc.)
            if(piece.getType()==PieceType::EMPTY)
                    cout << " . " ;

            if(piece.getColor()==Color::BLACK){
                
                if(piece.getType()==PieceType::ROOK)
                    cout << " r " ;
                if(piece.getType()==PieceType::KNIGHT)
                    cout << " n " ;
                if(piece.getType()==PieceType::BISHOP)
                    cout << " b " ;
                if(piece.getType()==PieceType::QUEEN)
                    cout << " q " ;
                if(piece.getType()==PieceType::KING)
                    cout << " k " ;
                if(piece.getType()==PieceType::PAWN)
                    cout << " p " ;
                
            }

            if(piece.getColor()==Color::WHITE){
                
                if(piece.getType()==PieceType::ROOK)
                    cout << " R " ;
                if(piece.getType()==PieceType::KNIGHT)
                    cout << " N " ;
                if(piece.getType()==PieceType::BISHOP)
                    cout << " B " ;
                if(piece.getType()==PieceType::QUEEN)
                    cout << " Q " ;
                if(piece.getType()==PieceType::KING)
                    cout << " K " ;
                if(piece.getType()==PieceType::PAWN)
                    cout << " P " ;
                
            }
        }
        cout << endl;
    }
    cout << "     -----------------------\n     a  b  c  d  e  f  g  h\n"; // finally printing the column letters to terminal.
}


void Board::create_board() { // function to create the board.
    int i,j;
    for (i = 0; i < 8; ++i)
    {
        for (j = 0; j < 8; ++j)
        {   
            Pieces& piece = chess_board[i][j]; // taking the current piece of the board.
            // creating the board according to normal chess game formation by determining the type of the piece and color of the piece.'r n b k q b n r ...' 
            if(i==7 && (j==0 || j==7) ){ 
                piece.setColor(Color::BLACK);
                piece.setType(PieceType::ROOK);
                //cout << " r ";
            }
            else if(i==7 && (j==1 || j==6) ) {
                piece.setColor(Color::BLACK);
                piece.setType(PieceType::KNIGHT);
                //cout << " n ";
            }
            else if(i==7 && (j==2 || j==5) ){
                piece.setColor(Color::BLACK);
                piece.setType(PieceType::BISHOP);
                //cout << " b ";
            }
            else if(i==7 && j==3){
                piece.setColor(Color::BLACK);
                piece.setType(PieceType::KING);
                //cout << " k ";
               
            }
            else if(i==7 && j==4){
                piece.setColor(Color::BLACK);
                piece.setType(PieceType::QUEEN);
                //cout << " q ";
            }
            else if (i==6){
                piece.setColor(Color::BLACK);
                piece.setType(PieceType::PAWN);
                //cout << " p ";
            }
            else if (i==1){
                piece.setColor(Color::WHITE);
                piece.setType(PieceType::PAWN);
                //cout << " P ";
            }
            else if(i==0 && (j==0 || j==7) ){
                piece.setColor(Color::WHITE);
                piece.setType(PieceType::ROOK);
                // cout << " R " ;
            }
            else if(i==0 && (j==1 || j==6) ){
                piece.setColor(Color::WHITE);
                piece.setType(PieceType::KNIGHT);
                // cout << " N " ;
            }
            else if(i==0 && (j==2 || j==5) ){
                piece.setColor(Color::WHITE);
                piece.setType(PieceType::BISHOP);
                //cout << " B ";
            }
            else if(i==0 && j==3){
                piece.setColor(Color::WHITE);
                piece.setType(PieceType::KING);
                //cout << " K ";
            }
            else if(i==0 && j==4){
                piece.setColor(Color::WHITE);
                piece.setType(PieceType::QUEEN);
                //cout << " Q ";
            }
            else{
                piece.setType(PieceType::EMPTY);
                //cout << " . ";            }
            }
        }
    }
}



int main(){
    Board Chess; // creating the chess board 
    string choice; // string to choose play with txt board or normal board.
    cout << "Welcome to the Chess Game!" << endl ;
    cout << "Do you want to read the board from file or play with standard board?" << endl;
    cout << "Enter 'read' to read the board from file! Other inputs will play with standard board" << endl ;
    cout << "Enter the board style (read or normally) : " ;
    cin >> choice ;
    if(choice=="read"){
        Chess.read_from_file_board();
        Chess.print_board();
        Chess.play();
    }
    else{
        Chess.print_board();
        Chess.play();
        Chess.save_to_file_board();
    } 
}
