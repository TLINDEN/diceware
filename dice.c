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
#define  WMIN     6
#define  WMAX     10

int humantoss = 0;

int usage() {
  fprintf(stderr,
	  "Generate a random diceware passphrase\n"
	  "Usage: dice [-cfvh]\n"
	  "Options: \n"
	  "-t               Asks interactively for tossed dices\n"
	  "-c <count>       Number of words (default: 4)\n"
	  "-f <dictfile>    Dictionary file to use (default:\n"
	  "                 /usr/share/dict/american-english)\n"
	  "-v               Print program version\n"
	  "-h               Print this help screen\n"
	  );
  return 1;
}

void _dump(unsigned char *d, size_t s) {
  size_t i;
  int c;
  for (i=0; i<s; ++i) {
    fprintf(stdout, "%d ", d[i]);
  }
  fprintf(stdout, "\n");
}


unsigned char *toss(int count) {
 FILE *RAND;
 int i;
 int pos = 0;
 uint8_t onedice;
 unsigned char *tosses = malloc((size_t)count);
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

   if(linelen < 6)
     goto RETRY;

   for(i=0; i<count; i++) {
     if(isdigit(line[i]) == 0)
       goto RETRY;

     digit[0] = line[i];
     onedice = atoi(digit);

     if (onedice < 1 || onedice > 6)
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
 
   for(i=0; i<RLEN; i++) {
     onedice = rand[i];
     onedice &= 7;
     onedice %= 6;
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
  /* return a random number in the range [0..limit)
   */

    int divisor = RAND_MAX/limit;
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval == limit);

    return retval;
}

void getwords(char *dictfile, int count) {
  char **words;
  int one, two, three, four, five, pos, i, next, jump;
  char *line = NULL;
  size_t len = 0;
  ssize_t linelen;
  FILE *DICT;
  unsigned char *tosses;
  int *tossed;
  
  one   = 10000;
  two   =  1000;
  three =   100;
  four  =    10;
  five  =     1;

  pos   = 11111;

  words = malloc(66666 * sizeof(char *));
  tossed = malloc(count);
  jump = rand_lim(32);
  
  if((DICT = fopen(dictfile, "rb")) == NULL) {
    perror("Could not open dictfile");
  }

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
      
      if(five == 6) {
	five = 1;
	if(four == 60) {
	  four = 10;
	  if(three == 600) {
	    three = 100;
	    if(two == 6000) {
	      two = 1000;
	      one += 10000;
	    }
	    else
	      two += 1000;
	  }
	  else
	    three += 100;
	}
	else
	  four += 10;
      }
      else
	five++;

      pos = one + two + three + four + five;

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
}

int main (int argc, char **argv)  {
  int count = 4;
  char *dictfile = NULL;
  unsigned char *tosses = NULL;
  int opt;
  
  static struct option longopts[] = {
    { "wordcount", required_argument, NULL,           'c' },
    { "humantoss", required_argument, NULL,           't' },    
    { "dictfile",  required_argument, NULL,           'f' },
    { "version",   no_argument,       NULL,           'v' },
    { "help",      no_argument,       NULL,           'h' },
  };

   while ((opt = getopt_long(argc, argv, "tf:c:vh?", longopts, NULL)) != -1) {
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
   
   //_dump(tosses, (size_t) count);

   getwords(dictfile, count);
   

   return 0;
}
