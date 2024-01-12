#include "SerialSession.hpp"

#include <iostream>

void SerialSession::initialize(int&, char**&)
{
}

void SerialSession::finalize()
{
}

void SerialSession::synchronize()
{
}

void SerialSession::print_configuration(std::ostream& os)
{
    os << "Session: Serial 0 / 1 on host unknown.\n";
}
