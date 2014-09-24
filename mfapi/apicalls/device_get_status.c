/*
 * Copyright (C) 2013 Bryan Christ <bryan.christ@mediafire.com>
 *               2014 Johannes Schauer <j.schauer@email.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../utils/http.h"
#include "../../utils/json.h"
#include "../mfconn.h"
#include "../apicalls.h"        // IWYU pragma: keep

static int      _decode_device_get_status(mfhttp * conn, void *data);

int mfconn_api_device_get_status(mfconn * conn)
{
    const char     *api_call;
    int             retval;
    mfhttp         *http;

    // char        *rx_buffer;

    if (conn == NULL)
        return -1;

    api_call = mfconn_create_signed_get(conn, 0, "device/get_status.php",
                                        "?response_format=json");

    http = http_create();
    retval = http_get_buf(http, api_call, _decode_device_get_status, NULL);
    http_destroy(http);

    free((void *)api_call);

    return retval;
}

static int _decode_device_get_status(mfhttp * conn, void *data)
{
    json_error_t    error;
    json_t         *root;
    json_t         *node;
    json_t         *device_revision;

    if (data != NULL)
        return -1;

    root = http_parse_buf_json(conn, 0, &error);

    node = json_object_by_path(root, "response");

    device_revision = json_object_get(node, "device_revision");
    if (device_revision != NULL)
        printf("device_revision: %s\n\r", json_string_value(device_revision));

    printf("\n\r");

    if (root != NULL)
        json_decref(root);

    return 0;
}
