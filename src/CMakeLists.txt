add_library(dms UnorderedMpiOstream.cpp Print.cpp)

if(${SESSION} STREQUAL MPI_SESSION)
  target_sources(dms PRIVATE MpiSession.cpp MpiComm.cpp MpiCommCart.cpp)
  target_link_libraries(dms PUBLIC MPI::MPI_CXX)
else(${SESSION} STREQUAL MPI_SESSION)
  target_sources(dms PRIVATE SerialSession.cpp)
endif(${SESSION} STREQUAL MPI_SESSION)

target_include_directories(dms INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(dms PUBLIC ${SESSION})
dms_compile_options(dms)
