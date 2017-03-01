#include "ProcShell.h"
#include "functions.h"

#include <stdexcept>
#include <algorithm>
#include <functional>
#include <cctype>

using namespace std;

ProcShell::ProcShell() : _command_map()
{
}

ProcShell::~ProcShell()
{
}

ProcShell &ProcShell::when(string const &command, ProcShell::Proc callback, string const &explain)
{
	_command_map[command] = make_pair(callback, explain);
	return *this;
}

string ProcShell::explain(string const &command) const
{
    auto it = _command_map.find(command);
    if(it != _command_map.end())
    {
        return it->second.second;
    }
    else
    {
        throw invalid_argument("no such command");
    }
}

void ProcShell::parse_istream(istream &in)
{
	string read;
	if(getline(in, read))
	{
        str_trim_ws(read);
        if(read.size() == 0)
            return;
		auto it = _command_map.find(read);
		if(it != _command_map.end())
		{
			it->second.first();
		}
		else
		{
            in.setstate(ios_base::failbit);
		}
	}
}

