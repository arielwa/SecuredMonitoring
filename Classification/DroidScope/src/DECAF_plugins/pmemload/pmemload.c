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
#include "cpu.h"

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

#if TARGET_PHYS_ADDR_BITS > 32
#define GET_TPHYSADDR(h, l) (((uint64_t)(h) << 32) | (l))
#else
#define GET_TPHYSADDR(h, l) (l)
#endif

void do_physical_memory_load(Monitor *mon, unsigned int valh,
                                    unsigned int vall, int size,
                                    const char *filename)
{
    FILE *f;
    uint32_t l;
    size_t nReadBytes = 0;
    uint8_t buf[1024];
    target_phys_addr_t addr = GET_TPHYSADDR(valh, vall);

    f = fopen(filename, "rb");
    if (!f) {
        monitor_printf(mon, "could not open '%s'\n", filename);
        return;
    }
    if (size == 0)
    {
    	fseek(f, 0, SEEK_END);
    	size = ftell(f);
    	fseek(f, 0, SEEK_SET);
    }
    while (size != 0) {
        l = sizeof(buf);
        if (l > size)
            l = size;
        nReadBytes = fread(buf, 1, l, f);
        cpu_physical_memory_rw(addr, buf, nReadBytes, 1);
        addr += l;
        size -= l;
    }
    fclose(f);
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
