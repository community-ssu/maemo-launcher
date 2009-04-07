/*
 * $Id: booster_cpp.c Tue, 07 Apr 2009 09:38:38 +0300 makarhun $
 *
 * Copyright (C) 2008 Nokia Corporation
 *
 * Authors: Janne Karhunen <Janne.Karhunen@nokia.com>
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

extern "C" {

#include <stdio.h>

#include "booster.h"
#include "report.h"

booster_api_t booster_cpp_api;

static booster_state_t
booster_cpp_preinit(int *argc, char ***argv)
{
  debug("booster CPP module preinit (%s)\n", *argv[0]);

  return NULL;
}

static void
booster_cpp_init(const char *progfilename, const booster_state_t state)
{
  debug("booster CPP module init (%s)\n", progfilename);
}

static void
booster_cpp_reload(booster_state_t state)
{
  debug("booster CPP module reload\n");
}

void __init() __attribute__((constructor));

void __init() 
{
  booster_cpp_api.booster_version = BOOSTER_API_VERSION;
  booster_cpp_api.booster_preinit = booster_cpp_preinit;
  booster_cpp_api.booster_init    = booster_cpp_init;
  booster_cpp_api.booster_reload  = booster_cpp_reload;
}

}
