/***********************************************
This file is part of the NRTB project (https://github.com/fpstovall/nrtb_beta).

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

import std.stdio, std.math;

/** Defines a point or vector in cartesian 3d space. Most binary math
 * operators shoould work
 * with both vect3d and real arguments, but only +, -, *, / and simple
 * assignment and equality are unit tested here. Note that operators
 * return the simple calculated results; dot product and vector product
 * are provided as discrete method calls.
 *
 * Also provided are the common vector operations
 *  vector_product,
 *  dot_product,
 *  magnitude,
 *  range (distance between points),
 *  and normalize (scales to magnitude 1.0).
 **/

struct vect3d
{
  alias real[3] vec;
  // vector
  private vec coords;
  /// blt copy override to avoid binding accidently.
  this(ref this) { coords = coords.dup; }
  /// Construct from a real array. Only arrays of length 3 are acceptable.
  this(in real[] a)
  {
    assert(a.length==3,"!! Invalid input length in vect3d.(T[] a).");
    coords[] = a[0 .. 3];
  }
  /// Construct from a single real. All three elements will be assigned the
  /// same value.
  this(in real a) { coords[] = a;}
  /// Construct from 3 reals. They are assigned in the order presented.
  this(in real x, real y, real z) { coords[0]=x; coords[1]=y; coords[2]=z;}
  /// returns an array containing copies of the coordinates.
  vec values() { vec v; v = coords.dup; return v; }
  /// returns the length (the magnitude) of the vector
  real magnitude()
  {
    real returnme = 0.0;
    foreach(val; coords) { returnme += (val*val); }
    return sqrt(returnme);
  }
  /// returns the dot product
  real dot_product(in vect3d a)
  {
    real returnme = 0.0;
    returnme += coords[0] * a.coords[0];
    returnme += coords[1] * a.coords[1];
    returnme += coords[2] * a.coords[2];
    return returnme;
  }
  /// returns the vector product
  vect3d vector_product(in vect3d a)
  {
    vect3d returnme;
    with (returnme)
    {
      coords[0] =
        (this.coords[1] * a.coords[2]) - (this.coords[2] * a.coords[1]);
      coords[1] =
        (this.coords[2] * a.coords[0]) - (this.coords[0] * a.coords[2]);
      coords[2] =
        (this.coords[0] * a.coords[1]) - (this.coords[1] * a.coords[0]);
    }
    return returnme;
  }
  /// range returns the distance between two vect3d
  real range(in vect3d a)
  {
    return (this - a).magnitude();
  }
  /// returns the normalized vector (scaled to length 1.0)
  vect3d normalize()
  {
    return this / magnitude();
  }
  // vect3d by vect3d operators
  vect3d opBinary(string op)(vect3d rhs)
  {
    vect3d v;
    mixin("v.coords[] = coords[] "~op~" rhs.coords[];");
    return v;
  }
  // vect3d by real operators
  vect3d opBinary(string op)(real rhs)
  {
    vect3d v;
    mixin("v.coords[] = coords[] "~op~" rhs;");
    return v;
  }
}

unittest
{
  // test both literal constuctors
  auto a = vect3d(2);
  assert(a==vect3d(2,2,2));
  // test init from an array.
  real[] array = [2,3,4];
  auto at = vect3d(array);
  assert(at == vect3d(2,3,4));
  assert(at.values() == array);
  // test copying
  auto t=a;
  assert(t==vect3d(2,2,2));
  // test major vect3d by vect3d math
  auto b = vect3d(1,2,3);
  assert((b+a)==vect3d(3,4,5));
  assert((b-a)==vect3d(-1,0,1));
  assert((b*a)==vect3d(2,4,6));
  assert((b/a)==vect3d(1.0/2.0,2.0/2.0,3.0/2.0));
  // test major vect3d by real math shifts and scales
  assert((b+1)==vect3d(2,3,4));
  assert((b-1)==vect3d(0,1,2));
  assert((b*2)==vect3d(2,4,6));
  assert((b/2.0)==vect3d(1.0/2.0,2.0/2.0,3.0/2.0));
  // magnitude test
  assert(a.magnitude()==sqrt(2.0*2.0*3.0));
  // dot_product test
  assert(a.dot_product(a)==12.0);
  // vector_product test.
  assert(b.vector_product(a)==vect3d(-2,4,-2));
  assert(a.vector_product(a)==vect3d(0.0));
  // Range tests
  assert(a.range(a)==0.0);
  assert(approxEqual(a.range(b),1.41421));
  // normalize tests.
  assert(approxEqual(a.normalize().magnitude(),1.0));
  assert(approxEqual(b.normalize().magnitude(),1.0));
}
