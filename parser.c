#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "entry_list.h"

//Funcion que parsea cada pagina descargada
entry_node*
parser(char** pgs, unsigned short npgs){

  //Lista de entradas a retornar
  entry_node* entries = NULL;
  
  int i; //Variable de iteracion
  int retc = 0; //Para guardar valores de retorno

  for(i = 0; i < npgs; ++i){

    //Parsear pagina.
    //Algunas paginas pueden ser nulas
    //si hubo un error descargandolas
    if(pgs[i] != NULL){
      retc = parse(pgs[i],&entries);
    }
    //printf("parsed html %d\n", i);//(FLAG)

    //Salir si hubo un error
    if(retc){
      exit(1);
    }

    /*
    //concat(temp,entries);
    //free(temp);
      En caso de que quiera hacer esto con threads,
      no quiero q 'parse' cambie, asi q le paso una
      lista temporal y manejo la concurrencia en parser.
    */
  }

  //Liberar memoria
  for(i = 0; i < npgs; ++i){
    free(pgs[i]);
  }
  free(pgs);

  return entries;
}

//Funcion que parsea una pagina
int
parse(char* pg, struct entry_node** list){
  char** base_url = (char**)smalloc(sizeof(char*));
  char** path = (char**)smalloc(sizeof(char*));
  char* pg_ptr = pg;
  (*base_url) = NULL;
  (*path) = NULL;
  parse_url(pg_ptr,path,base_url);

  //Arreglo con desplazamientos de los matches
  //con respecto al comienzo del HTTP
  regmatch_t** matches = (regmatch_t**)smalloc(sizeof(regmatch_t*));
  (*matches) = NULL;

  regex_t** cpattern = (regex_t**)smalloc(sizeof(regex_t*));
  (*cpattern) = NULL;

  //Mientras halla entradas
  while(match_entry(pg_ptr,matches,cpattern)){

    //A~nadir la nueva entrada
    fileEntry e = parse_entry(pg_ptr,(*base_url),(*path),matches);
    *list = add_head(e,*list);
    //   printf("%s\n",e.path);//(FLAG)

    //regex.h devuelve el primer match en el string.
    //Esto mueve el apuntador de la pagina al final de
    //match actual, para que se puedan detectar las
    //otras entradas
    pg_ptr = pg_ptr + (*matches)[0].rm_eo;    

    //printf("%s\n",pg_ptr);//(FLAG)
  }

  //Liberar memoria
  if ((*matches) != NULL){
    free(*matches);
  }
  free(matches);

  if ((*cpattern) != NULL){
    regfree(*cpattern);
    free(*cpattern);
  }
  free(cpattern);

  free(*base_url);
  free(base_url);
  free(*path);
  free(path);

  return 0; //Retornar exito
}

//Funcion que hace match con la ultima entrada del HTML
int
match_entry(char* pg_ptr,regmatch_t** matches,regex_t** cpattern){
  int ret; //Para guardar valores de retorno
  const char* pattern = PATTERN;

  //Alocar los apuntadores que sean nulos
  if((*matches) == NULL){
    (*matches) = (regmatch_t*)smalloc(sizeof(regmatch_t)*(1+REG_NMATCHES));
  }
  if((*cpattern) == NULL){
    (*cpattern) = (regex_t*)smalloc(sizeof(regex_t));
    //Compilar la expresion regular
    ret = regcomp((*cpattern),pattern,REG_EXTENDED);
    handle_regex_errors(ret);
  }

  //Ejecutar la expresion regular
  ret = regexec((*cpattern),pg_ptr,(1+REG_NMATCHES),(*matches),0);
  if(ret == REG_NOMATCH){
    //    printf("Nothing found!!!\n");//(FLAG)
    return 0;//Retornar fracaso
  }
  //printf("Something found!!!\n");//(FLAG)
  handle_regex_errors(ret);

  return 1;//Retornar exito
}

//Funcion que construye un 'struct fileEntry' a partir de una entrada en el HTML
fileEntry
parse_entry(char* pg_ptr, char* base_url, char* path,
	    regmatch_t** matches){
  fileEntry e;
  init_entry(&e);
  
  /***Recuperar nombre***/
  char* name = get_data(REG_FILENAME_POS,pg_ptr,matches);
  if(name == (char*)-1){
    fprintf(stdout,"Error: entrada sin nombre\n");
    exit(EXIT_FAILURE);
  }  

  int entry_path_sz = strlen(path) + strlen(name) + 1;
  char* entry_path = (char*)smalloc(sizeof(char)*(entry_path_sz));
  strcpy(entry_path,path);
  strcat(entry_path,name);
  entry_path[entry_path_sz - 1] = '\0';
  e.path = entry_path;
  if(name != NULL){
    free(name);
  }

  /***Recuperar fecha***/
  char* entry_date = get_data(REG_DATE_POS,pg_ptr,matches);
  if(entry_date == (char*)-1){
    fprintf(stdout,"Error: entrada sin fecha\n");
    exit(EXIT_FAILURE);
  }
  e.date = entry_date;

  /***Recuperar tama~no***/
  char* entry_size = get_data(REG_SIZE_POS,pg_ptr,matches);
  if(entry_date == (char*)-1){
    fprintf(stdout,"Error: entrada sin tama~no\n");
    exit(EXIT_FAILURE);
  }
  e.size = entry_size;

  /***Agregar URL***/
  e.URL = (char*)smalloc(sizeof(char)*(strlen(base_url)+1));//+1 por el byte de terminacion
  strcpy(e.URL,base_url);

  return e;
}

//Funcion que parsea la URL agregada por fetcher al principio de cada HTML
void
parse_url(char* pg_ptr, char** path, char** base_url){
  int url_len = 0;
  int ret;//Para guardar valores de retorno
  char* temp = pg_ptr;
  char* url = NULL;

  //Calcular la longitud del URL
  while( *temp != '\n' ){
    temp += 1;
    url_len += 1;
  }
  url_len += 1;//Un byte extra para EOF

  //Copiar el URL a un string aparte
  url = (char*)smalloc(sizeof(char)*url_len);
  strncpy(url,pg_ptr,url_len);
  url[url_len-1] = '\0';

  //Varibles para las expresiones regulares
  regmatch_t* matches = (regmatch_t*)smalloc(sizeof(regmatch_t)*3);
  regex_t* cpattern = (regex_t*)smalloc(sizeof(regex_t));
  const char* url_pattern = "\(http://[\.a-zA-Z0-9]+\)\([a-zA-Z0-9/~]+\)";
  //const char* url_pattern = "a";
  
  //Compilar la expresion regular
  ret = regcomp(cpattern,url_pattern,REG_EXTENDED);
  handle_regex_errors(ret);

  //Ejecutar la expresion regular
  ret = regexec(cpattern,url,3,matches,0);
  if(ret == REG_NOMATCH){
    fprintf(stdout,"No se encontro URL al principio del HTML\n");
    exit(EXIT_FAILURE);
  }
  handle_regex_errors(ret);

  //Guardar URL y path
  int start = 0,end = 0;
  int size = 0;

  //Guardar URL
  start = matches[1].rm_so;
  end = matches[1].rm_eo;
  size = end - start + 1;
  if((start == -1)||(end == -1)){
    fprintf(stdout,"error en parser: dominio no encontrado\n");
    exit(EXIT_FAILURE);
  }
  (*base_url) = (char*)smalloc(sizeof(char)*size);
  strncpy((*base_url),(url+start),size);
  (*base_url)[size-1] = '\0';

  //Guardar path
  start = matches[2].rm_so;
  end = matches[2].rm_eo;
  size = end - start + 1;
  if((start == -1)||(end == -1)){
    fprintf(stdout,"error en parser: PATH no encontrado\n");
    exit(EXIT_FAILURE);
  }
  (*path) = (char*)smalloc(sizeof(char)*size);
  strncpy((*path),(url+start),size);
  (*path)[size-1] = '\0';

  free(url);
  free(matches);
  regfree(cpattern);
  free(cpattern);

  return;
}

//Funcion que recupera un subcampo de un match de la expresion regular
char*
get_data(int data_pos,char* pg_ptr,regmatch_t** matches){

  char *data = NULL;
  int start = (*matches)[data_pos].rm_so;
  int end = (*matches)[data_pos].rm_eo;
  int data_sz = end-start+1;

  //En caso de que el dato no halla sido encontrado
  if((start == -1)||(end == -1)){
    return (char*)-1;
  }

  data = (char*)smalloc(sizeof(char)*data_sz);
  strncpy(data,(pg_ptr+start),data_sz);
  data[data_sz-1] = '\0';

  return data;
}
