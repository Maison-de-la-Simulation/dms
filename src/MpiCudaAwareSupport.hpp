//!
//! @author Thomas Padioleau
//! @brief CUDA-related information.
//! @file MpiCudaAwareSupport.hpp
//! @date 29 Aug 2019
//!
#pragma once

#include <mpi.h>

#if defined(OPEN_MPI)
#include <mpi-ext.h> /* Needed for CUDA-aware check */
#endif // defined(OPEN_MPI)

//!
//! @brief Helper function to determine if MPI is CUDA-aware
//!
//! This function returns True only when the macro
//! MPIX_CUDA_AWARE_SUPPORT is set to true by Open MPI.
//! It returns False otherwise.
//!
//! @return True if MPI is CUDA-aware, False otherwise
//!
constexpr bool isMpiCudaAwareAvailable() noexcept
{
#if defined(MPIX_CUDA_AWARE_SUPPORT) && MPIX_CUDA_AWARE_SUPPORT
    return true;
#else
    return false;
#endif
}
