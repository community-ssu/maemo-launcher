/*
 * Copyright © 2005 Nokia Corporation
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

#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "report.h"
#include "invokelib.h"


int
invoke_raw_read(int fd, void* buffer, uint32_t size)
{
  uint32_t cnt = size;
  char*    buf = (char*)buffer;

  /* check buffer and size */
  if (NULL == buffer || 0 == size)
     return EINVAL;

   /* load message in several iterations */
   while (cnt > 0)
   {
      const ssize_t result = read(fd, buf, cnt);

      if (result > 0)
      {
         buf += result;
         cnt -= result;
      }
      else
      {
         /* prevent rubish in data */
         memset(buf, 0, cnt);
         return errno;
      }
   }

   return 0;
} /* invoke_raw_read */



bool
invoke_send_msg(int fd, uint32_t msg)
{
  debug("%s: %08x\n", __FUNCTION__, msg);
  return (sizeof(msg) == write(fd, &msg, sizeof(msg)));
}

bool
invoke_recv_msg(int fd, uint32_t *msg)
{
  const int result = invoke_raw_read(fd, msg, sizeof(*msg));
  debug("%s: %d %08x\n", __FUNCTION__, result, *msg);
  return (0 == result);
}

bool
invoke_send_str(int fd, char *str)
{
  uint32_t size;

  /* Send size. */
  size = (str && *str ? strlen(str) : 0);
  if (size > INVOKER_MAX_STRING_SIZE)
  {
    error("string size is %u and larger than %u in %s\n", size, INVOKER_MAX_STRING_SIZE, __FUNCTION__);
    return false;
  }

  if ( !invoke_send_msg(fd, size) )
  {
    error("unable to write string size is %u in %s\n", size, __FUNCTION__);
    return false;
  }

  debug("%s: '%s'\n", __FUNCTION__, str);

  /* Send the string if size is non-zero */
  if (size && size != (uint32_t)write(fd, str, size))
  {
    error("unable to write string with size %u in %s\n", size, __FUNCTION__);
    return false;
  }

  return true;
}

char *
invoke_recv_str(int fd)
{
  uint32_t size;
  char *str;

  /* Get the size. */
  if ( !invoke_recv_msg(fd, &size) )
  {
    error("string size read failure in %s\n", __FUNCTION__);
    return NULL;
  }

  if (size > INVOKER_MAX_STRING_SIZE)
  {
    error("string size is %u and larger than %u in %s\n", size, INVOKER_MAX_STRING_SIZE, __FUNCTION__);
    return NULL;
  }

  str = malloc(size + 1);
  if (!str)
  {
    error("mallocing in %s for %u bytes string failed\n", __FUNCTION__, size);
    return NULL;
  }

  /* Get the string if size is non-zero */
  if ( size )
  {
    const int ret = invoke_raw_read(fd, str, size);
    if ( ret )
    {
      error("getting string with %u bytes got error %d\n", size, ret);
      free(str);
      return NULL;
    }
  }
  str[size] = 0;

  debug("%s: '%s'\n", __FUNCTION__, str);

  return str;
}
