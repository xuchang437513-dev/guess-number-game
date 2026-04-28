#ifndef GAME_H
#define GAME_H

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

// 常量定义
#define MAX_PLAYERS 10
#define MAX_NAME_LEN 50
#define MAX_HISTORY 100
#define CODE_LENGTH 4

// ANSI 颜色码
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[91m"
#define GREEN   "\033[92m"
#define YELLOW  "\033[93m"
#define BLUE    "\033[94m"
#define MAGENTA "\033[95m"
#define CYAN    "\033[96m"
#define WHITE   "\033[97m"

// 玩家结构体
typedef struct {
    char name[MAX_NAME_LEN];
    int total_games;
    int total_wins;
    int total_attempts;
    int best_attempts;
} Player;

// 游戏记录结构体
typedef struct {
    int secret[CODE_LENGTH];
    int attempts[MAX_HISTORY][CODE_LENGTH];
    int results[MAX_HISTORY][2];
    int attempt_count;
    int won;
} GameRecord;

// 函数声明
void clear_input_buffer(void);
void set_utf8(void);
void print_title(void);
void print_separator(void);
void print_colored_box(const char *title, const char *content);

void generate_secret_number(int *secret);
int validate_input(const char *input, int *numbers);
void evaluate_guess(const int *secret, const int *guess, int *a, int *b);
int compare_players(const void *a, const void *b);

void register_player(Player players[], int *player_count);
Player* login_player(Player players[], int player_count);
void save_players(Player players[], int player_count);
void load_players(Player players[], int *player_count);
void display_ranking(Player players[], int player_count);

void start_game(Player *player);
void display_game_result(Player *player, GameRecord *record);

#endif
