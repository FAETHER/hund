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
#include <unistd.h>
#include <ncurses.h>
#include <panel.h>
#include <linux/limits.h>
#include <dirent.h>
#include <fcntl.h> // File control
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

struct file_view {
	char wd[PATH_MAX];
	WINDOW* win;
	PANEL* pan;
	int position_x;
	int position_y;
	int width;
	int height;
	struct dirent** namelist;
	int num_files;
};

void get_cwd(char[PATH_MAX]);
void scan_wd(char*, struct dirent***, int*); 
void file_view_update_geometry(struct file_view*);
void file_view_refresh(struct file_view*);
void file_view_setup_pair(struct file_view[2], int, int);
void file_view_delete_pair(struct file_view[2]);
#endif
