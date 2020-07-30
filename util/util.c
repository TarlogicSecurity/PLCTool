/*
  
  Copyright (C) 2013 Gonzalo José Carracedo Carballal
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program.  If not, see
  <http://www.gnu.org/licenses/>

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#include <util/util.h>

#define STRBUILD_BSIZ           16
#define DEBUG_BACKTRACE_NFUNCS  48

int saved_errno = 0;

void errno_save ()
{
  saved_errno = errno;
}

void errno_restore ()
{
  errno = saved_errno;
}

/* Prototipos de funciones estaticas */
void xalloc_die (void);


int
is_asciiz(const char *buf, int lbound, int ubound)
{
  register int i;

  for (i = lbound; i < ubound; i++)
    if (!buf[i])
      return i + 1;
  return 0;
}

/* ESCRIBIRLA */
char*
vstrbuild (const char *fmt, va_list ap)
{
  char *out;
  int size, zeroindex;
  int last;
  va_list copy;
  
  last = 0;
  
  if (fmt != NULL)
  {
    if (!*fmt) /* Yo no hago trabajo extra */
    {
      out = xmalloc (1);
      out[0] = '\0';
      return out;
    }
    
    va_copy (copy, ap);
    size = vsnprintf (NULL, 0, fmt, copy) + 1;
    va_end (copy);
    
    out = xmalloc (size);
    
    va_copy (copy, ap);
    vsnprintf (out, size, fmt, copy);
    va_end (copy);
    
    for(;;)
    {
      
      if ((zeroindex = is_asciiz (out, last, size)) != 0)
        break;
      
      /* Oh, algo ha ocurrido. No hay un cero en ese
	 intervalo. Tenemos que buscarlo */
      
      /* Estamos seguros de que en los intervalos anteriores NO hay
	 cero, as? que buscaremos a partir del size anterior */

      /* Incrementaremos el tama?o en STRBUILD_BSIZ bytes */
      last = size;
      size += STRBUILD_BSIZ;
      
      out = xrealloc (out, size); /* Reasignamos */
      
      va_copy (copy, ap);
      vsnprintf (out, size, fmt, copy);
      va_end (copy);
    }
  }
  else
    out = NULL;
  
  return out;
}


/* Construye una cadena mediante el formato printf y devuelve un
   puntero a la cadena resultado. DEBES liberar tu mismo la salida. */

/* FIXME: Buscar alguna alternativa mas portable */
char*
strbuild (const char *fmt, ...)
{
  char *out;
  va_list ap;

  va_start (ap, fmt);
  out =  vstrbuild (fmt, ap);
  va_end (ap);

  return out;
}

/* Wrapper para malloc que autocomprueba el valor de retorno */
void*
xmalloc (size_t size)
{
  void* m;
  
  m = malloc (size);

  if (m == NULL)
    xalloc_die ();
  
  return m;
}

/* Wrapper para realloc */
void*
xrealloc (void* ptr, size_t new_size)
{
  void* m;
  
  m = realloc (ptr, new_size);
  
  if (m == NULL)
    xalloc_die ();

  return m;
}


/* Wrapper para strdup */
char *
xstrdup (const char *str)
{
  char *ret;

  if (str != NULL)
  {
    ret = xmalloc (strlen (str) + 1);
    strcpy (ret, str);
  }
  else
    ret = NULL;
  
  return ret;
}

/* Cuando nos quedamos sin memoria... */
void
xalloc_die (void)
{
  ERROR ("No memory left (internal)\n");
  abort ();
}

/* Para manipular arrays de punteros */
int
ptr_list_append_check (void ***list, int *count, void *new)
{
  int i;
  void **reallocd_list;
  
  for (i = 0; i < *count; i++)
    if ((*list)[i] == NULL)
      break;
      
  if (i == *count)
  {
    if ((reallocd_list = xrealloc (*list, (1 + *count) * sizeof (void *))) == NULL)
      return -1;
    else
    {
      ++(*count);
      *list = reallocd_list;
    }
  }
    
  (*list)[i] = new;
  
  return i;
}

void
ptr_list_append (void ***list, int *count, void *new)
{
  (void) ptr_list_append_check (list, count, new);
}

int
ptr_list_remove_first (void ***list, int *count, void *ptr)
{
  int i;
  int found;
  
  found = 0;
  
  for (i = 0; i < *count; i++)
    if ((*list)[i] == ptr || ptr == NULL)
    {
      (*list)[i] = NULL;
      found++;
      
      break;
    }
    
  return found;
}


int
ptr_list_remove_all (void ***list, int *count, void *ptr)
{
  int i;
  int found;
  
  found = 0;
  
  for (i = 0; i < *count; i++)
    if ((*list)[i] == ptr || ptr == NULL)
    {
      (*list)[i] = NULL;
      found++;
    }
    
  return found;
}


char *
str_append_char (char* source, char c)
{
  int strsiz;
  char *nptr;

  strsiz = source == NULL ? 1 : strlen (source) + 1;

  nptr = (char *) xrealloc ( (void *) source, strsiz + 1);

  if (nptr == NULL)
    return NULL;

  nptr[strsiz - 1] = c;
  nptr[strsiz]     = '\0';

  return nptr;
}

char *
fread_line (FILE *fp)
{
  char c;
  char *line;
  int buffer_size;
  int n;
  
  line = NULL;

  
  for (buffer_size = n = 0; (c = fgetc (fp)) != EOF; n++)
  {
    if (c == '\r')
    {
      n--;
      continue;
    }
    
    if (c == '\n')
    {
      if (line == NULL)
        line = xstrdup ("");
        
      break;
    }
    
    if (buffer_size < (n + 1))
    {
      if (buffer_size)
      {
        buffer_size <<= 1;
        line = xrealloc (line, buffer_size + 1);
      }
      else
      {
        buffer_size = STRBUILD_BSIZ;
        line = xmalloc (buffer_size + 1);
      }
    }

    line[n] = c;
  }

  if (line != NULL)
    line[n] = '\0';
    
  return line;
}

/* Todo: this is interesting. Export if necessary */

struct strlist *
strlist_new (void)
{
  struct strlist *new;
  
  new = xmalloc (sizeof (struct strlist));
  
  memset (new, 0, sizeof (struct strlist));
  
  return new;
}

void
strlist_append_string (struct strlist *list, const char *string)
{
  ptr_list_append ((void ***) &list->strings_list, &list->strings_count,
    xstrdup (string));
}

void
strlist_walk (struct strlist *list, 
              void *data,
              void (*walk) (const char *, void *))
{
  int i;
  
  for (i = 0; i < list->strings_count; i++)
    if (list->strings_list[i] != NULL)
      (walk) (list->strings_list[i], data);
}

void
strlist_destroy (struct strlist *list)
{
  int i;
  
  for (i = 0; i < list->strings_count; i++)
    if (list->strings_list[i] != NULL)
      free (list->strings_list[i]);
      
  if (list->strings_list != NULL)
    free (list->strings_list);
    
  free (list);
}

int
strlist_have_element (const struct strlist *list, const char *string)
{
  int i;
  
  for (i = 0; i < list->strings_count; i++)
    if (list->strings_list[i] != NULL)
      if (strcmp (list->strings_list[i], string) == 0)
        return 1;
        
  return 0;
}

void
strlist_cat (struct strlist *dest, const struct strlist *list)
{
  int i;
  
  for (i = 0; i < list->strings_count; i++)
    if (list->strings_list[i] != NULL)
      strlist_append_string (dest, list->strings_list[i]);
}

void
strlist_union (struct strlist *dest, const struct strlist *list)
{
  int i;
  
  for (i = 0; i < list->strings_count; i++)
    if (list->strings_list[i] != NULL)
      if (!strlist_have_element (dest, list->strings_list[i]))
        strlist_append_string (dest, list->strings_list[i]);
}

void
strlist_debug (const struct strlist *list)
{
  int i;
  
  for (i = 0; i < list->strings_count; i++)
    if (list->strings_list[i] != NULL)
      fprintf (stderr, "%3d. %s\n", i, list->strings_list[i]);
    else
      fprintf (stderr, "<empty slot>\n");
}


/* 
   Bit layout of returned byte:
   8   4   0
   MMMMDDDDD
*/

void
al_append_argument (arg_list_t* al, const char* arg)
{
  char *ptr;
  char **argl;

  ptr = (char *) xstrdup (arg);

  argl = (char **) xrealloc ( (void *) al->al_argv, sizeof (char *) * (al->al_argc + 1));

  argl[al->al_argc++] = ptr;
  al->al_argv = argl;
}

void 
free_al (arg_list_t* al)
{
  int i;

  for (i = 0; i < al->al_argc; i++)
    free (al->al_argv[i]);

  if (al->al_line != NULL)
    free (al->al_line);
  
  free (al->al_argv);
  free (al);
}


arg_list_t *
__split_command (const char *line, char *separators, int fixed_sep_size)
{
  int i;

  int split_flag;
  int escape_flag;

  char *nptr;
  char *this_argument;
  arg_list_t* arg_info;

  arg_info = (arg_list_t *) xmalloc (sizeof (arg_list_t));

  arg_info->al_argc = 0;
  arg_info->al_argv = NULL;
  arg_info->al_line = NULL;
    
  this_argument = NULL;

  split_flag = 1;
  escape_flag = 0;

  i = 0;

  if (!fixed_sep_size)
    while (strchr (separators, line[i]) && line[i] != '\0')
      i++;

  for (; i < (int) strlen (line); i++)
  {
    if (strchr (separators, line[i]) && split_flag && !escape_flag)
    {
      if (this_argument == NULL)
      {
        if (fixed_sep_size)
          al_append_argument (arg_info, "");
        continue;
      }
      else
      {
        al_append_argument (arg_info, this_argument);

        free (this_argument);
        this_argument = NULL;
      }
    }
    else if (line[i] == '"' && !escape_flag)
      split_flag = !split_flag;
    else if (line[i] == '\\' && !escape_flag)
      escape_flag = 1;
    /* else if (line[i] == '#' && split_flag && !escape_flag)
      break; */
    else
    {
      nptr = str_append_char (this_argument, line[i]);

      if (nptr == NULL)
      {
        free (this_argument);
        free_al (arg_info);
        return NULL;
      }

      this_argument = nptr;
      escape_flag = 0;
    }
  }

  if (this_argument != NULL)
  {
    al_append_argument (arg_info, this_argument);
    free (this_argument);
  }

  return arg_info;
}

arg_list_t * 
csv_split_line (const char *line)
{
  return __split_command (line, ",", 1);
}

arg_list_t *
split_line (const char *line)
{
  return __split_command (line, " ", 0);
}

int
lscanf_huge (const char *fmt, ...)
{
  char *line;
  int result;
  va_list ap;
  
  va_start (ap, fmt);
  
  if ((line = fread_line (stdin)) == NULL)
    result = -1; 
  else
  {
    result = vsscanf (line, fmt, ap);
    free (line);
  }
  
  va_end (ap);
  
  return result;
}


int
lscanf (const char *fmt, ...)
{
  char line[RECOMMENDED_LINE_SIZE];
  int result;
  va_list ap;
  
  va_start (ap, fmt);
  
  if (fgets (line, RECOMMENDED_LINE_SIZE - 1, stdin) == NULL)
    result = -1; 
  else
    result = vsscanf (line, fmt, ap);
  
  va_end (ap);
  
  return result;
}

char *
ltrim (const char *str)
{
  while (*str)
    if (!isspace (*str))
      break;
    else
      str++;
      
  return xstrdup (str);
}

char *
rtrim (const char *str)
{
  char *copy;
  char *tail;
  
  copy = xstrdup (str);
  
  for (tail = copy + strlen (copy) - 1; (unsigned long) copy <= (unsigned long) tail; tail--)
  {
    if (!isspace (*tail))
      break;
    *tail = '\0';
  }
  
  return copy;
}

char *
trim (const char *str)
{
  char *copy;
  char *tail;

  while (*str)
    if (!isspace (*str))
      break;
    else
      str++;
        
  copy = xstrdup (str);
  
  for (tail = copy + strlen (copy) - 1; (unsigned long) copy <= (unsigned long) tail; tail--)
  {
    if (!isspace (*tail))
      break;
    *tail = '\0';
  }
  
  return copy;
}

/* 
   Bit layout of returned byte:
   8   4   0
   MMMMDDDDD
*/
 
unsigned int
yday_to_daymonth (int yday, int year)
{
  int monthdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  int month = 0;
  
  yday--;
  
  if ((year % 4 == 0) && ((!(year % 100 == 0)) || (year % 400 == 0))) 
    monthdays[1] = 29;
  
  while (monthdays[month] <= yday)
  {
    yday -= monthdays[month++];
    if (month == 12)
      return 0;
  }
  
  return yday | (month << 5);
}

char *
get_curr_ctime (void)
{
  time_t now;
  char *text;
  time (&now);
  
  text = ctime (&now);
  
  text[24] = 0;
  
  return text;
}

void
growbuf_init(struct growbuf *buf)
{
  buf->base = NULL;
  buf->size = 0;
  buf->allocation = 0;
}

void
growbuf_reset(struct growbuf *buf)
{
  buf->size = 0;
}

void *
growbuf_alloc(struct growbuf *buf, size_t size)
{
  size_t total_size, allocation;
  void *tmp;

  if ((allocation = buf->allocation) == 0)
    allocation = 1;

  total_size = buf->size + size;

  while (total_size > allocation)
    allocation <<= 1;

  if (allocation != buf->allocation) {
    if ((tmp = realloc(buf->base, allocation)) == NULL)
      return NULL;

    buf->base = tmp;
    buf->allocation = allocation;
  }

  tmp = buf->as_bytes + buf->size;
  buf->size = total_size;

  return tmp;
}

off_t
growbuf_get_offset(const struct growbuf *buf, const void *addr)
{
  off_t result;

  result = (const uint8_t *) addr - buf->as_bytes;

  if (result < 0 || result >= (off_t) buf->size)
    return -1;

  return result;
}

void
growbuf_finalize(struct growbuf *buf)
{
  if (buf->base != NULL)
    free(buf->base);
}
