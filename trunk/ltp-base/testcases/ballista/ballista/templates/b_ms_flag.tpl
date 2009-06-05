// b_ms_flag.tpl : Ballista Datatype Template for memory sync. flag
// Copyright (C) 1998-2001  Carnegie Mellon University
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

name int b_ms_flag;

parent b_int;

includes
[
 {
   #include "b_int.h"
   #include <sys/mman.h>
 }
]

global_defines
[
 {
 }
]

dials
[
  enum_dial HVAL : ASYNC,SYNC,INVALIDATE,ALL;
]

access
[
  ASYNC
  {
    _theVariable= MS_ASYNC;
  }
  SYNC
  {
    _theVariable= MS_SYNC;
  }
  INVALIDATE
  {
    _theVariable = MS_INVALIDATE;
  }
  ALL
  {
    _theVariable = MS_ASYNC | MS_SYNC | MS_INVALIDATE;
  }
]

commit
[
]

cleanup
[
]
