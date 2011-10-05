#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "sockutil.h"

void die(char *message) {
  perror(message);
  exit(1);
}

void copyData(int from, int to) {
  char buf[1024];
  int amount;

 
  while ((amount = read(from, buf, sizeof(buf))) > 0){
    if (write(to, buf, amount) != amount) {
      die("write");
      return;
    }
  }
  
  if (amount < 0) die("read");



}
