#include "MpiSession.hpp"

#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <sstream>

namespace
{
int getDigits(int n)
{
    int digits {0};
    while(n > 0)
    {
        n /= 10;
        digits++;
    }
    return digits;
}
}

void MpiSession::initialize(int& argc, char**& argv)
{
    if (!isInitialized())
    {
        errCheck(::MPI_Init(&argc, &argv), "MPI_Init");
    }
} // MpiSession::initialize

void MpiSession::finalize()
{
    if (isActive())
    {
        errCheck(::MPI_Finalize(), "MPI_Finalize");
    }
} // MpiSession::~MpiSession

void MpiSession::synchronize()
{
    if (isActive())
    {
        errCheck(::MPI_Barrier(MPI_COMM_WORLD), "MPI_Barrier");
    }
} // MpiSession::synchronize

bool MpiSession::isActive()
{
    return isInitialized() && !isFinalized();
}

bool MpiSession::isInitialized()
{
    int mpi_is_initialized = -1;
    errCheck(::MPI_Initialized(&mpi_is_initialized), "MPI_Initialized");
    return mpi_is_initialized;
}

bool MpiSession::isIOProc()
{
    return getRank() == getIOProc();
}

bool MpiSession::isFinalized()
{
    int mpi_is_finalized = -1;
    errCheck(::MPI_Finalized(&mpi_is_finalized), "MPI_Finalized");
    return mpi_is_finalized;
}

int MpiSession::getRank()
{
    int rank = 0;
    if (isActive())
    {
        errCheck(::MPI_Comm_rank(MPI_COMM_WORLD, &rank), "MPI_Comm_rank");
    }
    return rank;
}

int MpiSession::getNProc()
{
    int nproc = 1;
    if (isActive())
    {
        errCheck(::MPI_Comm_size(MPI_COMM_WORLD, &nproc), "MPI_Comm_size");
    }
    return nproc;
}

int MpiSession::getIOProc()
{
    return 0;
}

void MpiSession::errCheck(int mpierr, const std::string& fname)
{
    if (mpierr != 0)
    {
        std::cerr << fname << " returned error code " << mpierr << std::endl;
        throw;
    }
}

void MpiSession::print_configuration(std::ostream& os)
{
    if (isActive())
    {
        int nameLen = -1;
        char procName[MPI_MAX_PROCESSOR_NAME];
        errCheck(::MPI_Get_processor_name(procName, &nameLen), "MPI_Get_processor_name");

        int rank = getRank();
        int nproc = getNProc();

        os << "Session: MPI process ";
        os << std::setw(getDigits(getNProc()-1)) << std::setfill(' ') << rank << " / " << nproc;
        os << " [MPI_COMM_WORLD]";
        os << " on host " << procName << ".\n";
    }
    else
    {
        os << "Session: Serial 0 / 1 on host unknown.\n";
    }
}
