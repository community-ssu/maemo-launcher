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

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "report.h"
#include "comm_msg.h"

#define WORD_SIZE	sizeof(uint32_t)
#define WORD_MASK	(~(WORD_SIZE - 1))
#define WORD_ALIGN(x)	(((x) + WORD_SIZE - 1) & WORD_MASK)

comm_msg_t *
comm_msg_new(uint32_t size)
{
  comm_msg_t *msg;

  msg = malloc(sizeof(*msg));
  if (!msg)
  {
    error("allocating comm message\n");
    return NULL;
  }

  msg->used = msg->read = 0;
  msg->size = WORD_ALIGN(size);
  msg->buf = malloc(msg->size);
  if (!msg->buf)
  {
    free(msg);
    error("allocating comm message buffer\n");
    return NULL;
  }

  return msg;
}

bool
comm_msg_destroy(comm_msg_t *msg)
{
  if (msg->buf)
  {
    msg->size = msg->used = msg->read = 0;
    free(msg->buf);
    msg->buf = NULL;
  }

  return true;
}

bool
comm_msg_grow(comm_msg_t *msg, uint32_t need_size)
{
  uint32_t end_size;
  void *p;

  if (msg->size - msg->used >= need_size)
    return true;

  end_size = msg->size + need_size;

  /* Add some more space. */
  end_size += msg->size;

  p = realloc(msg->buf, end_size);
  if (!p)
    return false;

  msg->buf = p;
  memset(msg->buf + msg->used, 0, end_size - msg->used);

  msg->size = end_size;

  return true;
}

bool
comm_msg_reset(comm_msg_t *msg)
{
  msg->used = msg->read = 0;

  return true;
}

static bool
comm_msg_pack_mem(comm_msg_t *msg, const void *buf, uint32_t size)
{
  uint32_t aligned_size = WORD_ALIGN(size);
  uint32_t pad_size = aligned_size - size;

  if (!comm_msg_grow(msg, aligned_size + sizeof(uint32_t)))
    return false;

  /* Pack the size. */
  memcpy(msg->buf + msg->used, &aligned_size, sizeof(uint32_t));
  msg->used += sizeof(uint32_t);

  /* Pack the data. */
  memcpy(msg->buf + msg->used, buf, size);
  msg->used += size;

  if (pad_size)
  {
    /* Add padding, if needed. */
    memset(msg->buf + msg->used, 0, pad_size);
    msg->used += pad_size;
  }

  return true;
}

static const void *
comm_msg_unpack_mem(comm_msg_t *msg, uint32_t *size)
{
  void *mem;
  int old_read = msg->read;
  uint32_t new_size;

  if (msg->read + sizeof(uint32_t) > msg->used)
    return NULL;

  /* Unpack the size. */
  memcpy(&new_size, msg->buf + msg->read, sizeof(uint32_t));
  msg->read += sizeof(uint32_t);

  /* Keep a pointer to the data. */
  mem = msg->buf + msg->read;
  msg->read += new_size;

  if (msg->read > msg->used) {
    msg->read = old_read;
    return NULL;
  }

  *size = new_size;

  return mem;
}

bool
comm_msg_pack_int(comm_msg_t *msg, uint32_t i)
{
  comm_msg_pack_mem(msg, &i, sizeof(i));

  return true;
}

bool
comm_msg_unpack_int(comm_msg_t *msg, uint32_t *i)
{
  uint32_t size, *p;

  p = (uint32_t *)comm_msg_unpack_mem(msg, &size);

  if (size != sizeof(*i))
    return false;

  *i = *p;

  return true;
}

bool
comm_msg_pack_str(comm_msg_t *msg, const char *str)
{
  uint32_t size = strlen(str) + 1;

  comm_msg_pack_mem(msg, str, size);

  return true;
}

bool
comm_msg_unpack_str(comm_msg_t *msg, const char **str_r)
{
  uint32_t size;
  const char *str;

  str = comm_msg_unpack_mem(msg, &size);
  if (!str)
  {
    error("retrieving the string\n");
    return false;
  }

  if ((size - strlen(str)) > WORD_SIZE)
    return false;

  *str_r = str;

  return true;
}

bool
comm_msg_send(int fd, comm_msg_t *msg)
{
  write(fd, &msg->used, sizeof(msg->size));
  write(fd, msg->buf, msg->used);

  debug("%s: %08x\n", __FUNCTION__, msg);

  return true;
}

bool
comm_msg_recv(int fd, comm_msg_t *msg)
{
  uint32_t size;

  read(fd, &size, sizeof(size));

  if (!comm_msg_grow(msg, size))
    return false;

  read(fd, msg->buf, size);
  msg->used = size;

  debug("%s: %08x\n", __FUNCTION__, *msg);

  return true;
}

