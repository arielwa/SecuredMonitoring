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
 * @author Tomer Godinger
 * @date Oct 03 2013
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

#if TARGET_PHYS_ADDR_BITS > 32
#define GET_TPHYSADDR(h, l) (((uint64_t)(h) << 32) | (l))
#else
#define GET_TPHYSADDR(h, l) (l)
#endif

void bgu_hello(Monitor* mon)
{
	DECAF_printf("Hi there! =)\n");
}

/*
 extern int walk_memory_regions(void *priv,
 int (*fn)(void *, unsigned long, unsigned long, unsigned long));

 int bgu_advance_count(void* pCounter, unsigned long dummy1, unsigned long dummy2, unsigned long dummy3)
 {
 if (pCounter)
 {
 (*(unsigned long*)pCounter)++;
 }

 return 0;
 }

 void bgu_print_memory_size()
 {
 unsigned long nPageCount = 0;
 walk_memory_regions(&nPageCount, &bgu_advance_count);
 DECAF_printf("The memory contains %ul pages.\n", nPageCount);
 }
 */

void bgu_do_physical_memory_load(Monitor *mon, unsigned int valh,
		unsigned int vall, int size, const char *filename)
{
	FILE *f;
	uint32_t l;
	size_t nReadBytes = 0;
	uint8_t buf[1024];
	target_phys_addr_t addr = GET_TPHYSADDR(valh, vall);

	f = fopen(filename, "rb");
	if (!f)
	{
		monitor_printf(mon, "could not open '%s'\n", filename);
		return;
	}
	if (size == 0)
	{
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
	}
	while (size != 0)
	{
		l = sizeof(buf);
		if (l > size) l = size;
		nReadBytes = fread(buf, 1, l, f);
		cpu_physical_memory_rw(addr, buf, nReadBytes, 1);
		addr += l;
		size -= l;
	}
	fclose(f);
}

void bgu_do_physical_memory_dump(Monitor *mon, const char *filename)
{
/*	FILE *f;
	uint32_t l;
	uint8_t buf[1024];
	target_phys_addr_t addr = GET_TPHYSADDR(0, 0);
	int size = 0;

	f = fopen(filename, "wb");
	if (!f)
	{
		monitor_printf(mon, "could not open '%s'\n", filename);
		return;
	}
	while (size != 0)
	{
		l = sizeof(buf);
		if (l > size) l = size;
		cpu_physical_memory_rw(addr, buf, l, 0);
		fwrite(buf, 1, l, f);
		fflush(f);
		addr += l;
		size -= l;
	}
	fclose(f);
 	*/
	DECAF_printf("Sorry, this feature is not yet implemented...\n");
}

static mon_cmd_t BGU_term_cmds[] =
{
	#include "plugin_cmds.h"
	{ NULL, NULL , }
};

void bgu_cleanup()
{
}

plugin_interface_t BGU_interface;

plugin_interface_t* init_plugin(void)
{
	BGU_interface.mon_cmds = BGU_term_cmds;
	BGU_interface.plugin_cleanup = &bgu_cleanup;

	//initialize the plugin
	return (&BGU_interface);
}
