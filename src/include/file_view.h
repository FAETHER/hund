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

#ifndef FILE_VIEW_H
#define FILE_VIEW_H

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include <panel.h>
#include <dirent.h>
#include <fcntl.h> // File control
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <syslog.h>

#include "path.h"

struct file_record {
	char* file_name;
	unsigned char t;
	struct stat s;
};

struct file_view {
	char wd[PATH_MAX];
	struct file_record** file_list;
	int num_files;
	int selection;
	int view_offset;
	bool focused;

	PANEL* pan;

	int position_x;
	int position_y;
	int width;
	int height;
};

void get_cwd(char[PATH_MAX]);
struct passwd* get_pwd(void);
void scan_wd(char*, struct file_record***, int*);
void delete_file_list(struct file_record***, int);

void file_view_pair_setup(struct file_view[2], int, int);
void file_view_pair_delete(struct file_view[2]);

void file_view_pair_update_geometry(struct file_view[2]);
void file_view_redraw(struct file_view*);
#endif
