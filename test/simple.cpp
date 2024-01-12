#include <cstdlib>
#include <sstream>

#include "DistributedMemorySession.hpp"
#include "UnorderedMpiOstream.hpp"

int main(int argc, char** argv)
{
    Session::initialize(argc, argv);

    std::ostringstream oss;
    Session::print_configuration(oss);
    umpicout.fromAllProc() << oss.str();

    Session::finalize();

    return EXIT_SUCCESS;
}
