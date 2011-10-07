
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <signal.h>


#include "data_structures.h"
#include "entry_list.h"
#include "fetcher.h"
#include "parser.h"
#include "differ.h"
#include "utils.h"

#define DEFAULT_TIME 30
#define EXIT_FAILURE 1
#define EXIT_SUCESS 0
#define false 0
#define true 1


void 
mensaje ()
{
  printf (" Hola, soy el hijo\n");
  return;
}

void 
bye (FILE *fd, URL *urlList)
{
  if (fd)
    fclose (fd);
  if (urlList != NULL)
    free_URL (urlList);
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
  /* /\***********Testing***********\/ */
  /* int ret = 0; //to hold return values */
  /* int i = 0; //iteration variable */

  /* unsigned short naddrs = 1; */
  /* struct addrinfo* addrs = (struct addrinfo*)malloc(sizeof(struct addrinfo)*naddrs); */
  /* for(i = 0; i < naddrs; ++i){ */
  /*   memset(&(addrs[i]),0,sizeof(struct addrinfo)); */
  /* } */

  /* //Initialize hints structure to create addrinfos */
  /* struct addrinfo hints; */
  /* memset(&hints,0,sizeof(struct addrinfo)); */
  /* hints.ai_family = AF_INET; */
  /* hints.ai_socktype = SOCK_STREAM; */
  /* //The protocol is left to the program to decide. */
  /* //Since there aren't any protocols that are actually */
  /* //used for streaming besides TCP, we can leave it */
  /* //at it's default value. The correct behaviour would */
  /* //be to use 'getprotoent'. */

  /* ret = getaddrinfo("159.90.9.202","http",&hints,&addrs);; */
  /* if (ret){ */
  /*   printf("Error getting info about host"); */
  /*   exit(1); */
  /* } */

  /* //(BUG) fetcher() needs to know the actual URLs */
  /* //to construct the HTTP packets */
  /* char** pgs = fetcher(addrs,naddrs); */
  /* printf("FETCHER DONE\n"); */

  /* entry_node* entries = parser(pgs,naddrs); */
  /* printf("PARSER DONE\n"); */

  /* entry_node* diffs = differ(entries); */
  /* printf("DIFFER DONE\n"); */

  /* updater(diffs); */
  /* printf("UPDATER DONE\n"); */

  /* /\***********End of testing**********\/ */

  if (argc == 1)
    usage (EXIT_FAILURE);

  int a, index;
  int dir_enabled, file_enabled;
  dir_enabled = file_enabled = false;
  unsigned int time = -1;
  char dir[2048];
  char *file = NULL;
  FILE *fd = NULL;
  URL* urlList;
  char entry;
  pid_t pID; // PID para fetcher
  
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
      bye (fd, urlList);
      usage (EXIT_FAILURE);
    }
  
  /********* END Manejo de Opciones por cónsola ******/

  if (time == -1)
    time = DEFAULT_TIME; 

  urlList  = parseFile(fd);

  // opción de directorio y archivo activas
  if (dir_enabled && file_enabled)
    if (file_lookup(dir, urlList) != 0) 
      {
        fprintf (stderr, "El directorio %s no existe en el archivo %s.\n", 
                 dir, file);
        fclose (fd);
        free_URL (urlList);
        exit (EXIT_FAILURE);
      }
  
  pID = fork();
  
  if (pID == 0) // código del hijo
    {
      // Bind de función a señal SIGALRM
      signal (SIGALRM, mensaje);
      while (true)
        {
          alarm (time);
          pause ();
        }
    }
  else if (pID < 0) // Error al hacer fork
    {
      fprintf (stderr, "No se pudo hacer fork \n");
      bye (fd, urlList);
      exit (EXIT_FAILURE);
    }
  else 
    {
      // Código del padre
      printf ("Comienzo del programa: \n");
      while (true)
        {
          entry = getchar();
          puts("");
          switch (entry)
            {
            case 'p':
              puts ("pause the program");
              break;
            case 'c':
              puts ("continue program");
              break;
            case 's':
              puts ("terminate program");
              bye (fd, urlList);
              sleep (2);
              exit (EXIT_SUCESS);
            default:
              if (entry != '\n')
                puts("Instrucción desconocida");
            }
        }
    }

  bye (fd, urlList);
  /* if (fd)  */
  /*   fclose (fd);    */

  /* free_URL (urlList);   */
  return 0;
}


