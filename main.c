#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include "data_structures.h"
#include "entry_list.h"
#include "fetcher.h"
#include "parser.h"
#include "differ.h"

#define DEFAULT_TIME 30
#define EXIT_FAILURE 1

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
  /***********Testing***********/
  int ret = 0; //to hold return values
  int i = 0; //iteration variable

  unsigned short naddrs = 1;
  struct addrinfo* addrs = (struct addrinfo*)malloc(sizeof(struct addrinfo)*naddrs);
  for(i = 0; i < naddrs; ++i){
    memset(&(addrs[i]),0,sizeof(struct addrinfo));
  }

  //Initialize hints structure to create addrinfos
  struct addrinfo hints;
  memset(&hints,0,sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  //The protocol is left to the program to decide.
  //Since there aren't any protocols that are actually
  //used for streaming besides TCP, we can leave it
  //at it's default value. The correct behaviour would
  //be to use 'getprotoent'.

  ret = getaddrinfo("159.90.9.202","http",&hints,&addrs);;
  if (ret){
    printf("Error getting info about host");
    exit(1);
  }

  //(BUG) fetcher() needs to know the actual URLs
  //to construct the HTTP packets
  char** pgs = fetcher(addrs,naddrs);
  printf("FETCHER DONE\n");

  entry_node* entries = parser(pgs,naddrs);
  printf("PARSER DONE\n");

  entry_node* diffs = differ(entries);
  printf("DIFFER DONE\n");

  updater(diffs);
  printf("UPDATER DONE\n");

  /***********End of testing**********/

  if (argc == 1)
    usage (EXIT_FAILURE);

  int a, index;
  unsigned int time = -1;
  char dir[42];
  char *file = NULL;
  FILE *fd = NULL;
  
  opterr = 0;

  while ((a = getopt (argc, argv, "t:d:a:")) != -1) 
    switch (a)
      {
      case 't':
        if (optarg != 0)
          time = atoi(optarg);
        break;

      case 'd':
        strcpy (dir, optarg);
        break;

      case 'a':
        file = optarg;
        fd = fopen (file, "r");
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

  if (time == -1)
    time = DEFAULT_TIME; 

  for (index = optind; index < argc; index++) {
    printf ("%s no es una opción\n", argv[index]);
    usage (EXIT_FAILURE);
  }

  if (fd) 
    fclose (fd);   
  
  return 0;
}
