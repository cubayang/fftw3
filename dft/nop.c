/*
 * Copyright (c) 2002 Matteo Frigo
 * Copyright (c) 2002 Steven G. Johnson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* $Id: nop.c,v 1.1 2002-06-09 15:01:41 athena Exp $ */

/* plans for vrank -infty DFTs (nothing to do) */

#include "dft.h"

static void apply(plan *ego_, R *ri, R *ii, R *ro, R *io)
{
     UNUSED(ego_);
     UNUSED(ri);
     UNUSED(ii);
     UNUSED(ro);
     UNUSED(io);
}

static int applicable(const solver *ego_, const problem *p_)
{
     UNUSED(ego_);
     if (DFTP(p_)) {
	  const problem_dft *p = (const problem_dft *) p_;
	  return (p->vecsz.rnk == RNK_MINFTY);
     }
     return 0;
}

static void destroy(plan *ego)
{
     fftw_free(ego);
}

static void print(plan *ego, plan_printf prntf)
{
     UNUSED(ego);
     prntf("(dft-nop)");
}

static int score(const solver *ego, const problem *p)
{
     return applicable(ego, p) ? GOOD : BAD;
}

static plan *mkplan(const solver *ego, const problem *p, planner *plnr)
{
     static const plan_adt padt = { 
	  fftw_dft_solve, fftw_null_awake, print, destroy 
     };

     UNUSED(plnr);

     if (!applicable(ego, p))
	  return (plan *) 0;
     return MKPLAN_DFT(plan_dft, &padt, apply);
}

static solver *mksolver(void)
{
     static const solver_adt sadt = { mkplan, score };
     return MKSOLVER(solver, &sadt);
}

void fftw_dft_nop_register(planner *p)
{
     p->adt->register_solver(p, mksolver());
}
