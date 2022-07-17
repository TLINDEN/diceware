/*
 *    This file is part of dicepwgen
 *
 *    Copyright (C) 2015-2016 T.v.Dein.
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

#ifndef HAVE_DICE_H
#define  HAVE_DICE_H

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "dictfile.h"
#include "tossing.h"
#include "debug.h"

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

#define  VERSION  "1.2"
#define  RLEN     1024

extern int humantoss;
extern int verbose;
extern int dontjump;
extern int symbols;
extern int WMIN;
extern int WMAX;

int usage();
int main (int argc, char **argv);
void getwords(char *dictfile, int count);

#endif
