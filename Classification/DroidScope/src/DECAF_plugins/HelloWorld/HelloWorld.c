/**
 * Copyright (C) <2012> <Syracuse System Security (Sycure) Lab>
 *
 * This library is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
**/

/**
 * @author Lok Yan
 * @date Oct 18 2012
 */

#include <sys/time.h>

#include "DECAF_shared/DECAF_main.h"
#include "DECAF_shared/DroidScope/DalvikAPI.h"
#include "DECAF_shared/utils/OutputWrapper.h"
#include "DS_Common.h"
#include "dalvikAPI/DalvikOpcodeTable.h"
#include "dalvikAPI/AndroidHelperFunctions.h"
#include "dalvikAPI/DalvikConstants.h"
#include "dalvikAPI/DalvikPrinter.h"

void hello(Monitor* mon)
{
	DECAF_printf("Hello World!\n");
}

static mon_cmd_t Hello_term_cmds[] = {
  #include "plugin_cmds.h"
  {NULL, NULL, },
};

void hello_cleanup()
{
}

plugin_interface_t Hello_interface;

plugin_interface_t* init_plugin(void)
{
  Hello_interface.mon_cmds = Hello_term_cmds;
  Hello_interface.plugin_cleanup = &hello_cleanup;

  
  //initialize the plugin
  return (&Hello_interface);
}

