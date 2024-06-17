#include "builtin.h"
#include "string.h"
#include "stdio.h"

static void test_it()
{
	__asm__ volatile("pushfl; testl $0x200,(%%esp); jnz 0f; sti; nop; cli; 0: addl $4,%%esp\n":::"memory");
}
static int
assert_failed(const char *cond, const char *file, int line)
{
	printf("%s:%d: assertion '%s' failed.\n", file, line, cond);
	*(char *)0 = 0;
	exit(-1);
	while (1) ;
}
#define __STRING(x) #x

#define assertt(cond) \
((void)((cond) ? 0 : assert_failed(__STRING(cond), __FILE__, __LINE__)))

#define NR_PHILO 5

static char f[NR_PHILO]; /* tableau des fourchettes, contient soit 1 soit 0 selon si elle est utilisée ou non */
static char bloque[NR_PHILO]; /* mémorise l'état du philosophe, contient 1 ou 0 selon que le philosophe est en attente d'une fourchette ou non */
static int mutex_philo; /* exclusion mutuelle */
static int s[NR_PHILO]; /* un sémaphore par philosophe */
static int etat[NR_PHILO];

static void affiche_etat() {
    int i;
    printf("%c", 13);
    for (i = 0; i < NR_PHILO; i++) {
        unsigned long c;
        switch (etat[i]) {
            case 'm':
                c = 2;
                break;
            default:
                c = 4;
        }
        assertt(c % 2 == 0); // utilisation de c pour le compilateur
        printf("%c", etat[i]);
    }
}

static void waitloop(void) {
    int j;
    for (j = 0; j < 5000; j++) {
        int l;
        test_it();
        for (l = 0; l < 5000; l++);
    }
}

static void penser(long i) {
    wait(mutex_philo); /* DEBUT SC */
    etat[i] = 'p';
    affiche_etat();
    signal(mutex_philo); /* Fin SC */
    waitloop();
    wait(mutex_philo); /* DEBUT SC */
    etat[i] = '-';
    affiche_etat();
    signal(mutex_philo); /* Fin SC */
}

static void manger(long i) {
    wait(mutex_philo); /* DEBUT SC */
    etat[i] = 'm';
    affiche_etat();
    signal(mutex_philo); /* Fin SC */
    waitloop();
    wait(mutex_philo); /* DEBUT SC */
    etat[i] = '-';
    affiche_etat();
    signal(mutex_philo); /* Fin SC */
}

static int test(int i) {
    /* les fourchettes du philosophe i sont-elles libres ? */
    return ((!f[i] && (!f[(i + 1) % NR_PHILO])));
}

static void prendre_fourchettes(int i) {
    /* le philosophe i prend des fourchettes */
    wait(mutex_philo); /* Debut SC */
    if (test(i)) {  /* on tente de prendre 2 fourchettes */
        f[i] = 1;
        f[(i + 1) % NR_PHILO] = 1;
        signal(s[i]);
    } else {
        bloque[i] = 1;
    }
    signal(mutex_philo); /* FIN SC */
    wait(s[i]); /* on attend au cas où on ne puisse pas prendre 2 fourchettes */
}

static void poser_fourchettes(int i) {
    wait(mutex_philo); /* DEBUT SC */
    if ((bloque[(i + NR_PHILO - 1) % NR_PHILO]) && (!f[(i + NR_PHILO - 1) % NR_PHILO])) {
        f[(i + NR_PHILO - 1) % NR_PHILO] = 1;
        bloque[(i + NR_PHILO - 1) % NR_PHILO] = 0;
        signal(s[(i + NR_PHILO - 1) % NR_PHILO]);
    } else {
        f[i] = 0;
    }
    if ((bloque[(i + 1) % NR_PHILO]) && (!f[(i + 2) % NR_PHILO])) {
        f[(i + 2) % NR_PHILO] = 1;
        bloque[(i + 1) % NR_PHILO] = 0;
        signal(s[(i + 1) % NR_PHILO]);
    } else {
        f[(i + 1) % NR_PHILO] = 0;
    }
    signal(mutex_philo); /* Fin SC */
}

static int philosophe(void *arg) {
    /* comportement d'un seul philosophe */
    int i = (int)arg;
    int k;
    for (k = 0; k < 6; k++) {
        prendre_fourchettes(i); /* prend 2 fourchettes ou se bloque */
        manger(i); /* le philosophe mange */
        poser_fourchettes(i); /* pose 2 fourchettes */
        penser(i); /* le philosophe pense */
    }
    wait(mutex_philo); /* DEBUT SC */
    etat[i] = '-';
    affiche_etat();
    signal(mutex_philo); /* Fin SC */
    return 0;
}

static int launch_philo() {
    int i, pid;
    for (i = 0; i < NR_PHILO; i++) {
        pid = start(philosophe, 4000, 192, "philosophe", (void *)i);
        assertt(pid > 0);
    }
    return 0;
}

int test200(void * arg) {
    int j, pid;
    arg = arg;
    mutex_philo = screate(1); /* sémaphore d'exclusion mutuelle */
    for (j = 0; j < NR_PHILO; j++) {
        s[j] = screate(0); /* sémaphore de blocage des philosophes */
        f[j] = 0;
        bloque[j] = 0;
        etat[j] = '-';
    }
    printf("\n");
    pid = start(launch_philo, 4000, 193, "Lanceur philosophes", 0);
    assertt(pid > 0);
    assertt(waitpid(pid, 0) == pid);
    printf("\n");
    sdelete(mutex_philo);
    for (j = 0; j < NR_PHILO; j++) {
        sdelete(s[j]);
    }

    return 0;
}

void cmd_test_semaphore(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    set_color(VERT);
    printf("Test semaphore\n");set_color(BLANC);

    start(&test200, 4000, 128, "test_semaphore", NULL);

    set_color(VERT);printf("Test finished\n");
    set_color(BLANC);

    return;
}