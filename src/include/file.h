/*
 *  Copyright (C) 2017 by Michał Czarnecki <czarnecky@va.pl>
 *
 *  This file is part of the Hund.
 *
 *  The Hund is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The Hund is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FILE_H
#define FILE_H

// dirent.h and fnctl.h stuff
#define _GNU_SOURCE

#ifndef _XOPEN_SOURCE
	#define _XOPEN_SOURCE // S_ISSOCK
#endif
//#define _XOPEN_SOURCE_EXTENDED

#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <syslog.h>

#include "path.h"
#include "utf8.h"

/* From LSB to MSB, by bit index */
static const char* const mode_bit_meaning[] = {
	"execute/sears by others",
	"write by others",
	"read by others",
	"execute/search by group",
	"write by group",
	"read by group",
	"execute/search by user",
	"write by user",
	"read by user",
	"sticky bit",
	"set group ID on execution",
	"set user ID on execution"
};

typedef unsigned int fnum_t; // Number of Files

enum {
	FAILED_NOT = 0,
	FAILED_NAME = 1<<0,
	FAILED_STAT = 1<<1, // file STAT
	FAILED_LSTAT = 1<<2, // Link-sensitive STAT
	FAILED_LPATH = 1<<3,
};
/* If file is a symlink, l will point stat of the pointed file.
 * Otherwise it will point s.
 */
struct file_record {
	char* file_name;
	char* link_path;
	struct stat s;
	struct stat* l;
	int ff;
};

bool is_lnk(const char*);
bool is_dir(const char*);
bool file_exists(const char*);
int scan_dir(const char*, struct file_record***, fnum_t*);
int link_copy(const char* const, const char* const);
int file_move(const char*, const char*);
int file_remove(const char*);
int file_copy(const char*, const char*);

int dir_make(const char*);
#endif
