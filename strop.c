/*-
 * Copyright (c) 2024 Michael Roe
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * strop.c - convert between different ALGOL conventions
 */

/*
 * This program (probably) doesn't correctly handle some special cases:
 *   - What if there are quotation marks inside a comment?
 */

#include <stdio.h>
#include <getopt.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
wchar_t buff[1024];
wchar_t *cp;
int len;
int in_string;
int in_reserved;

  setlocale(LC_ALL, getenv("LANG"));

  while (fgetws(buff, sizeof(buff), stdin) > 0)
  {
    len = wcslen(buff);

    /*
     * Strip <CR><LF> from the end of the line.
     * Note that the file may have DOS, not UNIX, <CR><LF> convention.
     */

    if (buff[len-1] == '\n')
    {
      buff[len-1] = '\0';
      len--;
    }
    if (buff[len-1] == '\r')
    {
      buff[len-1] = '\0';
      len--;
    }

    in_string = 0;
    in_reserved = 0;
    cp = buff;
    while (*cp)
    {
      if (*cp == '`')
      {
        in_string = 1;
      }
      else if (*cp == '\'')
      {
        in_string = 0;
      }

      if (in_reserved)
      {
        if ((*cp == ' ') || (*cp == '\t'))
        {
          in_reserved = 0;
        }
      }
      else
      {
        if ((!in_string) && (*cp >= 'A') && (*cp <= 'Z'))
        {
          wprintf(L"_");
          in_reserved = 1;
        }
      }
      
      putwchar(*cp);
      cp++;
    }
    wprintf(L"\n");
  }

  return 0;
}
