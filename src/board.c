#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
                int color = (i < ((rows / 2) - 1)) ? BLACK : WHITE;

                if (((j % 2 == 1) && (i % 2 == 0)) || ((j % 2 == 0) && (i % 2 == 1)))
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
        .columns = columns,
        .nb_rounds = 0};

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

int is_valid_move(Board *board, Coordinate *src, Coordinate *dest)
{
    Vector *position_1 = (Vector *)board->pawns->items[src->y];
    
    if (position_1->items[src->x] == NULL)
    {
        return 0;
    }
    
    int direction = (board->nb_rounds % 2 == 0) ? -1 : 1;

    if (dest->y != src->y + direction)
    {
        return 0;
    }

    if ((dest->x != (src->x + 1)) && (dest->x != (src->x - 1)))
    {
        return 0;
    }

    return 1;
}

void move_pawn(Board *board)
{
    Coordinate *start, *dest;
    int valid = 0;
    do
    {
        start = select_pawn(board);
        dest = select_pawn(board);
        valid = is_valid_move(board, start, dest);

        if (!valid)
        {
            fprintf(stderr, "\nThe move is not valid. Try again.\n\n");
        }

    } while (!valid);

    swap_pawns(board->pawns, start, dest);
}

void swap_pawns(Vector *pawns, Coordinate *src, Coordinate *dest)
{
    Vector *s = (Vector *)pawns->items[src->y];
    Vector *d = (Vector *)pawns->items[dest->y];
    void *temp = d->items[dest->x];
    d->items[dest->x] = s->items[src->x];
    s->items[src->x] = temp;
}

Coordinate *select_pawn(Board *board)
{
    char input[10];
    Coordinate *pawn = NULL;
    do
    {
        printf("Enter coordinate: ");
        scanf("%s", input);

        pawn = parse_entry_move(input, board->rows, board->columns);
        if (pawn == NULL)
        {
            fprintf(stderr, "\nPosition not valid. Try again.\n\n");
        }
    }  while (pawn == NULL);

    return pawn;
}

Coordinate *parse_entry_move(const char *move, size_t rows, size_t columns)
{
    size_t size = strlen(move);
    if (size < 2)
    {
        return NULL;
    }

    if (is_letter(move[0]))
    {
        Coordinate *coord = malloc(sizeof(Coordinate));
        coord->x = is_upper_case(move[0]) ? move[0] - 'A' : move[0] - 'a';

        char *number = calloc(sizeof(char), size);
        size--;
        strncpy(number, move + 1, size);

        int temp = int_val(number);
        free(number);

        coord->y = rows - temp;

        return (coord->y < rows && coord->x < columns) ? coord : NULL;
    }

    return NULL;
}

int is_letter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_upper_case(char c)
{
    return (c >= 'A' && c <= 'Z');
}

int int_val(const char *str)
{
    size_t size = strlen(str);
    size_t number = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (isdigit(str[i]))
        {
            number *= 10;
            number += str[i] - '0';
        }
        else
        {
            printf("not a number!\n");
            return -1;
        }
    }

    return number;
}
