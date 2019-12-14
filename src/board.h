#ifndef BOARD_H
#define BOARD_H

#include "data_type.h"

#define PLAYERS_NUMBER 2
#define PLAYER_NAME_MIN_LENGTH 2
#define PLAYER_NAME_MAX_LENGTH 20

typedef struct player_t Player;
typedef struct pawn_t Pawn;
typedef struct board_t Board;

struct player_t
{
    String *name;
    int score;
};

struct pawn_t
{
    int color;
    size_t x;
    size_t y;
};

struct board_t
{
    size_t rows;
    size_t columns;
    Vector *pawns;
    Vector *players;
};

enum players_e
{
    PLAYER_1,
    PLAYER_2
};

enum colors_e
{
    WHITE,
    BLACK,
    WHITE_QUEEN,
    BLACK_QUEEN
};

Board *init_game();

Player *init_player(const char *);
Vector *init_players();
void print_players(Vector *);

Pawn *init_pawn(int, int, int);
Vector *init_pawns(size_t, size_t);
char get_pawn_color(Pawn*);

Board *init_board();
void print_board(Board *);

#endif