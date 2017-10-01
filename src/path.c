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

#include "include/path.h"

void get_cwd(char b[PATH_MAX]) {
	getcwd(b, PATH_MAX);
}

struct passwd* get_pwd(void) {
	return getpwuid(geteuid());
}

/* path[] must be absolute and not prettified
 * dir[] does not have to be single file, can be a path
 * Return values:
 * 0 if operation successful
 * -1 if PATH_MAX would be exceeded; path left unchanged
 *
 * I couldn't find any standard function that would parse path and shorten it.
 */
int enter_dir(char path[PATH_MAX], char dir[PATH_MAX]) {
	if (!path_is_relative(dir)) {
		strcpy(path, dir);
		return 0;
	}
	const size_t plen = strlen(path);
	/* path[] may contain '/' at the end
	 * enter_dir appends entries with '/' PREpended;
	 * path[] = '/a/b/c/' -> '/a/b/c'
	 * if dir[] contains 'd....' it appends "/d"
	 * If path ends with /, it would be doubled later
	 */
	if (path[plen-1] == '/' && plen > 1) {
		path[plen-1] = 0;
	}
	char* save_ptr = NULL;
	char* entry = strtok_r(dir, "/", &save_ptr);
	while (entry != NULL) {
		if (strcmp(entry, ".") == 0) {
			goto next_entry;
		}
		else if (strcmp(entry, "..") == 0) {
			char* p = path + strlen(path);
			// At this point path never ends with /
			// p points null pointer
			// Go back till nearest /
			while (*p != '/' && p != path) {
				*p = 0;
				p -= 1;
			}
			// loop stopped at /, which must be deleted too
			*p = 0;
		}
		else {
			// Check if PATH_MAX is respected
			if (strlen(path) + strlen(entry) < PATH_MAX) {
				if (path[0] == '/' && strlen(path) > 1) {
					// dont prepend / in root directory
					strcat(path, "/");
				}
				strcat(path, entry);
			}
			else {
				return -1;
			}
		}
		next_entry:
		entry = strtok_r(NULL, "/", &save_ptr);
	}
	return 0;
}

/* Basically removes everything after last '/', including that '/'
 * Return values:
 * 0 if operation succesful
 * -1 if path == '/'
 */
int up_dir(char path[PATH_MAX]) {
	if (strcmp(path, "/") == 0) return -1;
	int i;
	for (i = strlen(path); i > 0 && path[i] != '/'; i--) {
		path[i] = 0;
	}
	// At this point i points that last '/'
	// But if it's root, don't delete it
	if (i != 0) {
		path[i] = 0;
	}
	return 0;
}

/* Finds substring home in path and changes it to ~
 * /home/user/.config becomes ~/.config
 * Return values:
 * 0 if found home in path and changed
 * -1 if not found home in path; path unchanged
 */
int prettify_path(char path[PATH_MAX], char home[PATH_MAX]) {
	const int hlen = strlen(home);
	const int plen = strlen(path);
	if (memcmp(path, home, hlen) == 0) {
		path[0] = '~';
		memmove(path+1, path+hlen, plen-hlen+1);
		return 0;
	}
	return -1;
}

void current_dir(char path[PATH_MAX], char dir[NAME_MAX]) {
	const int plen = strlen(path);
	int lastslash = 0;
	for (int i = plen-1; i >= 0; i--) {
		if (path[i] == '/') {
			lastslash = i;
			break;
		}
	}
	if (lastslash == 0 && plen == 1) {
		memcpy(dir, "/", 2);
		return;
	}
	memcpy(dir, path+lastslash+1, strlen(path+lastslash));
}

bool path_is_relative(char path[PATH_MAX]) {
	return (path[0] != '/' && path[0] != '~') || (path[0] == '.' && path[1] == '/');
}