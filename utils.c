#include "utils.h"
#define URL_MAX_SIZE 2048

URL *
parseFile (FILE* fd) 
{
  int chars_read = 0;
  int char_size = URL_MAX_SIZE;
  char *input_line = (char *) malloc (char_size + 1);
  int clines = 0;
  URL *curr = (URL *) malloc (sizeof (URL));
  URL *head = curr;
  int i;
  
  while ((chars_read = getline (&input_line, &char_size, fd)) != -1)
    {
      // eliminar caracter \n 
      input_line[chars_read - 1] = '\0';
      strcpy (curr->dir, input_line);
      // extraer dominio del URL 
      // empezando desde el 7mo caracter http:// <-
      for (i = 7; i < char_size + 1; i++)
        if (strncmp ("/", input_line + i, 1) == 0)
            break;

      strncpy (curr->domain, input_line + 7, (i-7));
      curr->next = (URL *) malloc (sizeof (URL));
      curr = curr->next;
      clines++;
    }
  
  if (clines == 0) 
    {// no hay líneas en archivo
      free (curr);
      puts ("Archivo vacío");
      exit (EXIT_FAILURE);
    }
  free (input_line);
  return head;
}

void 
free_URL (URL *url_list) 
{
  URL *tmp = url_list;
  while (tmp->next != NULL)
    {
      url_list = tmp->next;
      free (tmp);
      tmp = url_list;
    }
  free (tmp);
  return;
}

int 
file_lookup (char *dir, URL *urlList) 
{
  URL *tmp = urlList;
  int cmp = -1;
  while (tmp->next != NULL)
    {
      cmp = strcmp (dir, tmp->dir);
      if (cmp == 0) 
        break;
      tmp = tmp->next;
    }
  return cmp;
}
