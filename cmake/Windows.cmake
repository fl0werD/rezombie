#--------------------------------------------------------------------------------------------
# Common compile definitions
#--------------------------------------------------------------------------------------------

add_compile_definitions(
  # Disable Windows-provided min/max macros
  NOMINMAX

  # Build type Debug
  $<$<CONFIG:Debug>:_DEBUG>

  # Build type Release, MinSizeRel, RelWithDebInfo
  $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:NDEBUG>

  # Executable
  $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,EXECUTABLE>:_CONSOLE>

  # Shared library
  $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,SHARED_LIBRARY>:_USRDLL>
  $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,SHARED_LIBRARY>:_WINDLL>
  $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,SHARED_LIBRARY>:_WINDOWS>

  # Static library
  $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,STATIC_LIBRARY>:_LIB>

  # C++
  $<$<COMPILE_LANGUAGE:CXX>:$<$<NOT:$<BOOL:${ENABLE_EXCEPTIONS}>>:_HAS_EXCEPTIONS=0>>
)

#--------------------------------------------------------------------------------------------
# Common compile options
#--------------------------------------------------------------------------------------------

# MSVC exception handling flag /EHsc is not added to CMAKE_CXX_FLAGS by default
string(REGEX REPLACE "[ \t\r\n]*\/EH[a-z]+" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS)

# Select the MSVC runtime library for use by compilers targeting the MSVC ABI
set(CMAKE_MSVC_RUNTIME_LIBRARY
  MultiThreaded$<$<CONFIG:Debug>:Debug>DLL
)

# List of Key=Value records to be set per target as target properties
set(CMAKE_VS_GLOBALS
  "RunCodeAnalysis=false"
  "EnableMicrosoftCodeAnalysis=false"
  "EnableClangTidyCodeAnalysis=false"
)

# Enable Just My Code with Visual Studio debugger
set(CMAKE_VS_JUST_MY_CODE_DEBUGGING
  $<IF:$<CONFIG:Debug>,ON,OFF>
)

add_compile_options(
  /diagnostics:caret          # Diagnostics format: prints column and the indicated line of source
  /source-charset:utf-8       # Character set of source files
  /Zc:threadSafeInit-         # Thread-safe local static initialization

  # Enable/Disable RTTI support
  $<$<COMPILE_LANGUAGE:CXX>:$<IF:$<BOOL:${ENABLE_RTTI}>,/GR,/GR->>

  # Enable C++ exception handling
  $<$<COMPILE_LANGUAGE:CXX>:$<$<BOOL:${ENABLE_EXCEPTIONS}>:/EHsc>>
)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_compile_options(
    /Gw-                      # Whole-program global data optimization
    /Gy-                      # Function-level linking
    /Od                       # Disable optimization
    /Oy-                      # Omit frame pointer
    /Zi                       # Generate complete debugging information
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  add_compile_options(
    /O2                       # Create fast code
    /Ob2                      # Inline Function expansion
    /Ot                       # Favor fast code
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
  add_compile_options(
    /O1                       # Create small code
    /Ob1                      # Inline Function expansion
    /Os                       # Favor small code
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
  add_compile_options(
    /O2                       # Create fast code
    /Ob1                      # Inline Function expansion
    /Ot                       # Favor fast code
  )
endif()

if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_compile_options(
    /GF                       # String pooling
    /GS-                      # Checks buffer security
    /guard:cf-                # Control flow guard security checks
    /guard:ehcont-            # Generate EH Continuation (EHCONT) metadata
    /Gw                       # Whole-program global data optimization
    /Gy                       # Function-level linking
    /Oi                       # Generate intrinsic functions
    /Oy                       # Omit frame pointer

    # Optimize for Windows applications
    $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,EXECUTABLE>:/GA>
  )
endif()

#--------------------------------------------------------------------------------------------
# MSVC compile options
#--------------------------------------------------------------------------------------------

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_compile_options(
    /wd4706                   # Disable warning (assignment within conditional expression)
    /arch:SSE2                # Minimum CPU architecture requirements
    /cgthreads${NCORES}       # Number of cl.exe threads to use for optimization and code generation
    /external:anglebrackets   # Treat all headers included via <> as external
    /external:templates-      # Evaluate warning level across template instantiation chain
    /external:W0              # Warning level for external headers
    /FC                       # Displays the full path of source code files passed to cl.exe in diagnostic text
    /MP${NCORES}              # Build with multiple processes
    /options:strict           # Unrecognized compiler options are errors
    /validate-charset         # Validate UTF-8 files for only compatible characters
  )

  if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_compile_options(
      /Zo-                    # Generate richer debugging information for optimized code
    )
  endif()

  if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_options(
      /GL                     # Whole program optimization
      /JMC-                   # Just My Code debugging
      /sdl-                   # Enable more security features and warnings
      /Zc:inline              # Remove unreferenced functions or data if they're COMDAT or have internal linkage only
      /Qpar                   # Automatic parallelization of loops
    )
  endif()
endif()

#--------------------------------------------------------------------------------------------
# Common link options
#--------------------------------------------------------------------------------------------

add_link_options(
  /GUARD:NO                   # Control flow guard protection
  /MACHINE:X86                # The target platform
  /NXCOMPAT                   # Compatible with the Windows Data Execution Prevention feature
  /SUBSYSTEM:CONSOLE          # Subsystem
  /WX                         # Treat linker warnings as errors
)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_link_options(
    /DEBUG:FULL               # Debugging information
    /INCREMENTAL              # Incremental linking
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
  add_link_options(
    /DEBUG:NONE               # Debugging information
  )
endif()

if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_link_options(
    /INCREMENTAL:NO           # Incremental linking
    /LTCG                     # Link-time code generation
    /OPT:REF,ICF              # Link optimizations
  )
endif()

# Static libraries linked by default
link_libraries(
  advapi32.lib
  comdlg32.lib
  gdi32.lib
  kernel32.lib
  odbc32.lib
  odbccp32.lib
  ole32.lib
  oleaut32.lib
  shell32.lib
  user32.lib
  uuid.lib
  winspool.lib
)

#--------------------------------------------------------------------------------------------
# MSVC link options
#--------------------------------------------------------------------------------------------

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_link_options(
    /CGTHREADS:${NCORES}      # Number of cl.exe threads to use for optimization and code generation
  )

  if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_link_options(
      /DEBUG:FASTLINK         # Debugging information
    )
  endif()
endif()

#--------------------------------------------------------------------------------------------
# Targets compile options
# setup_target_properties(target1 target2 ...)
#--------------------------------------------------------------------------------------------

function(setup_target_properties)
  foreach(target_name ${ARGV})
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      set_target_properties("${target_name}" PROPERTIES
        VS_GLOBAL_UseDebugLibraries true
        VS_GLOBAL_LinkIncremental true
      )
    else()
      set_target_properties("${target_name}" PROPERTIES
        VS_GLOBAL_UseDebugLibraries false
        VS_GLOBAL_LinkIncremental false
      )
    endif()
  endforeach()
endfunction()

#--------------------------------------------------------------------------------------------
# Targets compile options
# setup_target_compile_options(target1 target2 ...)
#--------------------------------------------------------------------------------------------

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  function(setup_target_compile_options)
    foreach(target_name ${ARGV})
      target_compile_options("${target_name}" PRIVATE
        -D_MBCS               # Use multi-byte character set
        /W4                   # Output warning level
        /permissive-          # Standard-conformance mode

        # Build type Debug
        $<$<CONFIG:Debug>:
          /GS                 # Checks buffer security
          /JMC                # Just My Code debugging
          /RTC1               # Runtime checks
          /sdl                # Enable more security features and warnings
        >
      )
    endforeach()
  endfunction()
endif()
