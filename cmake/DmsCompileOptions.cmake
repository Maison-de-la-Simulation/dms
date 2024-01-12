function(dms_compile_options target)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${target} PRIVATE
      -Wall
      -Wextra
      -pedantic
      -Wshadow
      -Wformat=2
      -Wfloat-equal
      -Wwrite-strings)
  endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${target} PRIVATE
      -Wall
      -Wextra
      -pedantic
      -Wshadow
      -Wformat=2
      -Wfloat-equal
      -Wwrite-strings
      -Weffc++
      -Wdouble-promotion
      -Wconversion
      # -Wsign-conversion
      # -Wold-style-cast
      # -Wuseless-cast
      -Wlogical-op)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6)
      target_compile_options(${target} PRIVATE
        -Wduplicated-cond
        -Wnull-dereference)
    endif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6)

    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7)
      target_compile_options(${target} PRIVATE
        -Wduplicated-branches)
    endif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7)
  endif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    target_compile_options(${target} PRIVATE
      -Wall
      -Wextra
      -pedantic
      -Wshadow
      -Wformat=2
      -Wfloat-equal
      -Wwrite-strings)
  endif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
endfunction(dms_compile_options)
