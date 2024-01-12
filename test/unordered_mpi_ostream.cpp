#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "DistributedMemorySession.hpp"
#include "UnorderedMpiOstream.hpp"

int main(int argc, char** argv)
{
    umpicout.fromIOProc() << "[Before initializing distributed session]\n";
    {
        std::ostringstream oss;
        Session::print_configuration(oss);
        umpicout.fromAllProc() << oss.str();
    }

    Session::initialize(argc, argv);

    umpicout.fromIOProc() << "[After initializing distributed session]\n";
    {
        std::ostringstream oss;
        Session::print_configuration(oss);
        umpicout.fromAllProc() << oss.str();
    }

    Session::finalize();

    umpicout.fromIOProc() << "[After finalizing distributed session]\n";
    {
        std::ostringstream oss;
        Session::print_configuration(oss);
        umpicout.fromAllProc() << oss.str();
    }

    return EXIT_SUCCESS;
}
