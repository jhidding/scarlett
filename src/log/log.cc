#include "log.h"

#include <iomanip>
#include <iterator>
using namespace Scarlett;

std::unique_ptr<std::set<Log *>> Log::_log_book;

std::ostream &Scarlett::operator<<(
	std::ostream &out, std::pair<double, std::string> const &E)
{
	out 
		<< std::setiosflags(std::ios::fixed)
		<< std::setprecision(6)
		<< std::setfill('0')
		<< "["
		<< std::setw(13) << E.first
		<< "] | "
		<< E.second;

	return out;
}

Log::Log(std::string const &prefix_, bool echo_):
	prefix(prefix_), echo(echo_)
{
	log_book().insert(this);
	time_stamp("start");
}

Log::~Log()
{
	time_stamp("end");
	log_book().erase(this);
}

void Log::print_log_book(std::ostream &out)
{
	std::vector<Entry> A;

	for (Log *L : log_book())
	{
		std::copy(L->entries.begin(), L->entries.end(),
			std::back_inserter(A));
	}

	std::sort(A.begin(), A.end(),
		[] (Entry const &x, Entry const &y)
	{
		return x.first < y.first;
	});

	for (Entry const &E : A)
	{
		out << E << std::endl;
	}
}

void Log::time_stamp(std::string const &tag)
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);

	char c[80];
	strftime(c, 80, "%A %e %B %Y %T %Z", localtime(&t.tv_sec));

	entry(tag, " time: ", std::setfill('0'), t.tv_sec,
			".", std::setw(9), t.tv_nsec,
			" (", c, ")");
}

