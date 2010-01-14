/* see nrtb_timer.h for whatever documentation you can find */

#include <nrtb_timer.h>
#include <math.h>
#include <sstream>

namespace NRTB
{

hirez_timer::hirez_timer(const double init)
{
	reset();
	start(init);
};

hirez_timer::hirez_timer(const timeval & init)
{
	reset();
	start(init);
};

void hirez_timer::reset()
{
	running = false;
	starttime = 0;
	elapsed = 0;
};

void hirez_timer::start(const double init)
{
	if (!running)
	{
		timeval startt;
		gettimeofday(&startt,0);
		starttime = tv_to_ll(startt);
		if (init > 0)
		{
			starttime -= double_to_ll(init);
		};
	};
	running = true;
};

void hirez_timer::start(const timeval & init)
{	
	starttime = tv_to_ll(init);
	running = true;
};

double hirez_timer::stop()
{
	stop_timer();
	return ll_to_double(elapsed);
};

unsigned long long hirez_timer::stop_as_usec()
{
	stop_timer();
	return elapsed;
};

double hirez_timer::interval()
{
	return ll_to_double(interval_time());
};

unsigned long long hirez_timer::interval_as_usec()
{
	return interval_time();
};

tm hirez_timer::interval_as_tm()
{
	time_t temp = (interval_time()+500000)/1000000;
	return *gmtime(&temp);
};

double hirez_timer::ll_to_double(const unsigned long long int ll)
{
	return ll / (double) 1e6;
};

timeval hirez_timer::ll_to_tv(const unsigned long long int ll)
{
	timeval t;
	t.tv_sec = ll / (unsigned long long int) 1e6;
	t.tv_usec = ll % (unsigned long long int) 1e6;
	return t;
};

unsigned long long int hirez_timer::tv_to_ll(const timeval & tv)
{
	return ((unsigned long long) tv.tv_sec * (unsigned long long) 1e6) 
		+ (unsigned long long) tv.tv_usec;
};

unsigned long long int hirez_timer::double_to_ll(const double d)
{
	return (unsigned long long int) floor(d * 1e6);
};

void hirez_timer::stop_timer()
{
	if (running)
	{
		timeval endtime;
		gettimeofday(&endtime,0);
		elapsed += (tv_to_ll(endtime) - starttime);
	};
	running = false;
};

unsigned long long int hirez_timer::interval_time()
{
	if (running)
	{
		timeval endtime;
		gettimeofday(&endtime,0);
		return elapsed + (tv_to_ll(endtime) - starttime);
	}
	else
	{
		return elapsed;
	};
};

std::string hirez_timer::interval_as_HMS(const bool days)
{
	// local working vars.
	std::stringstream output;
	double t1;
	long int t2;
	t1 = interval();
	// Days (only if needed)
	if (days)
	{
		t2 = long(t1) / 86400l;
		if (t2 > 0)
		{
			output << t2 << "d, ";
			t1 -= t2 * 86400l;
		};
	};
	// hours
	t2 = long(t1) / 3600;
	output << t2;
	t1 -=  t2 * 3600;
	// minutes
	t2 = long(t1) / 60;
	output << ((t2 < 10) ? ":0" : ":") << t2;
	t1 -= t2 * 60;
	// seconds
	output << ((t1 < 10) ? ":0" : ":") << t1;
	return output.str();
};

} // namespace NRTB;

