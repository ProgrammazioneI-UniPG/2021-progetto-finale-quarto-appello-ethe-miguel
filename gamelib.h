#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Pulitore di schermo
#ifdef _WIN32      // Windows
#include <conio.h> 
#define clrscr() system("cls");
#else /* Linux */
#define clrscr() printf("\e[1;1H\e[2J") /* ASCII sequenza di scape */
#endif

enum Stato_giocatore {astronauta, impostore, assassinato, defenestrato};
enum Nome_giocatore {rosso, blu, arancione, bianco, marrone, nero, verde, ciano, giallo, viola};
enum Tipo_stanza {vuota, quest_semplice, quest_complicata, botola};

typedef struct Stanza {
  struct Stanza *NextAvanti;
  struct Stanza *NextDestra;
  struct Stanza *NextSinistra;
  struct Stanza *stanza_precedente; 
  enum Tipo_stanza stanza;
  int emergenza_chiamata;
}Stanza;

typedef struct Giocatore {
  struct Stanza *posizione;
  enum Stato_giocatore stato;
  enum Nome_giocatore colore;
}Giocatore;

typedef struct linked_list{
  struct Stanza *position;
  struct linked_list *next;
}linked_list;

void Imposta_Gioco();
void Gioca();
void Termina_Gioco();
