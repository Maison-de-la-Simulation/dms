//!
//! @author Thomas Padioleau
//! @brief Test if isMpiCudaAwareAvailable() is correctly implemented.
//! @file cuda_aware_support.cpp
//! @date 29 Aug 2019
//!
#include <cstdlib>
#include <iostream>
#include <sstream>

#if defined(MPI_SESSION)
#include <mpi.h>

#if defined(OPEN_MPI)
#include <mpi-ext.h> /* Needed for CUDA-aware check */
#endif // defined(OPEN_MPI)

#include "MpiCudaAwareSupport.hpp"
#endif

int main(int argc, char** argv)
{
#if defined(MPI_SESSION)
    std::ostringstream oss;

    oss << "Compile time check:\n";
#if defined(MPIX_CUDA_AWARE_SUPPORT) && MPIX_CUDA_AWARE_SUPPORT
    if (isMpiCudaAwareAvailable() == true)
    {
        oss << "This MPI library has CUDA-aware support.\n";
    }
    else
    {
        return EXIT_FAILURE;
    }
#elif defined(MPIX_CUDA_AWARE_SUPPORT) && !MPIX_CUDA_AWARE_SUPPORT
    if (isMpiCudaAwareAvailable() == false)
    {
        oss << "This MPI library does not have CUDA-aware support.\n";
    }
    else
    {
        return EXIT_FAILURE;
    }
#else
    if (isMpiCudaAwareAvailable() == false)
    {
        oss << "This MPI library cannot determine if there is CUDA-aware support.\n";
    }
    else
    {
        return EXIT_FAILURE;
    }
#endif // MPIX_CUDA_AWARE_SUPPORT

    oss << "Run time check:\n";
#if defined(MPIX_CUDA_AWARE_SUPPORT)
    if (1 == MPIX_Query_cuda_support())
    {
        if (isMpiCudaAwareAvailable() == true)
        {
            oss << "This MPI library has CUDA-aware support.\n";
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        if (isMpiCudaAwareAvailable() == false)
        {
            oss << "This MPI library does not have CUDA-aware support.\n";
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
#else // !defined(MPIX_CUDA_AWARE_SUPPORT)
    if (isMpiCudaAwareAvailable() == false)
    {
        oss << "This MPI library cannot determine if there is CUDA-aware support.\n";
    }
    else
    {
        return EXIT_FAILURE;
    }
#endif // MPIX_CUDA_AWARE_SUPPORT

    ::MPI_Init(&argc, &argv);

    int rank = 0;
    ::MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        std::cout << oss.str();
    }

    ::MPI_Finalize();

#endif // defined(MPI_SESSION)

    return EXIT_SUCCESS;
}
