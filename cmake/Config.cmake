#--------------------------------------------------------------------------------------------
# General CMake configuration
#--------------------------------------------------------------------------------------------

if(APPLE)
  # Compiler id for Apple Clang is now "AppleClang"
  set(CMAKE_POLICY_DEFAULT_CMP0025 NEW)
endif()

# List TARGET_OBJECTS in SOURCES target property
set(CMAKE_POLICY_DEFAULT_CMP0051 NEW)

# INTERPROCEDURAL_OPTIMIZATION is enforced when enabled
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# Control generation of Position Independent Executable (PIE)
set(CMAKE_POLICY_DEFAULT_CMP0083 NEW)

# MSVC warning flags are not in CMAKE_<LANG>_FLAGS by default
set(CMAKE_POLICY_DEFAULT_CMP0092 NEW)

# Ninja generators transform DEPFILEs from add_custom_command()
set(CMAKE_POLICY_DEFAULT_CMP0116 NEW)

# MSVC RTTI flag /GR is not added to CMAKE_CXX_FLAGS by default
set(CMAKE_POLICY_DEFAULT_CMP0117 NEW)

# First search using Config mode before falling back to Module mode
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)

# Set the number of processor cores
include(ProcessorCount)
ProcessorCount(NCORES)

if(NCORES EQUAL 0)
  set(NCORES 4)
endif()

if(NCORES GREATER 8)
  set(NCORES 8)
endif()

# Specify the maximum number of concurrent processes to use when building
if(NOT $ENV{CMAKE_BUILD_PARALLEL_LEVEL} OR $ENV{CMAKE_BUILD_PARALLEL_LEVEL} LESS_EQUAL 0)
  set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} ${NCORES})
endif()

# Specify the number of tests for CTest to run in parallel
if(NOT $ENV{CTEST_PARALLEL_LEVEL} OR $ENV{CTEST_PARALLEL_LEVEL} LESS_EQUAL 0)
  set(ENV{CTEST_PARALLEL_LEVEL} ${NCORES})
endif()

# Specify additional arguments to the underlying build command
set(CTEST_BUILD_FLAGS -j${NCORES})

# Specify the available build types (configurations) on multi-config generators
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

# Specify the default output directory of the compiled binary files
if(NOT DEFAULT_OUTPUT_DIR)
  set(DEFAULT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}-$<CONFIG>")
endif()

# C
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)

# C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# IPO (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)

# Miscellaneous
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
