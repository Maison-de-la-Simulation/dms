function (dms_add_test executable)
  if(ARGC EQUAL 1)
    set(nproc 2)
  else(ARGC EQUAL 1)
    set(nproc ${ARGV1})
  endif(ARGC EQUAL 1)

  if(${SESSION} STREQUAL SERIAL_SESSION)
    add_test(NAME ${executable} COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${executable})
  else(${SESSION} STREQUAL SERIAL_SESSION)
    add_test(NAME ${executable}
      COMMAND
      ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${nproc}
      ${CMAKE_CURRENT_BINARY_DIR}/${executable} ${MPIEXEC_POSTFLAGS})
  endif(${SESSION} STREQUAL SERIAL_SESSION)
endfunction(dms_add_test)

add_executable(simple simple.cpp)
target_link_libraries(simple dms)
dms_add_test(simple)


add_executable(unordered_mpi_ostream unordered_mpi_ostream.cpp)
target_link_libraries(unordered_mpi_ostream dms)
dms_add_test(unordered_mpi_ostream)


add_executable(cuda_aware_support cuda_aware_support.cpp)
target_link_libraries(cuda_aware_support dms)
dms_add_test(cuda_aware_support)


if(${SESSION} STREQUAL MPI_SESSION)
  add_executable(out_of_session_initialization out_of_session_initialization.cpp)
  target_link_libraries(out_of_session_initialization dms)
  dms_add_test(out_of_session_initialization)


  add_executable(mpicomm mpicomm.cpp)
  target_link_libraries(mpicomm dms)
  dms_add_test(mpicomm)
endif(${SESSION} STREQUAL MPI_SESSION)
