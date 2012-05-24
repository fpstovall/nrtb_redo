/***********************************************
This file is part of the NRTB project (https://launchpad.net/nrtb).

    NRTB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NRTB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NRTB.  If not, see <http://www.gnu.org/licenses/>.

**********************************************/

import std.stdio;

/** Template defining a point or vector in cartesian 3d space. Should work
 ** with all floating point types.
 **
 ** Implements the following operators: +,=,*,/,+=,-=,*=,/= for both
 ** triad<T> and <T> arguments; each returns an triad as the result.
 ** ==,!=,>>,<< are implemented for triad arguments. Additionally
 ** implements pow() with triad and T arguments. Finally implements range()
 ** which returns the distance between the triad and one supplied, and
 ** magnatude() which returns the length of the vector.
 **/

struct vect3d
{
  alias double[3] vec;
  // vector
  private vec coords;
  // constructors
  this(ref this) { coords = coords.dup; }
  this(double[] a)
  {
    assert(a.length==3,"!! Invalid input length in vect3d.(T[] a).");
    coords[] = a[0 .. 3];
  }
  this(double a) { coords[] = a;}
  this(double x, double y, double z) { coords[0]=x; coords[1]=y; coords[2]=z;}
  // vect3d by vect3d operators
  vect3d opBinary(string op)(vect3d rhs)
  {
    vect3d v;
    mixin("v.coords[] = coords[] "~op~" rhs.coords[];");
    return v;
  }
  // vect3d by double operators
  vect3d opBinary(string op)(double rhs)
  {
    vect3d v;
    mixin("v.coords[] = coords[] "~op~" rhs;");
    return v;
  }
  // overload the binary operators for vect3d
}

unittest
{
  // test both literal constuctors
  auto a = vect3d(2);
  assert(a==vect3d(2,2,2));
  // test init from an array.
  double[] array = [2,3,4];
  auto at = vect3d(array);
  assert(at == vect3d(2,3,4));
  // test copying
  auto t=a;
  assert(t==vect3d(2,2,2));
  // test vect3d by vect3d math
  auto b = vect3d(1,2,3);
  t = b + a;
  assert((b+a)==vect3d(3,4,5));
  assert((b-a)==vect3d(-1,0,1));
  assert((b*a)==vect3d(2,4,6));
  assert((b/a)==vect3d(1.0/2.0,2.0/2.0,3.0/2.0));
  // test vect3d by double math shifts and scales
  assert((b+1)==vect3d(2,3,4));
  assert((b-1)==vect3d(0,1,2));
  assert((b*2)==vect3d(2,4,6));
  assert((b/2.0)==vect3d(1.0/2.0,2.0/2.0,3.0/2.0));
}

void main()
{};

