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

#include "dicepwgen.h"

int usage() {
  fprintf(stderr,
	  "Generate a random diceware passphrase\n"
	  "Usage: dice [-tcfvhd]\n"
	  "Options: \n"
	  "-t --humantoss            Asks interactively for rolled dices\n"
	  "-c --wordcount <count>    Number of words (default: 4)\n"
	  "-f --dictfile <dictfile>  Dictionary file to use (default:\n"
	  "                          /usr/share/dict/american-english)\n"
	  "-l --minlen <count>       Minimum word len (default: 5)\n"
	  "-m --maxlen <count>       Maximum word len (default: 10)\n"
	  "-d --debug                Enable debug output\n"
	  "-v --version              Print program version\n"
	  "-h -? --help              Print this help screen\n"
	  );
  return 1;
}

int main (int argc, char **argv)  {
  int count = 4;
  char *dictfile = NULL;
  int opt;

  WMIN = 6;
  WMAX = 10;
  humantoss = 0;
  verbose = 0;
  
  static struct option longopts[] = {
    { "wordcount", required_argument, NULL,           'c' },
    { "minlen",    required_argument, NULL,           'l' },
    { "maxlen",    required_argument, NULL,           'm' },
    { "humantoss", required_argument, NULL,           't' },    
    { "dictfile",  required_argument, NULL,           'f' },
    { "version",   no_argument,       NULL,           'v' },
    { "help",      no_argument,       NULL,           'h' },
    { "debug",     no_argument,       NULL,           'd' },
  };

   while ((opt = getopt_long(argc, argv, "l:m:tf:c:vh?d", longopts, NULL)) != -1) {
     switch (opt) {
     case 'v':
       fprintf(stderr, "This is %s version %s\n", argv[0], VERSION);
       return 1;
       break;
     case 'h':
     case '?':
       return usage();
       break;
     case 'c':
       count = atoi(optarg);
       break;
     case 'l':
       WMIN = atoi(optarg);
       break;
     case 'm':
       WMAX = atoi(optarg);
       break;
     case 't':
       humantoss = 1;
       break;
     case 'd':
       verbose++;
       break;
     case 'f':
       dictfile = malloc(strlen(optarg));
       strncpy(dictfile, optarg, strlen(optarg));
       break;
     default:
      return usage();
      break;
     }
   }

   if(dictfile == NULL) {
     dictfile =  STRINGIZE_VALUE_OF(DICTFILE);
   }

   debug("     using dictfile: %s", dictfile);
   debug("minimum word length: %d", WMIN);
   debug("maximum word length: %d", WMAX);
   if(humantoss)
     debug("user rolls dices");
   else
     debug("program rolls dices");
   
   getwords(dictfile, count);

   return 0;
}

void getwords(char *dictfile, int count) {
  /*
    initiate dice tossing, extract matching number of words
    froom the wordlist and print it.
   */
  char **words;
  int i, pos, one, two, three, four, five;
  int *tossed;
  unsigned char *tosses;
  
  words = fetch_dict(dictfile);

  tossed = malloc(count * sizeof(int));
  
  for(i=0; i<count; i++) {
    tosses = toss(5, i);

    one    = tosses[0] * 10000;
    two    = tosses[1] *  1000;
    three  = tosses[2] *   100;
    four   = tosses[3] *    10;
    five   = tosses[4];
    
    pos = one + two + three + four + five;

    tossed[i] = pos;

    free(tosses);
  }

  for(i=0; i<count; i++) {
    fprintf(stdout, "%s ", words[tossed[i]]);
  }

  fprintf(stdout, "\n");
  
  free(tossed);

  for(i=0; i<6666; i++)
    if(words[i] != NULL)
      free(words[i]);

  free(words);
}
