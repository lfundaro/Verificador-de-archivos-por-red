#include "parser.h"
#include "entry_list.h"
#include <stdlib.h>
#include <stdio.h>

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
      En caso de que quiera hacer esto con threads,
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

//Funcion que parsea una pagina
int
parse(char* pg, struct entry_node** list){
  char** base_url_ptr = (char**)malloc(sizeof(char*));
  char** path_ptr = (char**)malloc(sizeof(char*));
  (*base_url_ptr) = NULL;
  (*path_ptr) = NULL;
  parse_url(pg,path_ptr,base_url_ptr);
  
  //Arreglo con desplazamientos de los matches
  //con respecto al comienzo del HTTP
  regmatch_t** matches_ptr = (regmatch_t**)malloc(sizeof(regmatch_t*));
  (*matches_ptr) = NULL;

  regex_t** cpattern_ptr = (regex_t**)malloc(sizeof(regex_t*));
  (*cpattern_ptr) = NULL;

  //Mientras halla entradas
  while(match_entry(pg,matches_ptr,cpattern_ptr)){

    //A~nadir la nueva entrada
    entry e = parse_entry(pg,(*base_url_ptr),(*path_ptr),matches_ptr);
    *list = add_head(e,*list);

    //regex.h devuelve el ultimo match en el string.
    //Esto 'corta' el string al principio del match
    //actual para que se puedan detectar otras entradas. 
    pg[(*matches_ptr)[0].rm_so] = '\0';
  }

  //Liberar memoria
  if ((*matches_ptr) != NULL){
    free(*matches_ptr);
  }
  free(matches_ptr);

  if ((*cpattern_ptr) != NULL){
    free(*cpattern_ptr);
  }
  free(cpattern_ptr);
  
  return 0; //Retornar exito
}

//Funcion que hace match con la ultima entrada del HTML
int
match_entry(char* pg,regmatch_t** matches_ptr,regex_t** cpattern_ptr){
  regmatch_t* matches = (*matches_ptr);
  regex_t* cpattern = (*cpattern_ptr);
  int ret; //Para guardar valores de retorno
  const char* pattern = PATTERN;

  //Alocar los apuntadores que sean nulos
  if(matches == NULL){
    matches = (regmatch_t*)malloc(sizeof(regmatch_t)*1+REG_NMATCHES);
  }
  if(cpattern == NULL){
    cpattern = (regex_t*)malloc(sizeof(regex_t));
    //Compilar la expresion regular
    ret = regcomp(cpattern,pattern,REG_EXTENDED);
    handle_regex_errors(ret);
  }

  //Ejecutar la expresion regular
  ret = regexec(cpattern,pg,4,matches,0);
  if(ret == REG_NOMATCH){
    return 0;//Retornar fracaso
  }
  handle_regex_errors(ret);

  return 1;//Retornar exito
}

//Funcion que construye un 'struct entry' a partir de una entrada en el HTML
entry
parse_entry(char* pg, char* base_url, char* path,
	    regmatch_t** matches_ptr){
  entry e;
  init_entry(&e);
  
  /***Recuperar nombre***/
  char* name = get_data(REG_FILENAME_POS,pg,matches_ptr);
  if(name == (char*)-1){
    fprintf(stdout,"Error: entrada sin nombre");
    exit(EXIT_FAILURE);
  }  
  char* entry_path = (char*)malloc(sizeof(char)*(strlen(path)+strlen(name)+1));
  strcat(entry_path,path);
  strcat(entry_path,name);
  e.path = entry_path;
  if(name != NULL){
    free(name);
  }

  /***Recuperar fecha***/
  char* entry_date = get_data(REG_DATE_POS,pg,matches_ptr);
  if(entry_date == (char*)-1){
    fprintf(stdout,"Error: entrada sin fecha");
    exit(EXIT_FAILURE);
  }  
  e.date = entry_path;

  /***Recuperar tama~no***/
  char* entry_size = get_data(REG_SIZE_POS,pg,matches_ptr);
  if(entry_date == (char*)-1){
    fprintf(stdout,"Error: entrada sin tama~no");
    exit(EXIT_FAILURE);
  }  
  e.size = entry_size;

  /***Agregar URL***/
  e.URL = (char*)malloc(sizeof(char)*strlen(base_url));
  strcpy(e.URL,base_url);

  return e;
}

//Funcion que parsea la URL agregada por fetcher al principio de cada HTML
void
parse_url(char* pg, char** path_ptr, char** base_url_ptr){
  int url_len = 0;
  int ret;//Para guardar valores de retorno
  char* temp = pg;
  char* url = NULL;

  //Calcular la longitud del URL
  while( *temp != '\n' ){
    temp += 1;
    url_len += 1;
  }

  //Copiar el URL a un string aparte
  url = (char*)malloc(sizeof(char)*url_len);
  strncpy(url,pg,url_len);
  url[url_len-1] = '\0';

  //Varibles para las expresiones regulares
  regmatch_t* matches = (regmatch_t*)malloc(sizeof(regmatch_t)*3);
  regex_t* cpattern = (regex_t*)malloc(sizeof(regex_t));
  const char* pattern = "http://\\([\\.a-zA-Z0-9]+\\)\\([/a-zA-Z0-9]+\\)";
  
  //Compilar la expresion regular
  ret = regcomp(cpattern,pattern,REG_EXTENDED);
  handle_regex_errors(ret);

  //Ejecutar la expresion regular
  ret = regexec(cpattern,pg,4,matches,0);
  if(ret == REG_NOMATCH){
    fprintf(stdout,"No se encontro URL al principio del HTML");
    exit(EXIT_FAILURE);
  }
  handle_regex_errors(ret);

  //Guardar URL y path
  char* base_url = (*base_url_ptr);
  char* path = (*path_ptr);
  int start = 0,end = 0;
  int size = 0;

  //Guardar URL
  start = matches[1].rm_so;
  end = matches[1].rm_eo;
  size = end - start + 1;
  if((start == -1)||(end == -1)){
    fprintf(stdout,"error en parser: URL no encontrada\n");
    exit(EXIT_FAILURE);
  }
  base_url = (char*)malloc(sizeof(char)*size);
  strncpy(base_url,(pg+start),size);
  base_url[size-1] = '\0';

  //Guardar path
  start = matches[2].rm_so;
  end = matches[2].rm_eo;
  size = end - start + 1;
  if((start == -1)||(end == -1)){
    fprintf(stdout,"error en parser: PATH no encontrado\n");
    exit(EXIT_FAILURE);
  }
  path = (char*)malloc(sizeof(char)*size);
  strncpy(path,(pg+start),size);
  path[size-1] = '\0';
  
  return;
}

//Funcion que recupera un subcampo de un match de la expresion regular
char*
get_data(int data_pos,char* pg,regmatch_t** matches_ptr){

  char *data = NULL;
  int start = (*matches_ptr)[data_pos].rm_so;
  int end = (*matches_ptr)[data_pos].rm_eo;
  int data_sz = end-start+1;

  //En caso de que el dato no halla sido encontrado
  if((start == -1)||(end == -1)){
    return (char*)-1;
  }

  data = (char*)malloc(sizeof(char)*data_sz);
  strncpy(data,(pg+start),data_sz);
  data[data_sz-1] = '\0';

  return data;
}

//Manejar errores de la libreria 'regex.h'
void
handle_regex_errors(int errcode){
  switch(errcode){
  case 0:
    return;

  case REG_BADBR:
    fprintf(stdout,"regex: There was an invalid '\\{...\\}' construct in the regular expression. A valid '\\{...\\}' construct must contain either a single number, or two numbers in increasing order separated by a comma.\n");
    break;

  case REG_BADPAT:
    fprintf(stdout,"regex: There was a syntax error in the regular expression.\n");
    break;

  case REG_BADRPT:
    fprintf(stdout,"regex: A repetition operator such as '?' or '*' appeared in a bad position (with no preceding subexpression to act on).\n");
    break;

  case REG_ECOLLATE:
    fprintf(stdout,"regex: The regular expression referred to an invalid collating element (one not defined in the current locale for string collation). See Locale Categories.\n");
    break;

  case REG_ECTYPE:
    fprintf(stdout,"regex: The regular expression referred to an invalid character class name.\n");
    break;

  case REG_EESCAPE:
    fprintf(stdout,"regex: The regular expression ended with '\'.\n");
    break;

  case REG_ESUBREG:
    fprintf(stdout,"regex: There was an invalid number in the '\\digit' construct.\n");
    break;

  case REG_EBRACK:
    fprintf(stdout,"regex: There were unbalanced square brackets in the regular expression.\n");
    break;

  case REG_EPAREN:
    fprintf(stdout,"regex: An extended regular expression had unbalanced parentheses, or a basic regular expression had unbalanced '\\(' and '\\)'.\n");
    break;

  case REG_EBRACE:
    fprintf(stdout,"regex: The regular expression had unbalanced '\\{' and '\\}'.\n");
    break;

  case REG_ERANGE:
    fprintf(stdout,"regex: One of the endpoints in a range expression was invalid.\n");
    break;

  case REG_ESPACE:
    fprintf(stdout,"regex: regcomp/regexec ran out of memory.\n");
    break;
  }

  exit(EXIT_FAILURE);

  return; 
}
