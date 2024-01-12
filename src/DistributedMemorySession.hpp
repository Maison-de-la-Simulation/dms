#pragma once

#if defined(SERIAL_SESSION)

#include "SerialSession.hpp"
using DistributedMemorySession = SerialSession;

#elif defined(MPI_SESSION)

#include "MpiSession.hpp"
using DistributedMemorySession = MpiSession;

#else

assert(false, "You must define a session")

#endif // ENABLE_MPI

using Session = DistributedMemorySession;
