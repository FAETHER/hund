/*
 *  Copyright (C) 2017-2018 by Michał Czarnecki <czarnecky@va.pl>
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

#include "fs.h"
#include "utf8.h"

static const char compare_values[] = "nstdpx";
#define FV_ORDER_SIZE (sizeof(compare_values)-1)
enum compare {
	CMP_NAME = 'n',
	CMP_SIZE = 's',
	CMP_DATE = 't',
	CMP_ISDIR = 'd',
	CMP_PERM = 'p',
	CMP_ISEXE = 'x',
};
static const char default_order[FV_ORDER_SIZE] = {
	CMP_NAME,
	CMP_ISEXE,
	CMP_ISDIR
};

struct file_view {
	char wd[PATH_MAX+1];
	struct file_record** file_list;
	fnum_t num_files;
	fnum_t num_hidden;
	fnum_t selection;
	fnum_t num_selected;
	int scending; // 1 = ascending, -1 = descending
	char order[FV_ORDER_SIZE];
	bool show_hidden;
};

bool visible(const struct file_view* const, const fnum_t);
struct file_record* hfr(const struct file_view* const);

void first_entry(struct file_view* const);
void last_entry(struct file_view* const);

void jump_n_entries(struct file_view* const, const int);

void delete_file_list(struct file_view* const);
fnum_t file_on_list(const struct file_view* const, const char* const);
void file_highlight(struct file_view* const, const char* const);

bool file_find(struct file_view* const, const char* const, fnum_t, fnum_t);

bool file_view_select_file(struct file_view* const);
int file_view_enter_selected_dir(struct file_view* const);
int file_view_up_dir(struct file_view* const);

void file_view_toggle_hidden(struct file_view* const);

int file_view_scan_dir(struct file_view* const);
void file_view_sort(struct file_view* const);

char* file_view_path_to_selected(struct file_view* const);

void file_view_sorting_changed(struct file_view* const);

void file_view_selected_to_list(struct file_view* const,
		struct string_list* const);

void select_from_list(struct file_view* const,
		const struct string_list* const);

/*
 * TODO find a better name
 */
struct assign {
	fnum_t from, to;
};

bool rename_prepare(const struct file_view* const, struct string_list* const,
		struct string_list* const, struct string_list* const,
		struct assign** const, fnum_t* const);

bool conflicts_with_existing(struct file_view* const,
		const struct string_list* const);

void remove_conflicting(struct file_view* const, struct string_list* const);
#endif