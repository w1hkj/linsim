// ---------------------------------------------------------------------
// Copyright 2013, Dave Freese W1HKJ  <w1hkj@w1hkj.com>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the
//
//  Free Software Foundation, Inc.
//  51 Franklin Street, Fifth Floor
//  Boston, MA  02110-1301 USA.
//
// ---------------------------------------------------------------------

#ifndef PATHSIMDOC_H
#define PATHSIMDOC_H

enum {OFF, ON};

#include "sim.h"

struct _vals
{
	_vals() {
		AWGN_on = false;
		snrdb = 0;
		p0.active = false;
		p0.offset = 0.0;
		p0.spread = 1.0;
		p1.active = false;
		p1.offset = 0.0;
		p1.spread = 1.0;
		p2.active = false;
		p2.offset = 0.0;
		p2.spread = 1.0;
		d.delay1 = 0.0;
		d.delay2 = 0.0;
		state = OFF;
	}

	bool	AWGN_on;
	double	snrdb;

	PATH_INFO p0;
	PATH_INFO p1;
	PATH_INFO p2;
	DELAY_INFO d;

	int		state;
};

extern _vals sim_vals;

#endif
