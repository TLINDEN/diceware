#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define  DICTFILE "/usr/share/dict/american-english"
#define  VERSION  "1.0"
#define  RLEN     1024

int humantoss = 0;
int WMIN      = 5;
int WMAX      = 10;

int usage() {
  fprintf(stderr,
	  "Generate a random diceware passphrase\n"
	  "Usage: dice [-cfvh]\n"
	  "Options: \n"
	  "-t --humantoss            Asks interactively for tossed dices\n"
	  "-c --wordcount <count>    Number of words (default: 4)\n"
	  "-f --dictfile <dictfile>  Dictionary file to use (default:\n"
	  "                          /usr/share/dict/american-english)\n"
	  "-l --minlen <count>       Minimum word len (default: 5)\n"
	  "-m --maxlen <count>       Maximum word len (default: 10)\n"
	  "-v --version              Print program version\n"
	  "-h -? --help              Print this help screen\n"
	  );
  return 1;
}

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
 
    if((RAND = fopen("/dev/urandom", "rb")) == NULL) {
      perror("Could not open /dev/urandom");
    }

    fread(rand, RLEN, 1, RAND);
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
  }

  words = malloc(66666 * sizeof(char *));
  digits = malloc(5);
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
      //fprintf(stdout, "%d: %s\n", pos, line);
      
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
    tosses = toss(5);

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

int main (int argc, char **argv)  {
  int count = 4;
  char *dictfile = NULL;
  unsigned char *tosses = NULL;
  int opt;
  
  static struct option longopts[] = {
    { "wordcount", required_argument, NULL,           'c' },
    { "minlen",    required_argument, NULL,           'l' },
    { "maxlen",    required_argument, NULL,           'm' },
    { "humantoss", required_argument, NULL,           't' },    
    { "dictfile",  required_argument, NULL,           'f' },
    { "version",   no_argument,       NULL,           'v' },
    { "help",      no_argument,       NULL,           'h' },
  };

   while ((opt = getopt_long(argc, argv, "l:m:tf:c:vh?", longopts, NULL)) != -1) {
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
     dictfile = DICTFILE;
   }
   
   getwords(dictfile, count);

   return 0;
}
