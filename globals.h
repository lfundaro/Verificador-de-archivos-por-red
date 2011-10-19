#ifndef GLOBALS_H
#define GLOBALS_H
/** \file */

/** Tiempo por defecto entre revisiones */
#define DEFAULT_TIME 30

/** Código estándar de salida con error */
#define EXIT_FAILURE 1

/** Código estándar de salida exitosa*/
#define EXIT_SUCESS 0

/** Macro para 'falso' */
#define false 0

/** Macro para 'verdadero' */
#define true 1

/** Posición de la subexpresión que contiene el nombre del archivo */
#define REG_FILENAME_POS 1

/** Posición de la subexpresión que contiene la fecha de modificación\
 del archivo */
#define REG_DATE_POS 2

/** Posición de la subexpresión que contiene el tamaño del archivo */
#define REG_SIZE_POS 3

/** Número de subexpresiones en la expresión regular para una entrada */
#define REG_NMATCHES 3

/** Número de posiciones en la tabla de hash donde se guardan las entradas*/
#define MAX_ENTRIES 1000

/***/
//Para cancer
/* #define PATTERN "\ */
/* <a href=\"\([^\"]+\)\">[^<>]+</a>/?</td><td[^<>]+>\ */
/* \([1-2][0-9][0-9][0-9]-[A-Z][a-z][a-z]-[0-9][0-9] \ */
/* [0-2][0-9]:[0-5][0-9]:[0-5][0-9]\)</td><td[^<>]+>\ */
/* \([0-9]*\.?[0-9]*-?\)" */

//Para la laptop de German
/* #define PATTERN "<a href=\"\([^\"]+\)\">[^<>]+</a>/?</td>\ */
/* <td[^<>]+>\([1-2][0-9][0-9][0-9]-[A-Z][a-z][a-z]-[0-9][0-9] \ */
/* [0-2][0-9]:[0-5][0-9]:[0-5][0-9]\)</td><td[^<>]+>\([0-9]+\.[0-9]+\)" */

/** Expresión regular que hace match con una entrada de archivo */
#define PATTERN "<a href=\"\([^\"]+\)\">[^<>]+</a>/?\
[ \t]+\([1-2][0-9]-[A-Z][a-z][a-z]-[1-2][0-9][0-9][0-9] \
[0-2][0-9]:[0-5][0-9]\)[ \t]+\(-*[0-9]*\)"
#endif
