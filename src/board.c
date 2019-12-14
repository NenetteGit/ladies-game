#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"

Player *init_player(const char *name)
{
    Player *player = malloc(sizeof(Player));
    *player = (Player){
        .name = string_init_with(name),
        .score = 0};

    return player;
}

Vector *init_players()
{
    Vector *players = vector_init();
    char tmp_player_name[50];

    while (players->length < PLAYERS_NUMBER)
    {
        printf("Enter name for the player %ld: ", (players->length + 1));

        scanf("%s", tmp_player_name);

        size_t len = strlen(tmp_player_name);

        if (len < PLAYER_NAME_MIN_LENGTH || len > PLAYER_NAME_MAX_LENGTH)
        {
            fprintf(stderr, "\nInput size must be between 2 and 20 characters(included).\n\n");
            continue;
        }

        Player *player = init_player(tmp_player_name);
        vector_push_ref(players, player);
    }

    return players;
}

void print_players(Vector *players)
{
    for (size_t i = 0; i < players->length; i++)
    {
        Player *player = (Player *)players->items[i];
        printf("Player %ld ---> name: %s\tscore: %d\n\n", (i + 1), player->name->buffer, player->score);
    }
}

Pawn *init_pawn(int color, int x, int y)
{
    Pawn *pawn = malloc(sizeof(Pawn));
    *pawn = (Pawn){
        .color = color,
        .x = x,
        .y = y};

    return pawn;
}

Vector *init_pawns(size_t rows, size_t columns)
{
    Vector *pawns = vector_init();

    for (size_t i = 0; i < rows; i++)
    {
        Vector *line = vector_init();

        for (size_t j = 0; j < columns; j++)
        {
            if (i < ((rows / 2) - 1) || i > (rows / 2))
            {
                int color = (i < ((rows / 2) - 1)) ? WHITE : BLACK;

                if (((j % 2 == 0) && (i % 2 == 0)) || ((j % 2 == 1) && (i % 2 == 1)))
                {
                    Pawn *pawn = init_pawn(color, i, j);
                    vector_push_ref(line, pawn);
                }
                else
                {
                    vector_push_ref(line, NULL);
                }
            }
            else
            {
                vector_push_ref(line, NULL);
            }
        }
        vector_push_ref(pawns, line);
    }

    return pawns;
}

char get_pawn_color(Pawn *pawn)
{
    char color;
    switch (pawn->color)
    {
    case WHITE:
        color = 'w';
        break;

    case BLACK:
        color = 'b';
        break;

    case WHITE_QUEEN:
        color = 'W';
        break;

    case BLACK_QUEEN:
        color = 'B';
        break;

    default:
        color = 'N';
        break;
    }

    return color;
}

Board *init_board()
{
    size_t rows = 0;
    size_t columns = 0;

    while (rows < 6 || columns < 4 || rows > 100 || columns > 100 || (rows % 2) != 0)
    {
        printf("Enter the number of rows and columns: ");
        scanf("%lu%lu", &rows, &columns);

        if (rows < 6 || columns < 4 || rows > 100 || columns > 100)
        {
            fprintf(stderr, "Minimum of rows: 6\tMaximum of rows: 100\n");
            fprintf(stderr, "Minimum of columns: 4\tMaximum of columns: 100\n\n");
        }
        else if ((rows % 2) != 0)
        {
            fprintf(stderr, "The number of rows must be even\n\n");
        }
    }

    Board *board = malloc(sizeof(Board));
    *board = (Board){
        .rows = rows,
        .columns = columns};

    return board;
}

void print_board(Board *board)
{
    for (size_t i = 0; i < board->rows; i++)
    {
        Vector *row = (Vector *)board->pawns->items[i];
        for (size_t j = 0; j < board->columns; j++)
        {
            Pawn *pawn = (Pawn *)row->items[j];
            if (pawn != NULL)
            {
                char pawn_color = get_pawn_color(pawn);
                printf("%c ", pawn_color);
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }
}

Board *init_game()
{
    Board *board = init_board();
    board->players = init_players();
    board->pawns = init_pawns(board->rows, board->columns);

    return board;
}
