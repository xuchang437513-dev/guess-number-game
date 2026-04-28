#include "game.h"

int main(void) {
    set_utf8();
    
    Player players[MAX_PLAYERS];
    int player_count = 0;
    Player *current_player = NULL;
    
    load_players(players, &player_count);
    
    print_title();
    
    while (1) {
        printf("\n");
        print_separator();
        
        if (!current_player) {
            printf("  " BOLD CYAN "═══ 主菜单 ═══" RESET "\n");
            print_separator();
            printf("    1. 注册新玩家\n");
            printf("    2. 登录\n");
            printf("    3. 查看排行榜\n");
            printf("    0. 退出游戏\n");
        } else {
            printf("  " BOLD CYAN "═══ 主菜单 ═══" RESET "\n");
            printf("  " GREEN "  当前: %s\n" RESET, current_player->name);
            print_separator();
            printf("    1. 开始游戏\n");
            printf("    2. 查看战绩\n");
            printf("    3. 查看排行榜\n");
            printf("    4. 切换玩家\n");
            printf("    0. 退出游戏\n");
        }
        
        print_separator();
        printf("\n  请选择: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("  " RED "✗ 输入无效！\n" RESET);
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 0:
                if (current_player) save_players(players, player_count);
                printf("\n");
                printf(BOLD CYAN "  ╔═══════════════════════════════════════════╗\n" RESET);
                printf(BOLD CYAN "  ║" RESET);
                printf("              感谢游玩，再见！              ");
                printf(BOLD CYAN "║\n" RESET);
                printf(BOLD CYAN "  ╚═══════════════════════════════════════════╝\n" RESET);
                return 0;
                
            case 1:
                if (!current_player) {
                    register_player(players, &player_count);
                } else {
                    start_game(current_player);
                    save_players(players, player_count);
                }
                break;
                
            case 2:
                if (!current_player) {
                    current_player = login_player(players, player_count);
                } else {
                    printf("\n");
                    printf(BOLD CYAN "  ┌─────────── 个人战绩 ───────────┐\n" RESET);
                    printf(BOLD CYAN "  │" RESET);
                    printf("  %-10s: %s", CYAN, current_player->name);
                    for (int i = 0; i < 15 - strlen(current_player->name); i++) printf(" ");
                    printf(BOLD CYAN "│\n" RESET);
                    printf(BOLD CYAN "  │" RESET "  总场次: %-18d " BOLD CYAN "│\n" RESET, current_player->total_games);
                    printf(BOLD CYAN "  │" RESET "  胜利次数: %-15d " BOLD CYAN "│\n" RESET, current_player->total_wins);
                    if (current_player->total_games > 0) {
                        printf(BOLD CYAN "  │" RESET "  胜  率: ");
                        printf(GREEN "%.1f%%", (double)current_player->total_wins / current_player->total_games * 100);
                        printf("                        " BOLD CYAN "│\n" RESET);
                    }
                    printf(BOLD CYAN "  │" RESET "  最佳成绩: %-15d " BOLD CYAN "│\n" RESET, current_player->best_attempts);
                    printf(BOLD CYAN "  └─────────────────────────────────┘\n" RESET);
                }
                break;
                
            case 3:
                display_ranking(players, player_count);
                break;
                
            case 4:
                if (current_player) {
                    save_players(players, player_count);
                    current_player = NULL;
                    printf("\n  " YELLOW "✓ 已退出登录\n" RESET);
                }
                break;
                
            default:
                printf("  " RED "✗ 无效选项！\n" RESET);
        }
    }
    
    return 0;
}
