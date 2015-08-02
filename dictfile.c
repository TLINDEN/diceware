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

#include "dictfile.h"

int *incr_dicedigit(int *digits) {
  /*
    increment an array of dice digits, we expect the first to
    be a multiple of 10000, the 2nd a multiple of 1000 and so on.
  */

  if(digits[4] == 6) {
    digits[4] = 1;
    if(digits[3] == 60) {
      digits[3] = 10;
      if(digits[2] == 600) {
	digits[2] = 100;
	if(digits[1] == 6000) {
	  digits[1] = 1000;
	  digits[0] += 10000; /* may overflow to 71111, must be catched by caller */
	}
	else
	  digits[1] += 1000;
      }
      else
	digits[2] += 100;
    }
    else
      digits[3] += 10;
  }
  else
    digits[4]++;

  return digits;
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

char **fetch_dict(char *dictfile) {
  /*
    read in the dictionary file. we generate an array of max
    66666 entries from the dictionary, each entry's index will
    be a dice number (1 <=> 6). to enhance randomness, we jump
    over a number of lines (1-32 lines) and start from the beginning
    of the file if we reach the end before our array is full.
  */

  char **words;
  int pos, i, next, jump;
  char *line = NULL;
  size_t len = 0;
  ssize_t linelen;
  FILE *DICT;
  int *digits;

  if((DICT = fopen(dictfile, "rb")) == NULL) {
    perror("Could not open dictfile");
    exit(1);
  }

  words = malloc(66666 * sizeof(char *));
  digits = malloc(5 * sizeof(int));
  jump = rand_lim(32);
  
  digits[0] = 10000;
  digits[1] =  1000;
  digits[2] =   100;
  digits[3] =    10;
  digits[4] =     1;

  pos   = 11111;
  next = 0;

  for(i=0; i<6666; i++)
    words[i] = NULL;

  
 LOOP:
  while ((linelen = getline(&line, &len, DICT)) != -1) {
    if(jump > 0) {
      jump--;
      continue;
    }
    else {
      jump = rand_lim(32);
    }
    if(linelen >= WMIN+1 && linelen <= WMAX+1) {
      line[linelen-1] = '\0'; /* remove newline */

      for(i=0; i<linelen-1; i++) {
	if(isalpha((int)line[i]) == 0) {
	  next = 1;
	  break;
	}
      }

      if(next) {
	next = 0;
	continue;
      }

      words[pos] = malloc(linelen);
      strncpy( words[pos], line, linelen);
      if(verbose > 1)
	debug("add to wordlist at index %d: %s", pos, line);

      digits = incr_dicedigit(digits);
      pos = get_dicenum(digits);

      /* this is what pos gets next after 66666, which is max reachable with 5 dices */
      if(pos == 71111)
	break;
    } /* endif word 4<=>10 */
  } /* while read */

  if(pos < 66666) {
    fseek(DICT, 0L, SEEK_SET);
    goto LOOP;
  }
  
  fclose(DICT);
  free(line);
  free(digits);

  return words;
}
