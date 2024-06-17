#include "start.h"
#include "stdio.h"
#include "builtin.h"
#include "string.h"
#include "test.h"
#include "utils.h"
#include "stdbool.h"
#include "theme.h"

#define CMD_SIZE 25

typedef struct {
    char *cmd;
    void (*func)(int, char**);
    char *help;
    bool canCrashKernel;
} Command;

int proc_runner_until(void * arg){
    int num = (int)arg;
    test_until(num);
    return 0;
}
int proc_runner(void * arg){
    int num = (int)arg;
    test_run(num);
    return 0;
}

void cmd_test_run(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Utilisation: test_run <num>\n");
        return;
    }
    int num = atoi(argv[1]);
    int pid = start(&proc_runner, 4000, 128, NULL, (void*)num);
    waitpid(pid, NULL);
    printf("Test finished\n");
}

void cmd_test_until(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Utilisation: test_until <num>\n");
        return;
    }
    int num = atoi(argv[1]);
    int pid = start(&proc_runner_until, 4000, 128, NULL, (void*)num);
    waitpid(pid, NULL);
    printf("Test finished\n");
}

void cmd_exit(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    int cpt = 3;
    while (cpt > 0) {
        printf("Fermeture dans %d secondes\n", cpt);
        wait_clock(100);
        cpt--;
    }
    printf("Sortie du noyau imminent...\n");
    exit_kernel();
    exit(0);
}

void cmd_uwu(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    const char* frame = "      /\\ ___ /\\ \n     =  u w u  =\n      /      \\\n";
    printf("\n\n\n");
    printf("MIAOW!!!\n DONNE MOI DES CROQUETTES!!!\n");
    printf("%s", frame);

    printf("Voulez-vous donner des croquettes a ce chat? (o/n)\n");
    char c;
    cons_read(&c, 1);
    printf("\n");
    if (c == 'o') {
        printf("Merci pour les croquettes! MIAOW!!!\n");
    } else {
        printf("Miaow... :(\n");
    }
}

void cmd_help(int argc, char *argv[]) {
    (void)argc;(void)argv;

    extern Command commands[];
    extern int num_commands;
    printf("Commandes disponibles :\n");
    for (int i = 0; i < num_commands; ++i) {
        printf("\n-    %s: %s %s\n", commands[i].cmd, commands[i].canCrashKernel ? "[Dangereux]" : "[Sur]", commands[i].help);
    }
}

void cmd_ps(int argc, char *argv[]) {
    (void)argc;(void)argv;
    affiche_table_process();
}

void parse_command(char *input, int *argc, char *argv[]) {
    *argc = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        argv[(*argc)++] = token;
        token = strtok(NULL, " ");
    }
    argv[*argc] = NULL; // Terminator for the array of strings
}

Command commands[] = {
    {"test_run", cmd_test_run, "Lancer un test", true},
    {"test_until", cmd_test_until, "Lancer un test ", true},
    {"ps", cmd_ps, "Lister les processus", false},
    {"exit", cmd_exit, "Sortir du noyau", false},
    {"uwu", cmd_uwu, "Miaow", false},
    {"help", cmd_help, "Afficher aide", false},
    {"theme", cmd_theme, "Theme Happy Birthday too you", false},
};

int num_commands = sizeof(commands) / sizeof(commands[0]);

int superShell(void *arg) {
    (void)arg; // Pour éviter l'avertissement sur le paramètre non utilisé

    while (true) {
        char str[CMD_SIZE];
        char *argv[CMD_SIZE / 2 + 1];  // Pointeurs pour chaque argument
        int argc;

        printf("$ ");
        cons_read(str, CMD_SIZE);
        parse_command(str, &argc, argv);

        if (argc == 0) continue;  // Pas de commande entrée

        int found = 0;
        for (int i = 0; i < num_commands; ++i) {
            if (strcmp(argv[0], commands[i].cmd) == 0) {
                // Si la fonction est dangereuse, demander confirmation
                if (commands[i].canCrashKernel) {
                    printf("Attention: cette commande peut planter le noyau. Continuer? (o/n)\n");
                    char c;
                    cons_read(&c, 1);
                    printf("\n");
                    if (c != 'o') {
                        printf("Commande annulee.\n");
                        continue;
                    }
                }
                commands[i].func(argc, argv);
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Commande non reconnue: %s\n", argv[0]);
            printf("Tapez 'help' pour voir la liste des commandes.\n");
        }
    }

    return 0;
}

int c_runner(void){
  // printf("proc runner !\n");
  //test_until(20);
  //test_run(17);
  return 0;
}


void printWelcomeMessage() {
    printf("**********************************************\n");
    printf("*                                            *\n");
    printf("*            Welcome to GenesisOS            *\n");
    printf("*                                            *\n");
    printf("**********************************************\n");
    printf("  ,-.       _,---._ __  / \\\n");
    printf(" /  )    .-'       `./ /   \\\n");
    printf("(  (   ,'            `/    /|\n");
    printf(" \\  `-\"             \\'\\   / |\n");
    printf("  `.              ,  \\ \\ /  |\n");
    printf("   /`.          ,'-`----Y   |\n");
    printf("  (            ;        |   '\n");
    printf("  |  ,-.    ,-'         |  /\n");
    printf("  |  | (   |        uwu | /\n");
    printf("  )  |  \\  `.___________|/\n");
    printf("  `--'   `--'\n");
    printf("\n");
    printf("Kernel Version: 1.0.0\n");
    printf("\n");
    printf("Type 'help' for a list of commands.\n");
    printf("\n");
}


void user_start(void) {
    set_color(JAUNE);
    printWelcomeMessage(NULL);
    set_color(BLANC);
    //start(&proc_runner, 4000, 128, NULL, NULL);
    start(&superShell, 4000, 2, "superShell", NULL);

  return;
}
