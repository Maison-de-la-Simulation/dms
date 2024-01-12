#pragma once

#include <iostream>

class SerialSession
{
public:
    static void initialize(int&, char**&);
    static void finalize();
    static void synchronize();
    static constexpr bool isActive()
    {
        return isInitialized() && !isFinalized();
    }
    static constexpr bool isInitialized()
    { return true; }
    static constexpr bool isIOProc()
    { return true; }
    static constexpr bool isFinalized()
    { return true; }
    static constexpr int getRank()
    { return 0; }
    static constexpr int getNProc()
    { return 1; }
    static constexpr int getIOProc()
    { return 0; }
    static void print_configuration(std::ostream& os);
}; // class DistributedMemorySession
