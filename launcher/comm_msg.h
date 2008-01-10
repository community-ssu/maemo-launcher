/*
 * $Id$
 *
 * Copyright (C) 2008 Nokia Corporation
 *
 * Author: Guillem Jover <guillem.jover@nokia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifndef COMM_MSG_H
#define COMM_MSG_H

#include <stdint.h>
#include <stdbool.h>

typedef struct comm_msg {
  uint32_t size;
  uint32_t used;
  uint32_t read;
  char *buf;
} comm_msg_t;

comm_msg_t *comm_msg_new(uint32_t size);
bool comm_msg_destroy(comm_msg_t *msg);
bool comm_msg_grow(comm_msg_t *msg);

bool comm_msg_send(int fd, comm_msg_t *msg);
bool comm_msg_recv(int fd, comm_msg_t *msg);

bool comm_msg_pack_int(comm_msg_t *msg, uint32_t i);
bool comm_msg_unpack_int(comm_msg_t *msg, uint32_t *i);

bool comm_msg_pack_str(comm_msg_t *msg, const char *str);
bool comm_msg_unpack_str(comm_msg_t *msg, const char **str);

#endif

