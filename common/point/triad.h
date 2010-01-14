/*************************************************************
**** CODE REVIEW: triad.h, starting 2008-11-24 ***************
* The triad template is fully implemented in this one file.
* This class will be used for all vector and point operations
* Please make any comments or suggestions directly in the file.
*************************************************************/

/**************************************************
TODO: add methods for dot product, cross multiply, 
	finding the surface normal, etc.
/**************************************************

#ifndef nrtb_triad_header
#define nrtb_triad_header

#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>

namespace NRTB
{

/** Template defining a point or vector in cartesian 3d space. Should work
 ** with all floating point types.
 ** 
 ** Implements the following operators: +,=,*,/,+=,-=,*=,/= for both
 ** triad<T> and <T> arguments; each returns a triad as the result.
 ** ==,!=,>>,<< are implemented for triad arguments. Additionally
 ** implements pow() with triad and T arguments. Finally implements range()
 ** which returns the distance between the triad and one supplied, and
 ** magnatude() which returns the length of the vector.
 **/
template <class T> 
struct triad
{
	/// X coordinate
	T x;
	/// Y coordinate
	T y;
	/// Z coordinate
	T z;

	/** Constructor. 
	 ** 
	 ** An easy way to create a point at the origin would be 
	 ** "triad<T> a;".
	 **/
	triad(T _x=0, T _y=0, T _z=0);
	triad<T> operator + (const triad<T> & a);
	triad<T> operator - (const triad<T> & a);
	triad<T> operator * (const triad<T> & a);
	triad<T> operator / (const triad<T> & a);
	triad<T> operator += (const triad<T> & a);
	triad<T> operator -= (const triad<T> & a);
	triad<T> operator *= (const triad<T> & a);
	triad<T> operator /= (const triad<T> & a);
	/// returns (pow(x,a.x),pow(y,a.y),pow(z,a.z)).
	triad<T> pow(const triad<T> & a);
	triad<T> operator + (const T & a);
	triad<T> operator - (const T & a);
	triad<T> operator * (const T & a);
	triad<T> operator / (const T & a);
	triad<T> operator += (const T & a);
	triad<T> operator -= (const T & a);
	triad<T> operator *= (const T & a);
	triad<T> operator /= (const T & a);
	/// returns (pow(x,a),pow(y,a),pow(z,a)).
	triad<T> pow(const T & a);
	/** Returns a normalized version of the triad.
	 ** 
	 ** Normalization in this case refers to an operation resulting in a
	 ** traid where the magnatude of the vector is 1.0 while maintaining the
	 ** same angular component. For example, given triad<float> a = (1,2,3),
	 ** a.normalize() = (0.267,0.534,0.802). Of course, the original triad
	 ** is not modified.
	 ** 
	 ** Normalized triads are very useful when calcuating force or
	 ** accelleration vectors and in many other cases.
	 **/
	triad<T> normalize();
	/// Returns the distance between *this and the supplied argument.
	T range(const triad<T> & a);
	/// Returns the magnatude of the vector.
	T magnatude();
	bool operator == (const triad<T> & a);
	bool operator != (const triad<T> & a);
	/// Loads from a std::string.
	triad<T> from_str(const std::string & a);
	/// Returns a std::string representation.
	std::string to_str(int precision=6);
};

template <class T>
triad<T>::triad(T _x, T _y, T _z)
{
	x = _x;
	y = _y;
	z = _z;
};

template <class T>
triad<T> triad<T>::operator + (const triad<T> & a)
{
	triad<T> returnme;
	returnme.x = x + a.x;
	returnme.y = y + a.y;
	returnme.z = z + a.z;
	return returnme;
};

template <class T>
triad<T> triad<T>::operator - (const triad<T> & a)
{
	triad<T> returnme;
	returnme.x = x - a.x;
	returnme.y = y - a.y;
	returnme.z = z - a.z;
	return returnme;
};

template <class T>
triad<T> triad<T>::operator * (const triad<T> & a)
{
	triad<T> returnme;
	returnme.x = x * a.x;
	returnme.y = y * a.y;
	returnme.z = z * a.z;
	return returnme;
};

template <class T>
triad<T> triad<T>::operator / (const triad<T> & a)
{
	triad<T> returnme;
	returnme.x = x / a.x;
	returnme.y = y / a.y;
	returnme.z = z / a.z;
	return returnme;
};

template <class T>
triad<T> triad<T>::operator += (const triad<T> & a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
};

template <class T>
triad<T> triad<T>::operator -= (const triad<T> & a)
{
	x -= a.x;
	y -= a.y;
	z -= a.z;
	return *this;
};

template <class T>
triad<T> triad<T>::operator *= (const triad<T> & a)
{
	x *= a.x;
	y *= a.y;
	z *= a.z;
	return *this;
};

template <class T>
triad<T> triad<T>::operator /= (const triad<T> & a)
{
	x /= a.x;
	y /= a.y;
	z /= a.z;
	return *this;
};

template <class T>
triad<T> triad<T>::pow(const triad<T> & a)
{
	triad<T> returnme;
	returnme.x = ::pow(x,a.x);
	returnme.y = ::pow(y,a.y);
	returnme.z = ::pow(z,a.z);
	return returnme;
};

template <class T>
triad<T> triad<T>::normalize()
{
	return (*this / this->magnatude());
};

template <class T>
triad<T> triad<T>::operator + (const T & a)
{
	triad<T> returnme;
	returnme.x = x + a;
	returnme.y = y + a;
	returnme.z = z + a;
	return returnme;
};

template <class T>
triad<T> triad<T>::operator - (const T & a)
{
	triad<T> returnme;
	returnme.x = x - a;
	returnme.y = y - a;
	returnme.z = z - a;
	return returnme;
};

template <class T>
triad<T> triad<T>::operator * (const T & a)
{
	triad<T> returnme;
	returnme.x = x * a;
	returnme.y = y * a;
	returnme.z = z * a;
	return returnme;
};

template <class T>
triad<T> triad<T>::operator / (const T & a)
{
	triad<T> returnme;
	returnme.x = x / a;
	returnme.y = y / a;
	returnme.z = z / a;
	return returnme;
};

template <class T>
triad<T> triad<T>::operator += (const T & a)
{
	x += a;
	y += a;
	z += a;
	return *this;
};

template <class T>
triad<T> triad<T>::operator -= (const T & a)
{
	x -= a;
	y -= a;
	z -= a;
	return *this;
};

template <class T>
triad<T> triad<T>::operator *= (const T & a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
};

template <class T>
triad<T> triad<T>::operator /= (const T & a)
{
	x /= a;
	y /= a;
	z /= a;
	return *this;
};

template <class T>
triad<T> triad<T>::pow(const T & a)
{
	triad<T> returnme;
	returnme.x = ::pow(x,a);
	returnme.y = ::pow(y,a);
	returnme.z = ::pow(z,a);
	return returnme;
};

template <class T>
T triad<T>::range(const triad<T> & a)
{
	triad<T> offset = *this - a;
	offset *= offset;
	return sqrt(offset.x + offset.y + offset.z);
};

template <class T>
T triad<T>::magnatude()
{
	return sqrt((x*x) + (y*y) + (z*z));
};

template <class T>
bool triad<T>::operator == (const triad<T> & a)
{
	return ((x == a.x) && (y == a.y) && (z == a.z));
};

template <class T>
bool triad<T>::operator != (const triad<T> & a)
{
	return (! (*this == a));
};

template <class T>
triad<T> triad<T>::from_str(const std::string & a)
{
	std::istringstream t(a.c_str());
	t >> *this;
	return *this;
};

template <class T>
std::string triad<T>::to_str(int precision)
{
	std::stringstream a;
	a << std::setprecision(precision) << *this;
	std::string returnme;
	a >> returnme;
	return returnme;
};

/// Writes the triad<T> in the format "(x,y,z)" to the output stream.
template <class T>
std::ostream & operator << (std::ostream &f, const triad<T> & a)
{
	f << "(" << a.x << "," << a.y << "," << a.z << ")";
	return f;
};

 //?? Review question: should we have the read operator
 //   throw instead of defaulting to (0,0,0) on a bad read?
 // A: No, because this is the same behavior the read
 // 	operator exhibits with other numeric types.
 
/** Reads the triad<T> from an input stream.
 ** 
 ** The acceptable format is [(]x,y,z[)]. Any other format will result
 ** in the triad<T> being set to (0,0,0). Any numeric format acceptable
 ** to an iostream is valid for x, y and z. Any other represention of x,
 ** y or z will result in that particular value being set to 0.
 **/
template <class T>
std::istream & operator >> (std::istream &f, triad<T> & a)
{
	std::string element;
	f >> element;
	//TODO: Need to replace split with boost equivelent
	std::vector<std::string> t = split(element,',');
	if (t.size() != 3)
	{
		a = 0;
	}
	else
	{
		std::stringstream transmorgrafier;
		transmorgrafier << std::setprecision(30);
		if (t[0][0] == '(') { t[0].erase(0,1); };
		for(int i=0; i < 3; i++) { transmorgrafier << t[i] << " "; };
		transmorgrafier >> a.x >> a.y >> a.z;
	};
	return f;
};

} // namespace NRTB

#endif // nrtb_triad_header
