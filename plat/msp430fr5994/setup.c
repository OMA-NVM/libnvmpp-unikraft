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

#include <uk/config.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <msp430fr5994/setup.h>
#include <msp430fr5994/console.h>
#include <msp430.h>
#include <uk/plat/console.h>
#include <uk/plat/bootstrap.h>
#include <uk/assert.h>
#include <uk/errptr.h>
#include <uk/plat/common/cpu.h>

void _libmsp430fr5994plat_entry() __noreturn;

extern char __bssstart, __bssend, __bsssize;

void _libmsp430fr5994plat_entry()
{
	/*
	 * Initialize platform console
	 */
	_libmsp430fr5994plat_init_console();

    /*
     * BSS-Segment usually contains zero intialized data, however if 
     * the chip is reset and still keeps memory contents all the nice 
     * code wont work
     */
	uk_pr_info("Resetting BSS (%p - %p)\n", &__bssstart, &__bssend);
	memset((void*) (size_t) &__bssstart, 0, &__bssend - &__bssstart);


	/*
	 * Enter Unikraft
	 */
    // could hand through argc und argp here:
	ukplat_entry(0, 0x0);
}
