/*
 * Copyright(c) 2014 Tim Ruehsen
 *
 * This file is part of libmget.
 *
 * Libmget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Libmget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libmget.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Testing Mget
 *
 * Changelog
 * 02.07.2014  Tim Ruehsen  created
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h> // exit()
#include "libtest.h"

int main(void)
{
	mget_test_url_t urls[]={
		{	.name = "/firstlevel/index.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Main Page</title></head><body><p>A link to a" \
				" <A hreF=\"http://localhost:{{port}}/firstlevel/secondpage.html\">second page</a>." \
				" <a href=\"file://path/file_to_be_ignored.txt\">Unsupported file scheme</a>." \
				"</p></body></html>",
			.headers = {
				"Content-Type: text/html",
			}
		},
		{	.name = "/firstlevel/secondpage.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Second Page</title></head><body><p>A link to a" \
				" <a hRef=\"http://localhost:{{port}}/firstlevel/lowerlevel/thirdpage.html\">third page</a>." \
				"</p></body></html>",
			.headers = {
				"Content-Type: text/html",
			}
		},
		{	.name = "/firstlevel/lowerlevel/thirdpage.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Third Page</title></head><body><p>A link to a" \
				" <A href=\"http://localhost:{{port}}/higherlevelpage.html\">higher level page</a>." \
				"</p></body></html>",
			.headers = {
				"Content-Type: text/html",
			}
		},
		{	.name = "/firstlevel/fourthpage.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Fourth Page</title></head><body><p>" \
				"This page is only linked by the higher level page. Therefore, it should not be downloaded." \
				"</p></body></html>",
			.headers = {
				"Content-Type: text/html",
			}
		},
		{	.name = "/higherlevelpage.html",
			.code = "200 Dontcare",
			.body =
				"<html><head><title>Higher Level Page</title></head><body><p>" \
				"This page is on a higher level in the URL path hierarchy. Therefore, it" \
				"should not be downloaded. Wget should not visit the following link to a" \
				" <a Href=\"http://localhost:{{port}}/firstlevel/fourthpage.html\">fourth page</a>." \
				"</p></body></html>",
			.headers = {
				"Content-Type: text/html",
			}
		},
	};

	// functions won't come back if an error occurs
	mget_test_start_http_server(
		MGET_TEST_RESPONSE_URLS, &urls, countof(urls),
		0);

	// test-i
	mget_test(
		MGET_TEST_OPTIONS, "-np -nH -r",
		MGET_TEST_REQUEST_URL, "firstlevel/",
		MGET_TEST_EXPECTED_ERROR_CODE, 0,
		MGET_TEST_EXPECTED_FILES, &(mget_test_file_t []) {
			{ urls[0].name + 1, urls[0].body },
			{ urls[1].name + 1, urls[1].body },
			{ urls[2].name + 1, urls[2].body },
			{	NULL } },
		0);

	exit(0);
}
