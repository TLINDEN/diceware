/*
 *    This file is part of dicepwgen
 *
 *    Copyright (C) 2015 T.v.Dein.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    You can contact me by mail: <tom AT vondein DOT org>.
 */

#include "tossing.h"

unsigned char *toss(int count) {
  /*
    toss <count> dices. if the global humandice is enabled (option -t),
    then ask the human to toss dices and enter the numbers interactively.
    otherwise generate tosses from /dev/random.
   */
  
  FILE *RAND;
  int i;
  int pos = 0;
  uint8_t onedice;
  unsigned char *tosses = NULL;
  unsigned char *rand = NULL;
  size_t len;
  ssize_t linelen;
 
  if(humantoss) {
    char *line = NULL;
    char digit[2];
    digit[1] = '\0';
   
  RETRY:
    fprintf(stderr, "enter 5 digits, each between 1-6\n");
    linelen = getline(&line, &len, stdin);
    tosses = malloc((size_t)count);
    
    if(linelen < 6) /* 5 digits max allowed */
      goto RETRY;

    for(i=0; i<count; i++) { /* only digits allowed */
      if(isdigit(line[i]) == 0)
	goto RETRY;

      digit[0] = line[i];
      onedice = atoi(digit);

      if (onedice < 1 || onedice > 6) /* no dice digit */
	goto RETRY;

      tosses[i] = onedice;
    }
    free(line);
  }
  else {
    rand = malloc(RLEN);
 
    if((RAND = fopen("/dev/urandom", "rb")) == NULL)
      perror("Could not open /dev/urandom");

    if(fread(rand, RLEN, 1, RAND) != 1)
      perror("Could not read from /dev/urandom");
    
    fclose(RAND);

    tosses = malloc((size_t)count);
    
    for(i=0; i<RLEN; i++) {
      onedice = rand[i];
      onedice &= 7; /* only use the last 3 bits, 0-7 */
      onedice %= 6; /* 6 is the max */
      if(onedice >= 1 && onedice <= 6) {
	tosses[pos] = onedice;
	pos++;
      }
      if(pos == count)
	break;
    }
    free(rand);
  }
 
  return tosses;
}

int rand_lim(int limit) {
  /*
    return a random number in the range [0..limit)
   */

    int divisor = RAND_MAX/limit;
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval == limit);

    return retval;
}



int get_dicenum(int *digits) {
  /*
    get the actual number of an array of dice digits
  */
  int i = 0;
  int pos = 0;
  for(i=0; i<5; i++)
    pos += digits[i];

  return pos;
}
