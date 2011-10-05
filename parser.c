#include "parser.h"
#include "entry_list.h"
#include <stdlib.h>
#include <stdio.h>

//Funcion que parsea una pagina
int
parse(char* pg, struct entry_node** list){
  //Build dummy entry
  entry e; e.name = NULL; e.URL = NULL;
  e.date = 0; e.dt = NEW;

  //Add new entry to the list
  *list = add_head(e,*list);
  
  return 0; //return success
}

//Funcion que parsea cada pagina descargada
entry_node*
parser(char** pgs, unsigned short npgs){

  //Lista de entradas de los directorios
  entry_node* entries = NULL;
  
  int i; //Variable de iteracion

  for(i = 0; i < npgs; ++i){

    //Parsear pagina
    int retc = parse(pgs[i],&entries);
    printf("parsed entry %d\n", i);

    //Salir si hubo un error
    if(retc){
      exit(1);
    }

    /*
      En case de que quiera hacer esto con threads,
      no quiero q 'parse' cambie, asi q le paso una
      lista temporal y manejo la concurrencia en parser.
    */
    //concat(temp,entries);
    //free(temp);
  }

  //Liberar memoria
  for(i = 0; i < npgs; ++i){
    free(pgs[i]);
  }
  free(pgs);

  return entries;
}
