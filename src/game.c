#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

int main()
{
    

    Board *board = init_game();

    print_board(board);

    return EXIT_SUCCESS;
}