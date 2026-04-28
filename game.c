#include "game.h"

// 清除输入缓冲区
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 设置 UTF-8 编码
void set_utf8(void) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
}

// 打印带颜色的标题
void print_title(void) {
    printf("\n");
    printf(BOLD CYAN "  +============================================+\n" RESET);
    printf(BOLD CYAN "  |" RESET);
    printf(BOLD YELLOW "       1A2B 猜 数 字 游 戏                   " RESET);
    printf(BOLD CYAN "|\n" RESET);
    printf(BOLD CYAN "  +============================================+\n" RESET);
    printf("\n");
}

// 打印分隔线
void print_separator(void) {
    printf(BOLD CYAN "  ------------------------------------------------\n" RESET);
}

// 打印菜单项
void print_menu_item(int num, const char *text) {
    printf("    %d. %-40s\n", num, text);
}

// 打印彩色信息框
void print_colored_box(const char *title, const char *content) {
    printf("\n  " BOLD "+-------------------------------------------+\n" RESET);
    printf("  " BOLD "|" RESET "  " CYAN "%s" RESET, title);
    int padding = 35 - (int)strlen(title) - (int)strlen(content);
    for (int i = 0; i < padding; i++) printf(" ");
    printf(BOLD "|\n" RESET);
    printf("  " BOLD "|" RESET "  %s", content);
    for (int i = 0; i < 37 - (int)strlen(content); i++) printf(" ");
    printf(BOLD "|\n" RESET);
    printf("  " BOLD "+-------------------------------------------+\n" RESET);
}

// 生成4位不重复的随机数字
void generate_secret_number(int *secret) {
    int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int used[10] = {0};
    int count = 0;
    
    srand((unsigned int)time(NULL));
    
    while (count < CODE_LENGTH) {
        int idx = rand() % 10;
        if (!used[idx]) {
            secret[count] = numbers[idx];
            used[idx] = 1;
            count++;
        }
    }
}

// 验证输入
int validate_input(const char *input, int *numbers) {
    if ((int)strlen(input) != CODE_LENGTH) return 0;
    
    int used[10] = {0};
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (!isdigit(input[i])) return 0;
        int num = input[i] - '0';
        if (used[num]) return 0;
        numbers[i] = num;
        used[num] = 1;
    }
    return 1;
}

// 评估猜测
void evaluate_guess(const int *secret, const int *guess, int *a, int *b) {
    *a = 0; *b = 0;
    
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            (*a)++;
        } else {
            for (int j = 0; j < CODE_LENGTH; j++) {
                if (guess[i] == secret[j]) { (*b)++; break; }
            }
        }
    }
}

// 排行榜排序
int compare_players(const void *a, const void *b) {
    Player *pa = (Player *)a, *pb = (Player *)b;
    double ra = pa->total_games ? (double)pa->total_wins / pa->total_games : 0;
    double rb = pb->total_games ? (double)pb->total_wins / pb->total_games : 0;
    if (rb != ra) return (rb > ra) ? 1 : -1;
    return pb->total_wins - pa->total_wins;
}

// 保存/加载玩家数据
void save_players(Player players[], int player_count) {
    FILE *fp = fopen("players.dat", "wb");
    if (!fp) return;
    fwrite(&player_count, sizeof(int), 1, fp);
    fwrite(players, sizeof(Player), player_count, fp);
    fclose(fp);
}

void load_players(Player players[], int *player_count) {
    FILE *fp = fopen("players.dat", "rb");
    if (!fp) { *player_count = 0; return; }
    fread(player_count, sizeof(int), 1, fp);
    fread(players, sizeof(Player), *player_count, fp);
    fclose(fp);
}

// 注册玩家
void register_player(Player players[], int *player_count) {
    if (*player_count >= MAX_PLAYERS) {
        printf("  " RED "[X] 玩家数量已达上限！\n" RESET);
        return;
    }
    
    printf("\n  " CYAN "=== 注册新玩家 ===" RESET "\n");
    printf("  请输入用户名: ");
    
    char name[MAX_NAME_LEN];
    scanf("%s", name);
    clear_input_buffer();
    
    for (int i = 0; i < *player_count; i++) {
        if (strcmp(players[i].name, name) == 0) {
            printf("  " RED "[X] 用户名已存在！\n" RESET);
            return;
        }
    }
    
    strcpy(players[*player_count].name, name);
    players[*player_count].total_games = 0;
    players[*player_count].total_wins = 0;
    players[*player_count].total_attempts = 0;
    players[*player_count].best_attempts = 0;
    
    (*player_count)++;
    save_players(players, *player_count);
    
    printf("\n  " GREEN "[OK] 注册成功！\n" RESET);
}

// 登录玩家
Player* login_player(Player players[], int player_count) {
    if (player_count == 0) {
        printf("  " YELLOW "[!] 暂无玩家，请先注册！\n" RESET);
        return NULL;
    }
    
    printf("\n  " CYAN "=== 玩家登录 ===" RESET "\n");
    printf("  请输入用户名: ");
    
    char name[MAX_NAME_LEN];
    scanf("%s", name);
    clear_input_buffer();
    
    for (int i = 0; i < player_count; i++) {
        if (strcmp(players[i].name, name) == 0) {
            printf("\n  " GREEN "[OK] 登录成功！欢迎 %s\n" RESET, players[i].name);
            return &players[i];
        }
    }
    
    printf("  " RED "[X] 用户不存在！\n" RESET);
    return NULL;
}

// 显示排行榜
void display_ranking(Player players[], int player_count) {
    if (player_count == 0) {
        printf("  " YELLOW "[!] 暂无玩家数据！\n" RESET);
        return;
    }
    
    Player sorted[MAX_PLAYERS];
    memcpy(sorted, players, sizeof(Player) * player_count);
    qsort(sorted, player_count, sizeof(Player), compare_players);
    
    printf("\n");
    printf(BOLD CYAN "  +========== 英 雄 榜 ==========+\n" RESET);
    printf(BOLD CYAN "  |" RESET);
    printf("  %-10s %-6s %-6s %-6s", RED "玩家", "总场次", "胜利", "胜率");
    printf(BOLD CYAN "|\n" RESET);
    printf(BOLD CYAN "  +-----------+--------+--------+-------+\n" RESET);
    
    for (int i = 0; i < player_count; i++) {
        double rate = sorted[i].total_games ? (double)sorted[i].total_wins / sorted[i].total_games * 100 : 0;
        printf(BOLD CYAN "  |" RESET " ");
        if (i == 0) printf(YELLOW "[1]");
        else if (i == 1) printf(CYAN "[2]");
        else if (i == 2) printf(MAGENTA "[3]");
        else printf("   ");
        printf(" %-9s ", sorted[i].name);
        printf("%-6d %-6d %-5.1f%% ", sorted[i].total_games, sorted[i].total_wins, rate);
        printf(BOLD CYAN "|\n" RESET);
    }
    printf(BOLD CYAN "  +-----------+--------+--------+-------+\n" RESET);
}

// 开始游戏
void start_game(Player *player) {
    GameRecord record = {0};
    generate_secret_number(record.secret);
    
    printf("\n");
    printf(GREEN "  +========================================+\n" RESET);
    printf(GREEN "  |" RESET BOLD GREEN "           游戏开始！                 " RESET GREEN "|\n" RESET);
    printf(GREEN "  +========================================+\n" RESET);
    printf("  我已经想好了一个 " BOLD CYAN "4位不重复" RESET " 的数字。\n");
    printf("  请输入你的猜测（4位不重复数字，如: 1234）\n");
    printf("  " YELLOW "提示: XA YB = X个位置正确，Y个数字正确但位置错误\n" RESET);
    print_separator();
    
    while (record.attempt_count < MAX_HISTORY) {
        printf("\n  " BOLD "第 %d 次猜测: " RESET, record.attempt_count + 1);
        
        char input[100];
        if (!fgets(input, sizeof(input), stdin)) continue;
        input[strcspn(input, "\n")] = 0;
        
        if (strcmp(input, "quit") == 0 || strcmp(input, "q") == 0) {
            printf("  " YELLOW "[!] 游戏中断。" RESET " 正确答案是: ");
            printf(BOLD RED "%d%d%d%d\n" RESET, record.secret[0], record.secret[1], record.secret[2], record.secret[3]);
            return;
        }
        
        if (!validate_input(input, record.attempts[record.attempt_count])) {
            printf("  " RED "[X] 输入无效！" RESET " 请输入4位不重复的数字。\n");
            continue;
        }
        
        int a, b;
        evaluate_guess(record.secret, record.attempts[record.attempt_count], &a, &b);
        record.results[record.attempt_count][0] = a;
        record.results[record.attempt_count][1] = b;
        
        printf("  你的猜测: " BOLD WHITE);
        for (int i = 0; i < CODE_LENGTH; i++) printf("%d", record.attempts[record.attempt_count][i]);
        printf(RESET "\n");
        
        printf("  结果: ");
        if (a > 0) printf(BOLD GREEN "%dA" RESET, a);
        if (b > 0) printf(BOLD YELLOW "%dB" RESET, b);
        if (a == 0 && b == 0) printf(RED "0A0B" RESET);
        printf("\n");
        
        record.attempt_count++;
        
        if (a == CODE_LENGTH) {
            record.won = 1;
            break;
        }
    }
    
    display_game_result(player, &record);
}

// 显示游戏结果
void display_game_result(Player *player, GameRecord *record) {
    printf("\n");
    
    if (record->won) {
        printf(GREEN "  +========================================+\n" RESET);
        printf(GREEN "  |" RESET);
        printf(BOLD GREEN "      恭喜 %s 猜对了！             " RESET GREEN "|\n" RESET, player->name);
        printf(GREEN "  +========================================+\n" RESET);
        
        printf("  " GREEN "[OK] 用了 %d 次猜中！\n" RESET, record->attempt_count);
        
        player->total_games++;
        player->total_wins++;
        player->total_attempts += record->attempt_count;
        
        if (!player->best_attempts || record->attempt_count < player->best_attempts) {
            player->best_attempts = record->attempt_count;
        }
        
        printf("  " CYAN "[*] 历史最佳: %d 次\n" RESET, player->best_attempts);
        
        printf("\n  ");
        if (record->attempt_count <= 4) printf(BOLD YELLOW "[**] 神仙级别！太厉害了！\n" RESET);
        else if (record->attempt_count <= 6) printf(BOLD GREEN "[**] 非常棒！\n" RESET);
        else if (record->attempt_count <= 8) printf(BOLD CYAN "[**] 不错的成绩！\n" RESET);
        else printf("[**] 继续加油！\n");
        
    } else {
        printf(RED "  +========================================+\n" RESET);
        printf(RED "  |" RESET);
        printf(BOLD RED "         很遗憾，没有猜出来              " RESET RED "|\n" RESET);
        printf(RED "  +========================================+\n" RESET);
        
        printf("  正确答案是: " BOLD GREEN);
        for (int i = 0; i < CODE_LENGTH; i++) printf("%d", record->secret[i]);
        printf(RESET "\n");
        
        player->total_games++;
    }
    
    printf("\n  " CYAN "[*] 已保存您的战绩\n" RESET);
}
