/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Authors: Simon Kuenzer <simon.kuenzer@neclab.eu>
 *
 *
 * Copyright (c) 2017, NEC Europe Ltd., NEC Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THIS HEADER MAY NOT BE EXTRACTED OR MODIFIED IN ANY WAY.
 */

#include <errno.h>
#include <uk/arch/types.h>
#include <uk/errptr.h>
#include <uk/assert.h>
#include <uk/plat/memory.h>
#include <uk/libparam.h>
#include <stdio.h>

#define MB2B		(1024 * 1024)

static struct uk_alloc *plat_allocator;
extern char _my_fram_begin, _my_fram_end;

int ukplat_memregion_count(void)
{
    // one memregion, the heap :)
    return 1;
}

int ukplat_memregion_get(int i __unused, struct ukplat_memregion_desc *m __unused)
{
	m->base  = (void *) &_my_fram_begin;
	m->len   = &_my_fram_end - &_my_fram_begin;
	m->flags = UKPLAT_MEMRF_ALLOCATABLE;
#if CONFIG_UKPLAT_MEMRNAME
	m->name  = "myfram";
#endif
        printf("_my_fram_begin: %p, _my_fram_end: %p\n", &_my_fram_begin, &_my_fram_end);
	return 0;
}

int _ukplat_mem_mappings_init(void)
{
	return 0;
}

void ukplat_stack_set_current_thread(void *thread_addr __unused)
{
	/* For now, signals use the current process stack */
}

int ukplat_memallocator_set(struct uk_alloc *a)
{
	UK_ASSERT(a != NULL);

	if (plat_allocator != NULL)
		return -1;

	plat_allocator = a;

	_ukplat_mem_mappings_init();

	return 0;
}
