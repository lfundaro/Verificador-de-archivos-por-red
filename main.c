
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>


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
#define false 0
#define true 1

int goDown = 0;
int goWork = 0;
int taken = 0;
int sdown = 0;

void 
bye (FILE *fd, URL *urlList)
{
  if (fd)
    fclose (fd);
  if (urlList != NULL)
    free_URL (urlList);
  return;
}

void 
SIGALRM_control ()
{
  while (true)
    {
      if (!taken)
        {
          taken = 1;
          goWork = 1;
          printf (" Hola, soy el hijo\n");
          taken = 0;
          return;
        }
    }
}

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
              alarm (p->time);
            }
        }
    }
  return;
}

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

  urlList  = parseFile(fd);
  
  resolve (urlList);
  
  // opción de directorio y archivo activas
  if (dir_enabled && file_enabled)
    if (file_lookup(dir, urlList) != 0) 
      {
        fprintf (stderr, "El directorio %s no existe en el archivo %s.\n", 
                 dir, file);
        bye (fd, urlList);
        exit (EXIT_FAILURE);
      }


  /***** BEGIN threads *****/

  /* sigset_t mask; */
  /* sigemptyset (&mask); */
  /* sigaddset (&mask, SIGALRM); */
  /* sigaddset (&mask, SIGTERM); */
  int tStatus;
  // Creación de hilo worker
  pthread_t workerPID;
  
  // Preparar argumento para worker

  struct workerInfo *wi = (workerInfo *) malloc (sizeof (workerInfo));
  wi->time = time;
  wi->urlList = urlList;

  tStatus = pthread_create (&workerPID, NULL, worker, (void *) wi);

  signal (SIGALRM, SIGALRM_control);
  alarm (time); // primera llamada al worker
  // Código de hilo principal
  printf ("Comienzo del programa: \n");
  while (true)
    {
      entry  = getchar ();
      puts("");
      switch (entry)
        {
        case 'p':
          puts ("pause the program");
          while (true)
              if (!taken) 
                {
                  taken = 1;
                  goDown = 1;
                  taken = 0;
                  break;
                }
          break;
        case 'c':
          puts ("continue program");
          while (true)
            if (!taken)
              {
                taken = 1;
                goDown = 0;
                taken = 0;
                break;
              }
          break;
        case 's':
          puts ("terminate program");
          sdown = 1;
          pthread_join (workerPID, NULL);
          sleep (2);
          bye (fd, urlList);
          free (wi);
          exit (EXIT_SUCESS);
        default:
          if (entry != '\n')
            puts("Instrucción desconocida");
        }
    }

  /***** END threads *****/


  bye (fd, NULL);
  return 0;

}




