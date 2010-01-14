// singleton template test program


#include <http_page.h>
#include "singleton.h"
#include <iostream>

using namespace NRTB;
using namespace std;

int main()
{
	typedef singleton<http_page> p_type;
	
	{
		p_type & getter = p_type::get_instance();

		cout << "newly initialized code: " << getter.result_code() << endl;

		getter.get("127.0.0.1:80","/");
	};

	cout << "first response code: " << p_type::get_instance().result_code()
		<< endl;
	
	p_type::get_instance().get("127.0.0.1:80","/not_there.html");

	cout << "second response code: " <<  p_type::get_instance().result_code()
		 << endl;

	p_type::get_instance().delete_me();
};
