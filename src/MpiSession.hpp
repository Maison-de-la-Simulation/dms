#pragma once

#include <iostream>
#include <string>

class MpiSession
{
public:
    static void initialize(int& argc, char**& argv);
    static void finalize();
    static void synchronize();
    static bool isActive();
    static bool isInitialized();
    static bool isIOProc();
    static bool isFinalized();
    static int getRank();
    static int getNProc();
    static int getIOProc();
    static void print_configuration(std::ostream& os);

private:
    static void errCheck(int mpierr, const std::string& fname);
}; // class DistributedMemorySession
