#include <cstdlib>
#include <iostream>
#include <sstream>

#if defined(MPI_SESSION)
#include <mpi.h>
#include "MpiComm.hpp"
#include "MpiSession.hpp"
#include "UnorderedMpiOstream.hpp"
#endif // defined(MPI_SESSION)

int main(int argc, char** argv)
{
#if defined(MPI_SESSION)
    using namespace distributed_memory_session;

    MpiSession::initialize(argc, argv);

    std::ostringstream oss;
    MpiSession::print_configuration(oss);
    umpicout.fromAllProc() << oss.str();

    int rank = MpiSession::getRank();
    int size = MpiSession::getNProc();

    if (size != 2)
    {
        if (rank == 0)
        {
            std::cout << "You should run this test with 2 processes\n";
        }
        return EXIT_FAILURE;
    }

    auto comm = MpiComm::world();
    if (rank == 0)
    {
        char send = 'O';
        char recv = ' ';
        char should_recv = 'K';
        comm.send(&send, 1, MpiComm::CHAR, 1, 111);
        comm.recv(&recv, 1, MpiComm::CHAR, 1, 112);

        if (recv != should_recv)
        {
            return EXIT_FAILURE;
        }
        std::cout << (std::string("Rank 0 is ")+send+recv+".\n");
    }
    if (rank == 1)
    {
        char send = 'K';
        char recv = ' ';
        char should_recv = 'O';

        comm.recv(&recv, 1, MpiComm::CHAR, 0, 111);
        comm.send(&send, 1, MpiComm::CHAR, 0, 112);

        if (recv != should_recv)
        {
            return EXIT_FAILURE;
        }
        std::cout << (std::string("Rank 1 is ")+recv+send+".\n");
    }

    MpiSession::finalize();
#endif // defined(MPI_SESSION)
    return EXIT_SUCCESS;
}
