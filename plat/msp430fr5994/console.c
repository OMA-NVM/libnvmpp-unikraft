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

#include <inttypes.h>
#include <string.h>
#include <uk/plat/console.h>
#include <uk/arch/lcpu.h>
#include <uk/assert.h>
#include <msp430.h>

void _libmsp430fr5994plat_init_console(void) {
    // Configure Ports
    P2SEL1 |= (BIT0 | BIT1);                // keep everything, put Bit0 and Bit1 to 1. P2SEL1, Pin2 Channel 0 = UART, Channel 1 = UART
    P2SEL0 &= ~(BIT0 | BIT1);               // keep everything, put Bit0 and Bit1 to 0. P2SEL0, Pin2 Channel 0 = UART, Channel 1 = UART
    PM5CTL0 &= ~LOCKLPM5;                   // turn on I/O
    
    // Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers
    
    // Setup UART
    UCA0CTLW0 |= UCSWRST;                   // put UART in reset
    UCA0CTLW0 |= UCSSEL__SMCLK;             // Choose SMCLK for UART A0
    UCA0BRW = 52;                           // set Prescaler
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
    UCA0CTLW0 &= ~UCSWRST;                  // enable UART
	uk_pr_info("UART initialized\n");

    // Enable Interrupts only for receiving
    UCA0IE = UCRXIE;
}

void _libmsp430fr5994plat_send_char(char c) {
    // Wait for previous action to be finished
    while (UCA0STATW & (0b1))
        ;
    UCA0TXBUF = c;
}

int ukplat_coutd(const char *str, unsigned int len)
{
	int ret = 0;
    // STDERR does not exist on UART, so just use normal output
    // and prefix with D:
    _libmsp430fr5994plat_send_char('D');
    _libmsp430fr5994plat_send_char(':');
    _libmsp430fr5994plat_send_char(' ');
    for (unsigned int i = 0; i < len; i++) {
      _libmsp430fr5994plat_send_char(*(str + i));
      ret++;
    }
	return ret;
}

int ukplat_coutk(const char *str, unsigned int len)
{
	int ret = 0;
    for (unsigned int i = 0; i < len; i++) {
      _libmsp430fr5994plat_send_char(*(str + i));
      ret++;
    }
	return ret;
}

int ukplat_cink(char *str, unsigned int maxlen)
{
	size_t ret = 0;
    const char *retstr = "Stdin not implemented yet :)";
    if ( strlen(retstr) < maxlen ) {
        memcpy( str, retstr, strlen (retstr) + 1 );
    }

	return (int) ret;
}
