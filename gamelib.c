//MIGUEL TORRES - Matricola n.333945

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"
#include <string.h>
#define TRUE 1      //Usato per il bool
#define FALSE 0
#define DEBUG FALSE

//Variabili globali
int giocabile = FALSE;
unsigned short numero_giocatori;
Giocatore *Giocatori;
Stanza *stanza_inizio;
int numero_stanze = 0;
int quest_completate = 0;
unsigned short quest_da_finire;
//Nombre Jugadores
static char *Nomi_Colori[10] = {"\033[1;31mRosso\033[0m", "\033[1;34mBlu\033[0m", "\033[1;91mArancione\033[0m", "\033[1;37mBianco\033[0m", "\033[1;33mMarrone\033[0m",
                                "\033[1;30mNero\033[0m", "\033[1;32mVerde\033[0m", "\033[1;36mCiano\033[0m", "\033[1;33mGiallo\033[0m", "\033[1;35mViola\033[0m"};
//Stato dei giocatori
static char *Stati_Giocatori[4] = {"\033[1;34mastronauta\033[0m", "\033[1;91mimpostore\033[0m", "\033[1;95massassinato\033[0m", "\033[1;35mdefenestrato\033[0m"};
//Tipo di stanze
static char *Tipo_Stanza[4] = {"\033[1;34mVuota\033[0m", "\033[1;91mQuest Semplice\033[0m", "\033[1;95mQuest Complicata\033[0m", "\033[1;35mBotola\033[0m"};

//Prototipi delle funzioni
static void imposta_giocatori();                        // Funzione per creare i giocatori (astronauti e impostori)
static Stanza *prob_stanze();                           // Seleziona con probabilita' i tipi delle stanze
static void movimenti_disponibili(int player_selected); // Verifica se il giocatore è impostore oppure astronauta
static void info_player(int player_selected);           // Mostra le informazioni rispettive del giocatore
static void impostor(int player_selected);              // Mostra le opzioni disponibili per gli impostori
static void astronaut(int player_selected);             // Mostra le opzioni disponibili per gli astronauti
static void avanza(int player_selected);                // Funzione che fa muovere il giocatore, crea le future stanze e le connetta dinamicamente
static int chiamata_emergenza(int player_selected);     // Verifica se c'è un giocatore assassinato ed espulsa uno dei giocatori che si trovano nella stessa stanza
static int uccidi_astronauta(int player_selected);      // Uccide un astronauta se ci sono
static int game_over();                                 // Verifica le condizioni per terminare il gioco
static int esegui_quest(int player_selected);           // Verifica se la stanza è di tipo quest semplice o complicata e la effettua
static int botolina(int player_selected);               // Può essere effettuata dagli impostori e gli sposta ad un'altra stanza botola se ci sono
static int sabotaggio(int player_selected);             // Può essere effettuata dagli impostori
static void Stampa_Info_Inizio();                       // Stampa la posizione iniziale di tutti i giocatori
static void trova_stanze(linked_list *current);         // Cerca la lista di stanze e le mette in un lista linkata usando ricorsione
static void delete_room(Stanza *current);               // Cancella tutte le stanze usando ricorsione

void Imposta_Gioco()
{
  do //Numero di giocatori
  {
    printf("\nInserisci il numero di giocatori (tra 4 e 10): ");
    scanf("%hd", &numero_giocatori);
    if (numero_giocatori < 4 || numero_giocatori > 10)
    {
      printf("\n\nIl valore inserito deve essere non meno di 4 e non più di 10. Riprovare.\n\n");
      int c;
      while ((c = getchar()) != '\n' && c != EOF); // Per non effettuare un ciclo infinito
    }
  } while (numero_giocatori < 4 || numero_giocatori > 10);

  do //Numero di quest da finire
  {
    printf("\nInserisci il numero di quest da completare: ");
    int numero;
    scanf("%d", &numero);
    if(numero>0) quest_da_finire = numero;
    
    if (quest_da_finire < numero_giocatori)
    {
      printf("\nIl numero di quest inserito deve essere uguale o superiore al numero di giocatori. Riprovare.\n\n");
      int c;
      while ((c = getchar()) != '\n' && c != EOF); // Per non effettuare un ciclo infinito
    }
  } while (quest_da_finire < numero_giocatori );
  int scelta;
  int continua = FALSE;
  imposta_giocatori(numero_giocatori); //Chiamo la funzione per creare i giocatori (astronauti e impostori)
  do
  {
    printf("\n\nOra scelga una delle seguenti opzioni:\n1.-Stampare i giocatori selezionati.\n2.-Tornare al menu' principale.\nScelta: ");
    scanf("%d", &scelta);
    if (scelta != 1 && scelta != 2)
    {
      printf("\nOpzione non valida. Riprovare.");
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
    }
    if (scelta == 1)
      Stampa_Info_Inizio();
    if (scelta == 2)
      continua = TRUE;
  } while (continua == FALSE);
}

void Gioca()
{
  if (giocabile != TRUE) //Verifica la flag di impostazione del gioco
  {
    printf("\nSi prega di impostare il gioco prima di giocare.\n\n");
    return;
  }
  clrscr();
  printf("\tInizio della partita.\n");
  int *lista_turni = (int *)malloc(sizeof(int) * numero_giocatori);
  if (lista_turni == NULL)
  {
    printf("Errore sulla memoria dinamica\n. Riesegua il gioco per riprovare.\n");
    exit(EXIT_FAILURE);
  }

  //Creazione della lista di turni
  int x, y;
  int success, current;
  for (int i = 0; i < numero_giocatori; i++)
  {
    success = FALSE;
    lista_turni[i] = rand() % numero_giocatori;
    current = i - 1;
    while (success != TRUE)
    {
      if (i == 0 || current < 0)
      {
        success = TRUE;
      }
      else if (lista_turni[i] == lista_turni[current])
      {
        current = i - 1;
        lista_turni[i] = rand() % numero_giocatori;
      }
      else
      {
        current--;
      }
    }
    x = lista_turni[i];
    x = Giocatori[x].colore;
    printf("\nTurno numero %d = %s", i + 1, Nomi_Colori[x]);
  }
  while (getchar() != '\n')
    ;
  clrscr();
  //Verifica stato dei giocatori
  int game_end = FALSE;
  do
  {
    for (int i = 0; i < numero_giocatori; i++)
    {
      x = lista_turni[i];
      if (Giocatori[x].stato != assassinato && Giocatori[x].stato != defenestrato)
      {
        y = Giocatori[x].stato;
        x = Giocatori[x].colore;
        printf("\n\nE'il turno del giocatore %s (%s)\n", Nomi_Colori[x], Stati_Giocatori[y]);
        info_player(lista_turni[i]);
        movimenti_disponibili(lista_turni[i]); //Mostra le opzioni disponibili dipendendo del tipo di giocatore e tipo di stanza
      }
      game_end = game_over();
      if (game_end == TRUE)
      {
        break;
      }
    }
  } while (game_end != TRUE);
  free(lista_turni); // libera lo spazio di memoria allocata dinamicamente
}

void Termina_Gioco()
{
  //Libera la memoria dinamica dei giocatori con free
  if (numero_giocatori > 0)
    free(Giocatori);
  if (numero_stanze > 0)
    delete_room(stanza_inizio); // Libera la memoria dinamica delle stanze create, con free
  printf("\n\tGioco terminato.\n\tGrazie per giocare.\n\n");
}

static void info_player(int player_selected)
{
  //printf("room id: %p\n", Giocatori[player_selected].posizione);
  printf("Si trova nella stanza di tipo %s con indirizzo %p\n", Tipo_Stanza[Giocatori[player_selected].posizione->stanza], Giocatori[player_selected].posizione);
  printf("Compagni di stanza: ");
  //stampa giocatori nella stessa stanza
  for (int i = 0; i < numero_giocatori; i++)
  {
    if (i != player_selected) //verifica che non sia lo stesso giocatore
    {
      if (Giocatori[player_selected].posizione == Giocatori[i].posizione)
      {
        printf("%s ", Nomi_Colori[Giocatori[i].colore]);
        if (Giocatori[i].stato == assassinato)
          printf("(dead)");
        if (Giocatori[i].stato == defenestrato)
          printf("(defenestrato)");
      }
    }
  }
  printf("\n");
}
static void imposta_giocatori()
{
  Giocatori = malloc(sizeof(Giocatore) * numero_giocatori); //alloca la memoria dinamica per i giocatori
  if (Giocatori == NULL)
  {
    printf("Errore sulla memoria dinamica\n. Riesegua il gioco per riprovare.\n");
    exit(EXIT_FAILURE);
  }
  int success;
  int current;

  // Imposta i colori ai giocatori di modo random
  for (int i = 0; i < numero_giocatori; i++)
  {
    success = FALSE;
    Giocatori[i].colore = rand() % 10;
    current = i - 1;
    while (success != TRUE)
    {
      if (i == 0 || current < 0)
      {
        success = TRUE;
      }
      else if (Giocatori[i].colore == Giocatori[current].colore)
      {
        if (DEBUG) // Per provare se funziona o salta qualche errore
          printf("numero eseguito %d partial = %d corrisponde a %d \n", i, Giocatori[i].colore, current);
        current = i - 1;
        Giocatori[i].colore = rand() % 10;
      }
      else
      {
        current--;
      }
    }
    if (DEBUG) // Per provare se funziona o salta qualche errore
      printf("numero %d = %d\n", i, Giocatori[i].colore);
  }
  //Selezione dei giocatori
  int impostori = 0;
  do
  {
    for (int i = 0; i < numero_giocatori; i++)
    {
      if (numero_giocatori == 4 && impostori <= 1)
      {
        if ((rand() % 100 <= 25) && (impostori < 1))
        {
          Giocatori[i].stato = impostore;
          impostori++;
        }
        else
          Giocatori[i].stato = astronauta;
      }
      if (numero_giocatori > 4 && numero_giocatori <= 6 && impostori < 2)
      {
        if (rand() % 100 <= 25)
        {
          Giocatori[i].stato = impostore;
          impostori++;
        }
        else
          Giocatori[i].stato = astronauta;
      }
      if (numero_giocatori > 6 && numero_giocatori <= 8 && impostori < 3)
      {
        if (rand() % 100 <= 50)
        {
          Giocatori[i].stato = impostore;
          impostori++;
        }
        else
          Giocatori[i].stato = astronauta;
      }
      if (numero_giocatori > 8 && impostori < 3)
      {
        if (rand() % 100 <= 75)
        {
          Giocatori[i].stato = impostore;
          impostori++;
        }
        else
          Giocatori[i].stato = astronauta;
      }
    }

  } while (impostori == 0);
  
  //IMPOSTAZIONI STANZE
  stanza_inizio = prob_stanze(); // Crea la prima stanza
  //Assegna a tutti i giocatori la prima stanza come il punto di partenza
  for (int i = 0; i < numero_giocatori; i++)
  {
    Giocatori[i].posizione = stanza_inizio;
  }
  giocabile = TRUE;
}
static void Stampa_Info_Inizio()
{
  printf("\nLa posizione iniziale  di tutti i giocatori è di tipo %s\n", Tipo_Stanza[stanza_inizio->stanza]); // stampa la posizione di tutti i giocatori all'inizio
  printf("\nGiocatore |Color |State|\n");
  for (int i = 0; i < numero_giocatori; i++)
  {
    printf("%d %s è un %s \n", i + 1, Nomi_Colori[(Giocatori[i].colore)], Stati_Giocatori[(Giocatori[i].stato)]); // stampa i colori e gli stati assegnati ad ogni giocatore
  }
}
static Stanza *prob_stanze()
{
  Stanza *newroom = malloc(sizeof(Stanza)); // Creazione delle stanze con memoria dinamica
  if (newroom == NULL)
  {
    printf("Errore sulla memoria dinamica\n. Riesegua il gioco per riprovare.\n");
    exit(EXIT_FAILURE);
  }
  int aleatory = rand() % 100 + 1;
  if (aleatory <= 25)
  {
    newroom->stanza = botola;
  }
  else if (aleatory > 25 && aleatory <= 40)
  {
    newroom->stanza = quest_complicata;
  }
  else if (aleatory > 40 && aleatory <= 70)
  {
    newroom->stanza = quest_semplice;
  }
  else
  {
    newroom->stanza = vuota;
  }
  newroom->emergenza_chiamata = FALSE; //inizializza il flag di riunione di emergenza come FALSO

  return newroom;
}
static void movimenti_disponibili(int player_selected)
{
  printf("\nOpzioni disponibili per il giocatore:\n");
  switch (Giocatori[player_selected].stato)
  {
  case impostore:
    impostor(player_selected); // Quando il giocatore è un impostore chiama questa funzione
    break;
  case astronauta:
    astronaut(player_selected); // Quando il giocatore è un astronauta chiama questa funzione
    break;
  default:
    break;
  }
}
static void impostor(int player_selected)
{
  int c;
  int option;
  int invalid = FALSE;
  while (invalid != TRUE) //Controlla sia l'input che l'opzione
  {
    printf("1.- Avanza\n2.- Chiamata Emergenza\n3.- Uccido Astronauta\n4.- Usa botola\n5.- Sabottagio");
    printf("\nSelezionare l'opzione da effettuare: ");
    scanf("%d", &option);
    switch (option)
    {
    case 1:
      avanza(player_selected);
      invalid = TRUE;
      break;
    case 2:
      printf("Chiamata di Emergenza\n");
      invalid = chiamata_emergenza(player_selected);
      break;
    case 3:
      printf("Uccido Astronauta\n");
      invalid = uccidi_astronauta(player_selected);
      break;
    case 4:
      printf("Uso botola\n");
      invalid = botolina(player_selected);
      break;
    case 5:
      printf("\nSabbotagio\n");
      invalid = sabotaggio(player_selected);
    default:
      while ((c = getchar()) != '\n' && c != EOF); // Per non effettuare un ciclo infinito
      break;
    }
    if (invalid == FALSE)
      printf("\nOpzione non valida, riprovare.\n");
  }
}
static void astronaut(int player_selected)
{
  int c;
  int option;
  int invalid = FALSE;
  while (invalid != TRUE) //Controlla sia l'input che l'opzione
  {
    printf("\n1.- Avanza\n2.- Esegui Quest\n3.- Chiamata Emergenza");
    printf("\nSelezionare l'opzione da effettuare: ");
    scanf("%d", &option);
    switch (option)
    {
    case 1:
      avanza(player_selected);
      invalid = TRUE;
      break;
    case 2:
      printf("Esegui Quest\n");
      invalid = esegui_quest(player_selected);
      break;
    case 3:
      printf("Chiama Emergenza\n");
      invalid = chiamata_emergenza(player_selected);
      break;
    default:
      while ((c = getchar()) != '\n' && c != EOF); // Per non effettuare un ciclo infinito
      break;
    }
    if (invalid == FALSE)
      printf("\nOpzione non valida, riprovare.\n");
  }
}

static void avanza(int player_selected)
{
  int valid = FALSE;
  int c;
  int option;
  while (valid == FALSE)
  {
    printf("\nSeleziona la direzione:\n\n1.- Avanti.\n2.- Destra.\n3.- Sinistra.\n4.- Rimanere fermo.\nScelta: ");
    scanf("%d", &option);
    switch (option)
    {
    case 1: // AVANZA - AVANTI
      printf("Avanti\n");
      if (Giocatori[player_selected].posizione->NextAvanti == NULL)
      {
        numero_stanze++;
        Stanza *new_room;
        new_room = prob_stanze();
        Giocatori[player_selected].posizione->NextAvanti = new_room;
        new_room->stanza_precedente = Giocatori[player_selected].posizione;
        Giocatori[player_selected].posizione = new_room; //si aggiorna la posizione attuale del giocatore alla nuova
        info_player(player_selected);
      }
      else
      {
        Giocatori[player_selected].posizione = Giocatori[player_selected].posizione->NextAvanti;
        info_player(player_selected);
      }
      valid = TRUE;
      break;
    case 2:
      printf("Destra\n");
      if (Giocatori[player_selected].posizione->NextDestra == NULL)
      {
        numero_stanze++;
        Stanza *new_room;
        new_room = prob_stanze();
        Giocatori[player_selected].posizione->NextDestra = new_room;
        new_room->stanza_precedente = Giocatori[player_selected].posizione;
        Giocatori[player_selected].posizione = new_room; //si aggiorna la posizione attuale del giocatore alla nuova
        info_player(player_selected);
      }
      else
      {
        Giocatori[player_selected].posizione = Giocatori[player_selected].posizione->NextDestra;
        info_player(player_selected);
      }
      valid = TRUE;
      break;
    case 3:
      printf("Sinistra\n");
      if (Giocatori[player_selected].posizione->NextSinistra == NULL)
      {
        numero_stanze++;
        Stanza *new_room;
        new_room = prob_stanze();
        Giocatori[player_selected].posizione->NextSinistra = new_room;
        new_room->stanza_precedente = Giocatori[player_selected].posizione;
        Giocatori[player_selected].posizione = new_room; //si aggiorna la posizione attuale del giocatore alla nuova
        info_player(player_selected);
      }
      else
      {
        Giocatori[player_selected].posizione = Giocatori[player_selected].posizione->NextSinistra;
        info_player(player_selected);
      }
      valid = TRUE;
      break;
    case 4:
      printf("Rimanere fermo!\n");
      valid = TRUE;
      break;
    default:
      printf("Opzione non valida, riprovare.");
      while ((c = getchar()) != '\n' && c != EOF); // Per non effettuare un ciclo infinito
      break;
    }
  }
}
static int esegui_quest(int player_selected)
{
  if (Giocatori[player_selected].posizione->stanza == quest_semplice)
  {
    quest_completate += 1; // Si somma di 1 alle quest completate
    Giocatori[player_selected].posizione->stanza = vuota;
    printf("Quest terminate = %d\n", quest_completate);
    return TRUE;
  }
  else if (Giocatori[player_selected].posizione->stanza == quest_complicata)
  {
    quest_completate += 2; // Si somma di 2 alle quest completate
    Giocatori[player_selected].posizione->stanza = vuota;
    printf("Quest terminate = %d\n", quest_completate);
    return TRUE;
  }
  else
  {
    printf("\nNon ci sono missioni disponibili in questa stanza.");
    return FALSE;
  }
}
static int chiamata_emergenza(int player_selected)
{
  // controlla se la riunione di emergenza è stata chiamata in precedenza se vero non si può chiamare di nuovo
  if (Giocatori[player_selected].posizione->emergenza_chiamata == TRUE)
  {
    return FALSE;
  }
  //Verifica se c'è un giocatore morto
  int dead_flag = FALSE;
  for (int i = 0; i < numero_giocatori; i++)
  {
    if (Giocatori[player_selected].posizione == Giocatori[i].posizione && Giocatori[i].stato == assassinato) // salta per verificare con il giocatore selezionato stesso
    {
      dead_flag = TRUE;
    }
  }
  if (dead_flag == FALSE)
  {
    return FALSE;
  }
  int impostor_same_room = 0;
  int astronaut_same_rooom = 0;
  for (int i = 0; i < numero_giocatori; i++)
  {
    if (i != player_selected && Giocatori[player_selected].posizione == Giocatori[i].posizione) // salta per verificare con il giocatore selezionato stesso
    {
      if (Giocatori[i].stato == astronauta)
        astronaut_same_rooom++; // somma la quantità di astronauti presenti nella stessa stanza

      if (Giocatori[i].stato == impostore)
        impostor_same_room++; // somma la quantità di impostori presenti nella stessa stanza
    }
  }
  int prob_astronaut, prob_impostor; // probabilità di votazione (espulsione di un giocatore presenta nella stessa stanza)
  prob_astronaut = 30 + (20 * astronaut_same_rooom) - (30 * impostor_same_room);
  prob_impostor = 30 + (20 * impostor_same_room) - (30 * astronaut_same_rooom);
  int espulsione;
  int valid = FALSE;
  while (valid == FALSE)
  {
    for (int i = 0; i < numero_giocatori; i++)
    {
      espulsione = rand() % 100 + 1;
      if (i != player_selected && Giocatori[player_selected].posizione == Giocatori[i].posizione)
      {
        if (Giocatori[i].stato == astronauta)
        {
          if (espulsione <= prob_astronaut)
          {
            Giocatori[i].stato = defenestrato;
            printf("\nIl giocatore %s e' stato defenestrato.\n%s era un astronauta\n", Nomi_Colori[Giocatori[i].colore], Nomi_Colori[Giocatori[i].colore]);
            valid = TRUE;
            break;
          }
        }
        else if (Giocatori[i].stato == impostore)
        {
          if (espulsione <= prob_impostor)
          {
            Giocatori[i].stato = defenestrato;
            printf("\nIl giocatore %s e' stato defenestrato.\n%s era un impostore\n", Nomi_Colori[Giocatori[i].colore], Nomi_Colori[Giocatori[i].colore]);
            valid = TRUE;
            break;
          }
        }
      }
    }
  }
  Giocatori[player_selected].posizione->emergenza_chiamata = TRUE;
  return TRUE;
}
static int uccidi_astronauta(int player_selected)
{
  int *kill_array;
  int espulsione;
  kill_array = (int *)malloc(1 * sizeof(int));
  if (kill_array == NULL)
  {
    printf("Errore sulla memoria dinamica\n. Riesegua il gioco per riprovare.\n");
    exit(EXIT_FAILURE);
  }
  int n = 0;
  for (int i = 0; i < numero_giocatori; i++)
  {
    if (Giocatori[player_selected].posizione == Giocatori[i].posizione && Giocatori[i].stato == astronauta)
    {
      n++;
      kill_array = (int *)realloc(kill_array, n * sizeof(int));
      kill_array[n - 1] = i;
    }
  }
  if (n == 0)
    return FALSE;

  int valid = FALSE;
  int c;
  int option;
  while (valid == FALSE)
  {
    for (int i = 0; i < n; i++)
    {
      printf("%d.- %s\n", i + 1, Nomi_Colori[Giocatori[kill_array[i]].colore]); //Stampa gli astronauti presenti nella stessa stanza
    }
    printf("\nScelta: ");
    scanf("%d", &option);
    if (option > n || option < 1)
    {
      while ((c = getchar()) != '\n' && c != EOF); // Per non effettuare un ciclo infinito
      printf("Opzione non valida, riprovare.\n");
    }
    else
    {
      valid = TRUE;
    }
  }

  printf("\nL'astronauta %s e' stato assassinato\n", Nomi_Colori[Giocatori[kill_array[option - 1]].colore]);
  Giocatori[kill_array[option - 1]].stato = assassinato; // cambia lo stato del giocatore assassinato

  //Probabilità di espulsione dipendendo dei giocatori presenti
  int probability_espulsione = 0;
  for (int i = 0; i < numero_giocatori; i++)
  {
    if (Giocatori[player_selected].posizione == Giocatori[i].posizione && Giocatori[i].stato == astronauta)
      probability_espulsione += 50;
    if (Giocatori[player_selected].posizione->stanza_precedente == Giocatori[i].posizione)
      probability_espulsione += 20;
  }
  //printf("probabilita' = %d\n", probability_espulsione);

  if (probability_espulsione >= 100)
  {
    Giocatori[player_selected].stato = defenestrato;
    printf("\nIl giocatore %s e' stato defenestrato.\n%s era un impostore\n", Nomi_Colori[Giocatori[player_selected].colore], Nomi_Colori[Giocatori[player_selected].colore]);
    return TRUE;
  }
  else if (probability_espulsione == 0)
  {
    return TRUE;
  }
  else
  {
    espulsione = rand() % 100 + 1;
    if (espulsione <= probability_espulsione)
    {
      Giocatori[player_selected].stato = defenestrato;
      printf("\nIl giocatore %s e' stato defenestrato.\n%s era un impostore\n", Nomi_Colori[Giocatori[player_selected].colore], Nomi_Colori[Giocatori[player_selected].colore]);
    }
  }
  free(kill_array); // libera lo spazio di memoria allocato dinamicamente
  return TRUE;
}
static int game_over()
{
  //Stesso numero di astronauti e impostori (Hanno vinto gli impostori)
  int astronauts = 0;
  int impostors = 0;
  for (int i = 0; i < numero_giocatori; i++)
  {
    if (Giocatori[i].stato == astronauta)
      astronauts++;
    if (Giocatori[i].stato == impostore)
      impostors++;
  }
  if (astronauts == impostors)
  {
    printf("\n\n\tStesso numero di impostori e astronauti.\n\tGli impostori hanno vinto la partita.\n");
    return TRUE;
  }
  else if (impostors == 0)
  {
    printf("\n\n\tTutti gli impostori sono stati espulsi.\n\tGli astronauti hanno vinto la partita.\n");
    return TRUE;
  }
  //QUEST TERMINATE (HANNO VINTO GLI ASTRONAUTI)
  if (quest_completate >= quest_da_finire)
  {
    printf("\n\n\tQuest terminate.\n\tGli astronauti hanno vinto la partita.\n");
    return TRUE;
  }
  return FALSE;
}

static int botolina(int player_selected)
{

  if (Giocatori[player_selected].posizione->stanza != botola)
    return FALSE;
  if (numero_stanze == 0)
    return FALSE;
  // Crea una linkedlist con la direzione di ogni stanza trovata su trova_stanze()
  linked_list *head = malloc(sizeof(linked_list)); // linkedlist con le direzioni di tutte le stanze in modo lineare
  if (head == NULL)
  {
    printf("Errore sulla memoria dinamica\n. Riesegua il gioco per riprovare.\n");
    exit(EXIT_FAILURE);
  }
  head->position = stanza_inizio; // imposta la posizione nella testa come la direzione di stanza_inizio
  trova_stanze(head);             // chiama la funzione per trovare le stanze

  //Verifica il numero di stanze
  int numero_di_stanze = 0;
  linked_list *current = head;
  while (current->next != NULL)
  {
    numero_di_stanze++;
    current = current->next;
  }
  /*
  printf("Numero di stanze trovate = %d\n", numero_di_stanze);
  printf("Numero di stanze create  = %d\n", numero_stanze);
  */

  //assegna il giocatore a qualsiasi altra stanza di tipo botola che trova
  current = head;
  while (current->next != NULL)
  {
    current = current->next;
    if (current->position->stanza == botola && Giocatori[player_selected].posizione != current->position)
    {
      Giocatori[player_selected].posizione = current->position;
      info_player(player_selected);
      current = head; // libera lo spazio di memoria allocata dinamicamente delle stanze trovate
      while (current->next != NULL)
      {
        linked_list *whatvever = current;
        current = current->next;
        free(whatvever);
      }
      return TRUE;
    }
  }
  // valuta qualunque stanza trovi
  int stanza_cons = FALSE;
  while (stanza_cons == FALSE)
  {
    current = head;
    numero_di_stanze = rand() % numero_di_stanze;
    for (int i = 0; i < numero_di_stanze; i++)
    {
      current = current->next;
      if (Giocatori[player_selected].posizione != current->position)
      {
        Giocatori[player_selected].posizione = current->position;
        stanza_cons = TRUE;
      }
    }
    info_player(player_selected);
  }
  current = head; // libera lo spazio di memoria allocata dinamicamente delle stanze trovate
  while (current->next != NULL)
  {
    linked_list *whatvever = current;
    current = current->next;
    free(whatvever);
  }
  return TRUE;
}

static void trova_stanze(linked_list *current)
{
  linked_list *overcurrent = current;
  Stanza *place = current->position;
  if (place->NextAvanti != NULL)
  {
    current->next = (linked_list *)malloc(sizeof(linked_list));
    if (current->next == NULL)
    {
      printf("Errore sulla memoria dinamica\n. Riesegua il gioco per riprovare.\n");
      exit(EXIT_FAILURE);
    }
    current->next->position = place->NextAvanti; // inserisce la posizione trovata nel linked list
    trova_stanze(current->next);                 // richiama la funzione trova_stanze per verificare se esistono altre stanze create con NextAvanti
  }
  while (overcurrent->next != NULL)
  {                                  //trova l'ultimo elemento nella linkedlist
    overcurrent = overcurrent->next; // passa la direzione
  }

  if (place->NextDestra != NULL)
  {
    overcurrent->next = (linked_list *)malloc(sizeof(linked_list));
    if (overcurrent->next == NULL)
    {
      printf("Errore sulla memoria dinamica\n. Riesegua il gioco per riprovare.\n");
      exit(EXIT_FAILURE);
    }
    overcurrent->next->position = place->NextDestra; // inserisce la posizione trovata nel linked list
    trova_stanze(overcurrent->next);                 // richiama la funzione trova_stanze per verificare se esistono altre stanze create con NextDestra
  }
  while (overcurrent->next != NULL)
  {                                  //trova l'ultimo elemento nella linkedlist
    overcurrent = overcurrent->next; // passa la direzione
  }
  if (place->NextSinistra != NULL)
  {
    overcurrent->next = (linked_list *)malloc(sizeof(linked_list));
    if (overcurrent->next == NULL)
    {
      printf("Errore sulla memoria dinamica\n. Riesegua il gioco per riprovare.\n");
      exit(EXIT_FAILURE);
    }
    overcurrent->next->position = place->NextSinistra; // inserisce la posizione trovata nel linked list
    trova_stanze(overcurrent->next);                   // richiama la funzione trova_stanze per verificare se esistono altre stanze create con NextAvanti
  }
  while (overcurrent->next != NULL)
  {                                  //trova l'ultimo elemento nella linkedlist
    overcurrent = overcurrent->next; // passa la direzione
  }

  return;
}
static void delete_room(Stanza *current) // funzione chiamata da Termina_Gioco per la liberazione della memoria allocata dinamicamente della creazione delle stanze
{
  Stanza *place = current;
  if (place->NextAvanti != NULL)
  {
    current = current->NextAvanti;
    delete_room(current);
  }
  if (place->NextDestra != NULL)
  {
    current = current->NextDestra;
    delete_room(current);
  }
  if (place->NextSinistra != NULL)
  {
    current = current->NextSinistra;
    delete_room(current);
  }
  free(place); // dopo aver richiamato tutte le volte delete_room per trovare tutte le stanze create, le elimina
}

static int sabotaggio(int player_selected)
{
  if (Giocatori[player_selected].posizione->stanza == quest_semplice || Giocatori[player_selected].posizione->stanza == quest_complicata)
  {
    Giocatori[player_selected].posizione->stanza = vuota;
    printf("\nIl nuovo tipo di sala è %s\n", Tipo_Stanza[Giocatori[player_selected].posizione->stanza]);
    return TRUE;
  }
  else
  {
    printf("\nSabotaggio non effettuato. La stanza non è di tipo quest semplice o quest complicata.\n");
    return FALSE;
  }
}
