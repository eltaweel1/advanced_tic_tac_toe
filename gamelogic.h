#ifndef GAMELOGIC_H
#define GAMELOGIC_H

class GameLogic {
public:
    GameLogic();
    bool makeMove(int row, int col, char symbol);
    bool checkWin(char symbol) const;
    bool checkDraw() const;
    char getCell(int row, int col) const;
    void reset();
    void undoMove(int row, int col);  

private:
    char board[3][3];
    int movesMade; //counter of moves made
};

#endif // GAMELOGIC_H
