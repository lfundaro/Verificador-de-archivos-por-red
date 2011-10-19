#include "utils.h"
#include "globals.h"
#define URL_MAX_SIZE 2048

URL *
parseFile (FILE* fd) 
{
  int chars_read = 0;
  int char_size = URL_MAX_SIZE;
  char *input_line = (char *) smalloc (char_size + 1);
  int clines = 0;
  URL *curr = NULL;
  URL *head = NULL;
  int i;
  
  // Leer línea por línea de archivo
  while ((chars_read = getline (&input_line, &char_size, fd)) != -1)
    {
      // Pedir e inicializar memoria para estructuras
      curr = (URL *) smalloc (sizeof (URL));
      memset ((void *) curr, '\0', sizeof (URL));
      curr->dir = (char *) smalloc (sizeof (char)*2048);
      memset ((void *) curr->dir, '\0', sizeof (char) * 2048);
      curr->domain = (char *) smalloc (sizeof (char)*1024);
      memset ((void *) curr->domain, '\0', sizeof (char) * 1024);      
      // eliminar caracter \n 
      input_line[chars_read - 1] = '\0';
      // Copiar string de directorio a estructura URL
      strcpy (curr->dir, input_line);
      // extraer dominio del URL 
      // empezando desde el 7mo caracter http:// <-
      for (i = 7; i < char_size + 1; i++)
        if (strncmp ("/", input_line + i, 1) == 0)
            break;

      strncpy (curr->domain, input_line + 7, (i-7));
      curr->next = head;
      head = curr;
      clines++;
    }
  
  if (clines == 0) 
    {// no hay líneas en archivo
      free (input_line);
      fclose (fd);
      puts ("Archivo vacío");
      exit (EXIT_FAILURE);
    }

  free (input_line);
  return head;
}


// Libera recursos para estructura urlList
void 
free_URL (URL *url_list) 
{
  URL *tmp = url_list;
  URL *aux;
  while (tmp != NULL)
    {
      aux = tmp->next;
      freeaddrinfo (tmp->netInfo);
      free (tmp->domain);
      free (tmp->dir);
      free (tmp);
      tmp = aux;
    }
  return;
}

int 
file_lookup (char *dir, URL *urlList) 
{
  URL *tmp = urlList;
  int cmp = -1;
  // Recorrer lista de directorios
  while (tmp->next != NULL)
    {
      cmp = strcmp (dir, tmp->dir);
      if (cmp == 0) 
        break;
      tmp = tmp->next;
    }
  return cmp;
}

// Inicializa estructura de tipo fileEntry.
void
init_entry(fileEntry* e){
  e->path = NULL;
  e->URL = NULL;
  e->date = NULL; 
  e->size = NULL; 
  e->dt = NODIFF;
}

//Manejar errores de la libreria 'regex.h'
void
handle_regex_errors(int errcode){
  switch(errcode){
  case 0:
    return;

  case REG_BADBR:
    fprintf(stdout,"\
regex: There was an invalid '\\{...\\}' construct in the regular expression. \
A valid '\\{...\\}' construct must contain either a single number, or two \
numbers in increasing order separated by a comma.\n");
    break;

  case REG_BADPAT:
    fprintf(stdout,"\
regex: There was a syntax error in the regular expression.\n");
    break;

  case REG_BADRPT:
    fprintf(stdout,"\
regex: A repetition operator such as '?' or '*' appeared in a bad position \
(with no preceding subexpression to act on).\n");
    break;

  case REG_ECOLLATE:
    fprintf(stdout,"\
regex: The regular expression referred to an invalid collating element \
(one not defined in the current locale for string collation). \
See Locale Categories.\n");
    break;

  case REG_ECTYPE:
    fprintf(stdout,"\
regex: The regular expression referred to an invalid character class name.\n");
    break;

  case REG_EESCAPE:
    fprintf(stdout,"regex: The regular expression ended with '\'.\n");
    break;

  case REG_ESUBREG:
    fprintf(stdout,"\
regex: There was an invalid number in the '\\digit' construct.\n");
    break;

  case REG_EBRACK:
    fprintf(stdout,"\
regex: There were unbalanced square brackets in the regular expression.\n");
    break;

  case REG_EPAREN:
    fprintf(stdout,"\
regex: An extended regular expression had unbalanced parentheses, or a basic \
regular expression had unbalanced '\\(' and '\\)'.\n");
    break;

  case REG_EBRACE:
    fprintf(stdout,"\
regex: The regular expression had unbalanced '\\{' and '\\}'.\n");
    break;

  case REG_ERANGE:
    fprintf(stdout,"\
regex: One of the endpoints in a range expression was invalid.\n");
    break;

  case REG_ESPACE:
    fprintf(stdout,"regex: regcomp/regexec ran out of memory.\n");
    break;
  }

  exit(EXIT_FAILURE);

  return; 
}

void 
add_eControl (ENTRY *p, eControl **controlNodes)
{
  eControl *x = (eControl *) smalloc (sizeof (eControl));
  x->node = p;
  x->next = *controlNodes;
  *controlNodes = x;
  return;
}

void
free_ENTRY (eControl **controlNodes)
{
  eControl *tmp;
  while (*controlNodes)
    {
      tmp = (*controlNodes)->next;
      free ((*controlNodes)->node->key);
      free ((*controlNodes)->node->data);
      free ((*controlNodes)->node);
      free (*controlNodes);
      *controlNodes = tmp;
    }
  return;
}

void*
smalloc(int size){
  void* ptr = malloc(size);

  if((size != 0)&&(ptr == NULL)){
    perror("Error reservando memoria");
    exit(EXIT_FAILURE);
  }

  return ptr;
}

int asciinum_to_int(char* asciinum, int num_length){
  int i = num_length-1;
  int mult = 1;
  int num = 0;

  while(i >= 0){
    num += asciichar_to_int(asciinum[i]) * mult;
    --i;
    mult *= 10;
  }

  return num;
}

int
asciichar_to_int(char c){
  switch(c){
  case '0': return 0;
  case '1': return 1;
  case '2': return 2;
  case '3': return 3;
  case '4': return 4;
  case '5': return 5;
  case '6': return 6;
  case '7': return 7;
  case '8': return 8;
  case '9': return 9;
  }

  return -1;
}

int
http_error_handler(char* http_code, char* url){

  //Recuperar codigo
  int code = 0;
  code += asciichar_to_int(http_code[0])*100;
  code += asciichar_to_int(http_code[1])*10;
  code += asciichar_to_int(http_code[2])*1;

  switch(code){
  case 200:
    //fprintf(stdout,"200 OK: %s\n",url);
    return 0;
    break;

  case 201:
    //fprintf(stdout,"201 Creado: %s\n",url);
    return 0;
    break;

  case 202:
    //fprintf(stdout,"202 Aceptada: %s\n",url);
    return 0;
    break;

  case 203:
    //fprintf(stdout,"203 Información no autoritativa: %s\n",url);
    return 0;
    break;
    
  case 204:
    //fprintf(stdout,"204 Sin contenido: %s\n",url);
    return 0;
    break;

  case 205:
    //fprintf(stdout,"205 Recargar contenido: %s\n",url);
    return 0;
    break;

  case 206:
    //fprintf(stdout,"206 Contenido parcial: %s\n",url);
    return 0;
    break;

  case 207:
    //fprintf(stdout,"207 Estado múltiple: %s\n",url);
    return 0;
    break;
    
  case 300:
    fprintf(stdout,"300 Múltiples opciones: %s\n",url);
    return 1;
    break;
    
  case 301:
    fprintf(stdout,"301 Movido permanentemente: %s\n",url);
    return 1;
    break;

  case 302:
    fprintf(stdout,"302 Movido temporalmente: %s\n",url);
    return 1;
    break;

  case 303:
    fprintf(stdout,"303 Vea otra: %s\n",url);
    return 1;
    break;

  case 304:
    fprintf(stdout,"304 No modificado: %s\n",url);
    return 1;
    break;

  case 305:
    fprintf(stdout,"305 Utilice un proxy: %s\n",url);
    return 1;
    break;

  case 306:
    fprintf(stdout,"306 Cambie de proxy: %s\n",url);
    return 1;
    break;

  case 307:
    fprintf(stdout,"307 Redirección temporal: %s\n",url);
    return 1;
    break;
    
  case 400:
    fprintf(stdout,"400 Solicitud incorrecta: %s\n",url);
    return 1;
    break;

  case 401:
    fprintf(stdout,"401 No autorizado: %s\n",url);
    return 1;
    break;

  case 402:
    fprintf(stdout,"402 Pago requerido: %s\n",url);
    return 1;
    break;

  case 403:
    fprintf(stdout,"403 Prohibido: %s\n",url);
    return 1;
    break;

  case 404:
    fprintf(stdout,"404 No encontrado: %s\n",url);
    return 1;
    break;

  case 405:
    fprintf(stdout,"405 Método no permitido: %s\n",url);
    return 1;
    break;
    
  case 406:
    fprintf(stdout,"406 No aceptable: %s\n",url);
    return 1;
    break;

  case 407:
    fprintf(stdout,"407 Autenticación Proxy requerida: %s\n",url);
    return 1;
    break;
    
  case 408:
    fprintf(stdout,"408 Tiempo de espera agotado: %s\n",url);
    return 1;
    break;

  case 409:
    fprintf(stdout,"409 Conflicto: %s\n",url);
    return 1;
    break;

  case 410:
    fprintf(stdout,"410 Ya no disponible: %s\n",url);
    return 1;
    break;

  case 411:
    fprintf(stdout,"411 Requiere longitud: %s\n",url);
    return 1;
    break;

  case 412:
    fprintf(stdout,"412 Falló precondición: %s\n",url);
    return 1;
    break;
    
  case 413:
    fprintf(stdout,"413 Solicitud demasiado larga: %s\n",url);
    return 1;
    break;

  case 414:
    fprintf(stdout,"414 URI demasiado larga: %s\n",url);
    return 1;
    break;

  case 415:
    fprintf(stdout,"415 Tipo de medio no soportado: %s\n",url);
    return 1;
    break;

  case 416:
    fprintf(stdout,"416 Rango solicitado no disponible: %s\n",url);
    return 1;
    break;

  case 417:
    fprintf(stdout,"417 Falló expectativa: %s\n",url);
    return 1;
    break;
    
  case 421:
    fprintf(stdout,"\
421 Hay muchas conexiones desde esta dirección de internet: %s\n",url);
    return 1;
    break;
    
  case 422:
    fprintf(stdout,"422 Entidad no procesable: %s\n",url);
    return 1;
    break;

  case 423:
    fprintf(stdout,"423 Bloqueado: %s\n",url);
    return 1;
    break;

  case 424:
    fprintf(stdout,"424 Falló dependencia: %s\n",url);
    return 1;
    break;
    
  case 425:
    fprintf(stdout,"425 Colección sin ordenar: %s\n",url);
    return 1;
    break;
    
  case 449:
    fprintf(stdout,"449 Reintente con: %s\n",url);
    return 1;
    break;

  case 500:
    fprintf(stdout,"500 Error interno: %s\n",url);
    return 1;
    break;

  case 501:
    fprintf(stdout,"501 No implementado: %s\n",url);
    return 1;
    break;

  case 502:
    fprintf(stdout,"502 Pasarela incorrecta: %s\n",url);
    return 1;
    break;

  case 503:
    fprintf(stdout,"503 Servicio no disponible: %s\n",url);
    return 1;
    break;

  case 504:
    fprintf(stdout,"504 Tiempo de espera de la pasarela agotado: %s\n",url);
    return 1;
    break;

  case 505:
    fprintf(stdout,"505 Versión de HTTP no soportada: %s\n",url);
    return 1;
    break;
    
  case 506:
    fprintf(stdout,"506 Variante también negocia: %s\n",url);
    return 1;
    break;
    
  case 507:
    fprintf(stdout,"507 Almacenamiento insuficiente: %s\n",url);
    return 1;
    break;

  case 509:
    fprintf(stdout,"509 Límite de ancho de banda excedido: %s\n",url);
    return 1;
    break;
    
  case 510:
    fprintf(stdout,"510 No extendido: %s\n",url);
    return 1;
    break;
  }
}
