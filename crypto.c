/************************************************************************
* crypto.c
* Copyright (C) Lisa Milne 2011 <lisa@ltmnet.com>
*
* crypto.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* crypto.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "md5.h"
#include "crypto.h"

/* get the md5sum of a string */
char* md5(char* str)
{
	char* r = NULL;
	char m[17];
	FILE *f;
	int i;
	unsigned int j;
	char t[5];

	f = fopen(str,"r");
	if (!f)
		return NULL;
	
	r = malloc(sizeof(char)*33);

	memset(r,0,sizeof(char)*33);
	md5_stream(f,(void*)m);

	fclose(f);

	for (i=0; i<16; i++) {
		j = 0;
		j |= (unsigned char)m[i];
		sprintf(t,"%02X",j);
		strcat(r,t);
	}

	return r;
}

