#include "utils.h"
#include "globals.h"
#define URL_MAX_SIZE 2048

// Extrae las lÃ­neas especificadas en el archivo 
// y almacena la informaciÃ³n en la estructura URL. 
URL *
parseFile (FILE* fd) 
{
  int chars_read = 0;
  int char_size = URL_MAX_SIZE;
  char *input_line = (char *) smalloc (char_size + 1);
  int clines = 0;
  URL *curr;
  URL *head = NULL;
  int i;
  
  // Leer lÃ­nea por lÃ­nea de archivo
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
    {// no hay lÃ­neas en archivo
      free (curr);
      puts ("Archivo vacÃ­o");
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

// Verifica si el directorio especificado por el usuario 
// con la opciÃ³n -d existe dentro del archivo especificado 
// por la opciÃ³n -a
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
  e->dateI = 0;
  e->dt = NODIFF;
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

// Hace una copia del apuntador de la estructura ENTRY 
// que se almacena en la tabla de hash para después 
// liberar dicho apuntador con todas sus subestructuras 
void 
add_eControl (ENTRY *p, ENTRY **eControl)
{
  int i;
  for (i = 0; eControl[i] != NULL; i++);
  if (i <= MAX_ENTRIES)
    eControl[i] = p;
  return;
}

// Función que libera los datos guardados dentro de la 
// tabla de hash.
void
free_ENTRY (ENTRY **eControl)
{
  int i;
  for (i = 0; i <= MAX_ENTRIES && eControl[i] != NULL; i++)
    {
      free (eControl[i]->key);
      free (eControl[i]->data);
      free (eControl[i]);
    }
  return;
}

void*
smalloc(int size){
  void* ptr = malloc(size);

  if((size != 0)&&(ptr == NULL)){
    perror("Error reservando memoria: ");
    exit(EXIT_FAILURE);
  }

  return ptr;
}

int
asciinum_to_int(char c){
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
}

void
http_error_handler(char* http_code, char* url){

  //Recuperar codigo
  int code = 0;
  code += asciinum_to_int(http_code[0])*100;
  code += asciinum_to_int(http_code[1])*10;
  code += asciinum_to_int(http_code[2])*1;

  switch(code){
  case 200:
    //fprintf(stdout,"OK: %s\n",url);
    break;

  case 201:
    //fprintf(stdout,"Creado: %s\n",url);
    break;

  case 202:
    //fprintf(stdout,"Aceptada: %s\n",url);
    break;

  case 203:
    //fprintf(stdout,"Información no autoritativa: %s\n",url);
    break;
    
  case 204:
    //fprintf(stdout,"Sin contenido: %s\n",url);
    break;

  case 205:
    //fprintf(stdout,"Recargar contenido: %s\n",url);
    break;

  case 206:
    //fprintf(stdout,"Contenido parcial: %s\n",url);
    break;

  case 207:
    //fprintf(stdout,"Estado múltiple: %s\n",url);
    break;
    
  case 300:
    fprintf(stdout,"Múltiples opciones: %s\n",url);
    break;
    
  case 301:
    fprintf(stdout,"Movido permanentemente: %s\n",url);
    break;

  case 302:
    fprintf(stdout,"Movido temporalmente: %s\n",url);
    break;

  case 303:
    fprintf(stdout,"Vea otra: %s\n",url);
    break;

  case 304:
    fprintf(stdout,"No modificado: %s\n",url);
    break;

  case 305:
    fprintf(stdout,"Utilice un proxy: %s\n",url);
    break;

  case 306:
    fprintf(stdout,"Cambie de proxy: %s\n",url);
    break;

  case 307:
    fprintf(stdout,"Redirección temporal: %s\n",url);
    break;
    
  case 400:
    fprintf(stdout,"Solicitud incorrecta: %s\n",url);
    break;

  case 401:
    fprintf(stdout,"No autorizado: %s\n",url);
    break;

  case 402:
    fprintf(stdout,"Pago requerido: %s\n",url);
    break;

  case 403:
    fprintf(stdout,"Prohibido: %s\n",url);
    break;

  case 404:
    fprintf(stdout,"No encontrado: %s\n",url);
    break;

  case 405:
    fprintf(stdout,"Método no permitido: %s\n",url);
    break;
    
  case 406:
    fprintf(stdout,"No aceptable: %s\n",url);
    break;

  case 407:
    fprintf(stdout,"Autenticación Proxy requerida: %s\n",url);
    break;
    
  case 408:
    fprintf(stdout,"Tiempo de espera agotado: %s\n",url);
    break;

  case 409:
    fprintf(stdout,"Conflicto: %s\n",url);
    break;

  case 410:
    fprintf(stdout,"Ya no disponible: %s\n",url);
    break;

  case 411:
    fprintf(stdout,"Requiere longitud: %s\n",url);
    break;

  case 412:
    fprintf(stdout,"Falló precondición: %s\n",url);
    break;
    
  case 413:
    fprintf(stdout,"Solicitud demasiado larga: %s\n",url);
    break;

  case 414:
    fprintf(stdout,"URI demasiado larga: %s\n",url);
    break;

  case 415:
    fprintf(stdout,"Tipo de medio no soportado: %s\n",url);
    break;

  case 416:
    fprintf(stdout,"Rango solicitado no disponible: %s\n",url);
    break;

  case 417:
    fprintf(stdout,"Falló expectativa: %s\n",url);
    break;
    
  case 421:
    fprintf(stdout,"Hay muchas conexiones desde esta dirección de internet: %s\n",url);
    break;
    
  case 422:
    fprintf(stdout,"Entidad no procesable: %s\n",url);
    break;

  case 423:
    fprintf(stdout,"Bloqueado: %s\n",url);
    break;

  case 424:
    fprintf(stdout,"Falló dependencia: %s\n",url);
    break;
    
  case 425:
    fprintf(stdout,"Colección sin ordenar: %s\n",url);
    break;
    
  case 449:
    fprintf(stdout,"Reintente con: %s\n",url);
    break;

  case 500:
    fprintf(stdout,"Error interno: %s\n",url);
    break;

  case 501:
    fprintf(stdout,"No implementado: %s\n",url);
    break;

  case 502:
    fprintf(stdout,"Pasarela incorrecta: %s\n",url);
    break;

  case 503:
    fprintf(stdout,"Servicio no disponible: %s\n",url);
    break;

  case 504:
    fprintf(stdout,"Tiempo de espera de la pasarela agotado: %s\n",url);
    break;

  case 505:
    fprintf(stdout,"Versión de HTTP no soportada: %s\n",url);
    break;
    
  case 506:
    fprintf(stdout,"Variante también negocia: %s\n",url);
    break;
    
  case 507:
    fprintf(stdout,"Almacenamiento insuficiente: %s\n",url);
    break;

  case 509:
    fprintf(stdout,"Límite de ancho de banda excedido: %s\n",url);
    break;
    
  case 510:
    fprintf(stdout,"No extendido: %s\n",url);
    break;
  }
}
