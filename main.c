#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main(void)
  {
    time_t t;
    srand((unsigned) time(&t));
    clrscr();
    printf("Per una migliore esperienza, imposti il gioco a schermo intero\nOra prema INVIO per continuare...");
    while (getchar() != '\n');
    clrscr();
    // TRANNOI
    printf("\t\t\t\033[107m#######\033[0m   \033[107m####\033[0m      \033[107m####\033[0m    \033[107m##\033[0m    \033[107m#\033[0m    \033[107m##\033[0m    \033[107m#\033[0m    \033[107m####\033[0m     \033[107m#\033[0m\n");
    printf("\t\t\t   \033[107m#\033[0m      \033[107m#\033[0m   \033[107m#\033[0m    \033[107m#\033[0m    \033[107m#\033[0m   \033[107m#\033[0m \033[107m#\033[0m   \033[107m#\033[0m    \033[107m#\033[0m \033[107m#\033[0m   \033[107m#\033[0m   \033[107m#\033[0m    \033[107m#\033[0m    \033[107m#\033[0m\n");
    printf("\t\t\t   \033[107m#\033[0m      \033[107m####\033[0m     \033[107m######\033[0m   \033[107m#\033[0m  \033[107m#\033[0m  \033[107m#\033[0m    \033[107m#\033[0m  \033[107m#\033[0m  \033[107m#\033[0m   \033[107m#\033[0m    \033[107m#\033[0m    \033[107m#\033[0m\n");
    printf("\t\t\t   \033[107m#\033[0m      \033[107m#\033[0m  \033[107m#\033[0m     \033[107m#\033[0m    \033[107m#\033[0m   \033[107m#\033[0m   \033[107m#\033[0m \033[107m#\033[0m    \033[107m#\033[0m   \033[107m#\033[0m \033[107m#\033[0m   \033[107m#\033[0m    \033[107m#\033[0m    \033[107m#\033[0m\n");
    printf("\t\t\t   \033[107m#\033[0m      \033[107m#\033[0m   \033[107m#\033[0m    \033[107m#\033[0m    \033[107m#\033[0m   \033[107m#\033[0m    \033[107m##\033[0m    \033[107m#\033[0m    \033[107m##\033[0m    \033[107m####\033[0m     \033[107m#\033[0m\n");
    printf("\n\n");
    // ASTRONAUTI - IMPOSTORI
    printf("     \033[45m          \033[0m          \033[41m          \033[0m          \033[103m          \033[0m          \033[100m          \033[0m          \033[44m          \033[0m         \033[107m          \033[0m     \n");
    printf("    \033[45m             \033[0m       \033[41m             \033[0m       \033[103m             \033[0m       \033[100m             \033[0m       \033[44m             \033[0m     \033[107m             \033[0m   \n");
    printf("  \033[45m        \033[104m       \033[0m     \033[41m        \033[104m       \033[0m     \033[103m        \033[106m       \033[0m     \033[100m        \033[106m       \033[0m     \033[44m        \033[106m       \033[0m     \033[106m       \033[107m        \033[0m   \n");
    printf("  \033[45m               \033[0m     \033[41m               \033[0m     \033[103m               \033[0m     \033[100m               \033[0m     \033[44m               \033[0m     \033[107m               \033[0m   \n");
    printf("  \033[45m              \033[0m      \033[41m               \033[0m     \033[103m              \033[0m      \033[100m              \033[0m      \033[44m              \033[0m       \033[107m              \033[0m    \n");
    printf("  \033[45m              \033[0m      \033[41m               \033[0m     \033[103m              \033[0m      \033[100m              \033[0m      \033[44m              \033[0m       \033[107m              \033[0m    \n");
    printf("    \033[45m            \033[0m        \033[41m            \033[0m        \033[103m            \033[0m        \033[100m            \033[0m        \033[44m            \033[0m       \033[107m            \033[0m    \n");
    printf("    \033[45m   \033[0m      \033[45m   \033[0m        \033[41m   \033[0m      \033[41m   \033[0m        \033[103m   \033[0m      \033[103m   \033[0m        \033[100m   \033[0m      \033[100m   \033[0m        \033[44m   \033[0m      \033[44m   \033[0m       \033[107m   \033[0m      \033[107m   \033[0m    \n");
    printf("\n\n");
    printf("Un gruppo di \033[1;34mastronauti\033[0m si trova in un viaggio sull'astronave Skelt, e il loro obiettivo è riuscire a completare tutte le attività previste (le quest) per il mantenimento della nave, arrivanddo così a destinazione. Tra di loro si celano però anche degli \033[1;31mimpostori\033[0m, il cui scopo è eliminare di nascosto gli \033[1;34mastronauti\033[0m senza farsi scoprari da essi. Riusciranno ad arrivare a destinazione prima di essere decimati? Trannoi è liberamente ispirato ad un gioco esistente.");
    while (getchar() != '\n');
    clrscr();
    int scelta = 0;

    do
    {
      if(scelta == 0)
      {
        printf("\n-----------------------------------------------------------------------------+\n");
        printf("|                        BENVENUTO NEL MENU' PRINCIPALE                     |\n");
        printf("+----------------------------------------------------------------------------+\n\n");
      }
      printf("-Inserisci 1 per impostare il gioco\n-Inserisci 2 per iniziare una nuova partita\n-Inserisci 3 per uscire dal gioco\n\nOpzione: ");
      scanf("%d", &scelta);
      while (getchar() != '\n');
      switch (scelta)
      {
        case 1:
              Imposta_Gioco();
              scelta = 0;
              break;
        case 2:
              Gioca();
              scelta = 0;
              break;
        case 3:
              Termina_Gioco();
              break;
        default:
        printf("\nComando non valido, si prega di inserire un valore tra 1 e 3.\n\n");
        scelta = 4;
      }
    }
    while (scelta != 3);
    return 0;
}
