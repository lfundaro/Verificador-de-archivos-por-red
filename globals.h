#ifndef GLOBALS_H
#define GLOBALS_H

#define DEFAULT_TIME 30
#define EXIT_FAILURE 1
#define EXIT_SUCESS 0
#define false 0
#define true 1

#define REG_FILENAME_POS 1
#define REG_DATE_POS 2
#define REG_SIZE_POS 3
#define REG_NMATCHES 3

//Para la laptop de German
//#define PATTERN "<a href=\"\([^\"]+\)\">[^<>]+</a>/</td><td[^<>]+>\([1-2][0-9][0-9][0-9]-[A-Z][a-z][a-z]-[0-9][0-9] [0-2][0-9]:[0-5][0-9]:[0-5][0-9]\)</td><td[^<>]+>\([0-9]+\.[0-9]+\)"

//Para cancer
//#define PATTERN "<a href=\"\([^\"]+\)\">[^<>]+</a>/?</td><td[^<>]+>\([1-2][0-9][0-9][0-9]-[A-Z][a-z][a-z]-[0-9][0-9] [0-2][0-9]:[0-5][0-9]:[0-5][0-9]\)</td><td[^<>]+>\([0-9]+\.[0-9]+\)"

//Para el public_html del prof.Ricardo
#define PATTERN "<a href=\"\([^\"]+\)\">[^<>]+</a>/?[ \t]+\([1-2][0-9]-[A-Z][a-z][a-z]-[1-2][0-9][0-9][0-9] [0-2][0-9]:[0-5][0-9]\)[ \t]+\(-*[0-9]*\)"

#endif
