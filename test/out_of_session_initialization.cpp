#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include "MpiSession.hpp"

int main(int argc, char** argv)
{
    MpiSession::finalize();

    ::MPI_Init(&argc, &argv);

    MpiSession::initialize(argc, argv);

    ::MPI_Finalize();

    MpiSession::initialize(argc, argv);

    MpiSession::finalize();

    return EXIT_SUCCESS;
}
