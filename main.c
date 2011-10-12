#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>


#include "globals.h"
#include "data_structures.h"
#include "entry_list.h"
#include "fetcher.h"
#include "parser.h"
#include "differ.h"
#include "utils.h"
#include "dispatcher.h"

#define DEFAULT_TIME 30
#define EXIT_FAILURE 1
#define EXIT_SUCESS 0
#define MAX_ENTRIES 200
#define false 0
#define true 1

// Variables de control para threads
int goDown = 0;
int goWork = 0;
int taken = 0;
int sdown = 0;


// Libera recursos:
//    - Lista enlazada de estructuras URL.
//    - Cierra file descriptor.
void 
bye (FILE *fd, URL *urlList)
{
  if (fd)
    fclose (fd);
  if (urlList != NULL)
    free_URL (urlList);
  return;
}


// Función que activa la bandera goWork
// para que el thread realice la conexión 
// con el/los servidor/es especificado/s en 
// la línea de comandos.
void 
SIGALRM_control ()
{
  while (true)
    {
      if (!taken)
        {
          taken = 1;  // Bloquear acceso de variable goWork
          goWork = 1;
          taken = 0;
          return;
        }
    }
}

// Función del thread que chequea si existen archivos 
// nuevos o modificados en el/los servidor/es. 
void *
worker (void *arg)
{
  workerInfo *p = (workerInfo *) arg;
  while (!sdown)
    {
      if (!goDown) // hilo no suspendido 
        {
          if (goWork)
            {
              // llamada a dispatcher
              dispatcher (p->urlList);
              goWork = 0;
              alarm (p->time); // Reactivar señal SIGALRM
            }
        }
    }
  return;
}

// Función que explica el uso del programa.
int 
usage (int status) 
{
  fprintf (stderr, 
           "Uso: ./verific [-t <num_segundos>] [-d <directorio>] [-a <archivo.txt>] \n\
   -t      intervalo de tiempo que pasa hasta que la aplicación verifica cambios en archivos. \n\
   -d      directorio donde se desea verificar cambios en archivos. \n\
   -a      archivo con lista de directorios a verificar. \n");
  exit (status);
}


int 
main (int argc, char **argv) 
{
  if (argc == 1)
    usage (EXIT_FAILURE);

  int a, index;
  int dir_enabled, file_enabled;
  dir_enabled = file_enabled = false;
  unsigned int time = -1;
  char dir[2048];
  char *file = NULL;
  char entry;
  FILE *fd = NULL;
  URL *urlList;
  // Crear hash table
  hcreate (MAX_ENTRIES);
  
  opterr = 0;

  /********* BEGIN Manejo de Opciones por cónsola ******/

  while ((a = getopt (argc, argv, "t:d:a:")) != -1) 
    switch (a)
      {
      case 't':
        if (optarg != 0)
          time = atoi(optarg);
        break;

      case 'd':
        strcpy (dir, optarg);
        dir_enabled = true;
        break;

      case 'a':
        file = optarg;
        fd = fopen (file, "r");
        if (fd == NULL) 
          {
            perror ("No se puedo abrir el archivo %s");
            exit (EXIT_FAILURE);
          }
        file_enabled = true;
        break;

      case '?':
        if (optopt == 'a' || optopt == 'd' || optopt == 't')
          fprintf (stderr, "La opción -%c requiere un argumento. \n", optopt);
        else if (isprint (optopt)) 
          fprintf (stderr, "Opción desconocida `-%c'. \n", optopt);
        else 
          fprintf (stderr, "Caracter de opción desconocido `\\x%x'. \n", optopt);
        fclose (fd);
        usage (EXIT_FAILURE);
        break;
        
      case ':':
        fprintf (stderr, "Opción sin argumento. \n");
        break;

      default:
        usage (EXIT_FAILURE);
      }

  for (index = optind; index < argc; index++) 
    {
      printf ("%s no es una opción\n", argv[index]);
      bye (fd, NULL);
      usage (EXIT_FAILURE);
    }
  
  /********* END Manejo de Opciones por cónsola ******/

  if (time == -1)
    time = DEFAULT_TIME;
  
  if (fd != NULL) // extraer información útil de archivo
    urlList  = parseFile(fd);
  else // poner información de directorio en urlList 
    {
      urlList = (URL *) malloc (sizeof (URL));
      memset ((void *) urlList, '\0', sizeof (URL));
      urlList->dir = (char *) malloc (sizeof (char)*2048);
      memset ((void *) urlList->dir, '\0', sizeof (char) * 2048);
      urlList->domain = (char *) malloc (sizeof (char)*1024);
      memset ((void *) urlList->domain, '\0', sizeof (char) * 1024);      
      strcpy (urlList->dir, dir);
      // extracción de dominio en URL
      int i;
      for (i = 7; i < strlen (dir) ; i++)
        if (strncmp ("/", dir + i, 1) == 0)
            break;

      strncpy (urlList->domain, dir + 7, (i-7));
      urlList->next = NULL;
    }
  
  // Resolución DNS
  resolve (urlList);
  
  // opción de directorio y archivo activas
  if (dir_enabled && file_enabled)
    // Chequear si directorio dir existe en archivo
    if (file_lookup(dir, urlList) != 0)  
      {
        fprintf (stderr, "El directorio %s no existe en el archivo %s.\n", 
                 dir, file);
        bye (fd, urlList);
        exit (EXIT_FAILURE);
      }


  /***** BEGIN threads *****/

  int tStatus;
  pthread_t workerPID; // hilo worker
  
  // Preparar argumento para la función del hilo worker

  struct workerInfo *wi = (workerInfo *) malloc (sizeof (workerInfo));
  wi->time = time;
  wi->urlList = urlList;

  tStatus = pthread_create (&workerPID, NULL, worker, (void *) wi);

  // Binding de señal SIGARLM con función SIGALRM_control 
  signal (SIGALRM, SIGALRM_control);  
  alarm (time); // primera llamada al worker
  // Código de hilo principal
  printf ("Comienzo del programa: \n");
  while (true)
    {
      entry  = getchar ();
      switch (entry)
        {
        case 'p':
          puts ("En pausa...");
          while (true)
              if (!taken) 
                {
                  taken = 1; // Bloquear variable goDown
                  goDown = 1;
                  taken = 0;
                  break;
                }
          break;
        case 'c':
          puts ("Continuando...");
          while (true)
            if (!taken)
              {
                taken = 1; // Bloquear variable goDown
                goDown = 0;
                taken = 0;
                break;
              }
          break;
        case 's':
          puts ("Salir...");
          sdown = 1;  // El thread termina
          pthread_join (workerPID, NULL); // Esperar a que thread termine
          bye (fd, urlList);
          free (wi);
          hdestroy ();
          sleep (2);
          exit (EXIT_SUCESS);
        default:
          if (entry != '\n')
            printf ("Comando inválido. Lista de comandos disponibles: \n\
    p: pausar \n\
    c: continuar \n\
    s: salir \n");
        }
    }

  /***** END threads *****/
  return 0;
}




