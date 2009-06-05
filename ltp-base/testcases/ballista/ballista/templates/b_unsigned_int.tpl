// b_unsigned_int.tpl : Ballista Datatype Template for an unsigned integer
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

name Uint b_unsigned_int;

parent b_unsigned_short;

includes
[
 {
   #include "b_unsigned_short.h"
   #include <limits.h>
   #define Uint unsigned int
 }
]

global_defines
[
 {
 }
]

dials
[
  enum_dial HVAL : MAXUINT;
]

access
[
  MAXUINT
    {
       _theVariable= UINT_MAX;
    }
]

commit
[
]

cleanup
[
]
