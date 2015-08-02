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

#include "debug.h"

void debug(const char * fmt, ...) {
  va_list ap;
  char *err = NULL;

  if(verbose) {
    va_start(ap, fmt);
  
    if(vasprintf(&err, fmt, ap) >= 0) {
      va_end(ap);
      fprintf(stderr, "DEBUG%d: %s\n", verbose, err);
      free(err);
    }
    else {
      fprintf(stderr, "Could not store debug message %s!\n", fmt);
    }
  }
}
