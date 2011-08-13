// singleton template test program


#include "singleton.h"
#include <serializer.h>
#include <iostream>

using namespace nrtb;
using namespace std;

typedef singleton<serializer> sequence_type;

int main()
{
  int er_count = 0;
  
  sequence_type & a = sequence_type::get_instance();
  
  for (int i=0; i<10; i++)
  {
	cout << a();
  };
  
  sequence_type & b = sequence_type::get_instance();
  
  
  if ( b() != 11)
  {
	er_count++;
  };
  
  cout << "\nsingleton test " << (er_count ? "passed" : "failed")
	<< "." << endl;

  return er_count;
  
};
