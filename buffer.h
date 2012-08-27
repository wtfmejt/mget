/*
 * Copyright(c) 2012 Tim Ruehsen
 *
 * This file is part of MGet.
 *
 * Mget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Mget.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Header file for memory/string buffer routines
 *
 * Changelog
 * 22.08.2012  Tim Ruehsen  created
 *
 */

#ifndef _MGET_BUFFER_H
#define _MGET_BUFFER_H

#include <stddef.h>

typedef struct {
	char
		*data; // pointer to internal memory
	size_t
		length; // number of bytes in 'data'
	size_t
		size; // capacity of 'data' (terminating 0 byte doesn't count here)
	unsigned int
		release_data : 1, // 'data' has been malloc'ed and must be freed
		release_buf : 1; // buffer_t structure has been malloc'ed and must be freed
} buffer_t;


buffer_t
	*buffer_init(buffer_t *buf, char *data, size_t size),
	*buffer_alloc(size_t size);
void
	buffer_ensure_capacity(buffer_t *buf, size_t size) NONNULL(1),
	buffer_deinit(buffer_t *buf) NONNULL(1),
	buffer_free(buffer_t **buf) NONNULL(1),
	buffer_free_data(buffer_t *buf) NONNULL(1);
size_t
	buffer_memcpy(buffer_t *buf, const void *data, size_t length) NONNULL(1),
	buffer_memcat(buffer_t *buf, const void *data, size_t length) NONNULL(1),
	buffer_strcpy(buffer_t *buf, const char *s) NONNULL(1),
	buffer_strcat(buffer_t *buf, const char *s) NONNULL(1);


#endif /* _MGET_BUFFER_H */
