set(CMAKE_OBJCXX_COMPILER "/usr/bin/c++")
set(CMAKE_OBJCXX_COMPILER_ARG1 "")
set(CMAKE_OBJCXX_COMPILER_ID "AppleClang")
set(CMAKE_OBJCXX_COMPILER_VERSION "14.0.3.14030022")
set(CMAKE_OBJCXX_COMPILER_VERSION_INTERNAL "")
set(CMAKE_OBJCXX_COMPILER_WRAPPER "")
set(CMAKE_OBJCXX_STANDARD_COMPUTED_DEFAULT "98")
set(CMAKE_OBJCXX_EXTENSIONS_COMPUTED_DEFAULT "ON")
set(CMAKE_OBJCXX_STANDARD_LATEST "20")
set(CMAKE_OBJCXX_COMPILE_FEATURES "")
set(CMAKE_OBJCXX98_COMPILE_FEATURES "")
set(CMAKE_OBJCXX11_COMPILE_FEATURES "")
set(CMAKE_OBJCXX14_COMPILE_FEATURES "")
set(CMAKE_OBJCXX17_COMPILE_FEATURES "")
set(CMAKE_OBJCXX20_COMPILE_FEATURES "")
set(CMAKE_OBJCXX23_COMPILE_FEATURES "")

set(CMAKE_OBJCXX_PLATFORM_ID "Darwin")
set(CMAKE_OBJCXX_SIMULATE_ID "")
set(CMAKE_OBJCXX_COMPILER_FRONTEND_VARIANT "GNU")
set(CMAKE_OBJCXX_SIMULATE_VERSION "")


set(CMAKE_AR "/usr/bin/ar")
set(CMAKE_OBJCXX_COMPILER_AR "")
set(CMAKE_RANLIB "/usr/bin/ranlib")
set(CMAKE_OBJCXX_COMPILER_RANLIB "")
set(CMAKE_LINKER "/usr/bin/ld")
set(CMAKE_LINKER_LINK "")
set(CMAKE_LINKER_LLD "")
set(CMAKE_OBJCXX_COMPILER_LINKER "/Library/Developer/CommandLineTools/usr/bin/ld")
set(CMAKE_OBJCXX_COMPILER_LINKER_ID "AppleClang")
set(CMAKE_OBJCXX_COMPILER_LINKER_VERSION 857.1)
set(CMAKE_OBJCXX_COMPILER_LINKER_FRONTEND_VARIANT GNU)
set(CMAKE_MT "")
set(CMAKE_TAPI "/Library/Developer/CommandLineTools/usr/bin/tapi")
set(CMAKE_COMPILER_IS_GNUOBJCXX )
set(CMAKE_OBJCXX_COMPILER_LOADED 1)
set(CMAKE_OBJCXX_COMPILER_WORKS TRUE)
set(CMAKE_OBJCXX_ABI_COMPILED TRUE)

set(CMAKE_OBJCXX_COMPILER_ENV_VAR "OBJCXX")

set(CMAKE_OBJCXX_COMPILER_ID_RUN 1)
set(CMAKE_OBJCXX_SOURCE_FILE_EXTENSIONS M;m;mm)
set(CMAKE_OBJCXX_IGNORE_EXTENSIONS inl;h;hpp;HPP;H;o;O)

if (CMAKE_OBJC_COMPILER_ID_RUN)
  foreach(extension IN LISTS CMAKE_OBJC_SOURCE_FILE_EXTENSIONS)
    list(REMOVE_ITEM CMAKE_OBJCXX_SOURCE_FILE_EXTENSIONS ${extension})
  endforeach()
endif()

foreach (lang IN ITEMS C CXX OBJC)
  foreach(extension IN LISTS CMAKE_OBJCXX_SOURCE_FILE_EXTENSIONS)
    if (CMAKE_${lang}_COMPILER_ID_RUN)
      list(REMOVE_ITEM CMAKE_${lang}_SOURCE_FILE_EXTENSIONS ${extension})
    endif()
  endforeach()
endforeach()

set(CMAKE_OBJCXX_LINKER_PREFERENCE 25)
set(CMAKE_OBJCXX_LINKER_PREFERENCE_PROPAGATES 1)
set(CMAKE_OBJCXX_LINKER_DEPFILE_SUPPORTED )
set(CMAKE_LINKER_PUSHPOP_STATE_SUPPORTED )
set(CMAKE_OBJCXX_LINKER_PUSHPOP_STATE_SUPPORTED )

# Save compiler ABI information.
set(CMAKE_OBJCXX_SIZEOF_DATA_PTR "8")
set(CMAKE_OBJCXX_COMPILER_ABI "")
set(CMAKE_OBJCXX_BYTE_ORDER "LITTLE_ENDIAN")
set(CMAKE_OBJCXX_LIBRARY_ARCHITECTURE "")

if(CMAKE_OBJCXX_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_OBJCXX_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_OBJCXX_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_OBJCXX_COMPILER_ABI}")
endif()

if(CMAKE_OBJCXX_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()





set(CMAKE_OBJCXX_IMPLICIT_INCLUDE_DIRECTORIES "/usr/local/include;/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1;/Library/Developer/CommandLineTools/usr/lib/clang/14.0.3/include;/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include;/Library/Developer/CommandLineTools/usr/include")
set(CMAKE_OBJCXX_IMPLICIT_LINK_LIBRARIES "c++")
set(CMAKE_OBJCXX_IMPLICIT_LINK_DIRECTORIES "/usr/local/lib;/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib")
set(CMAKE_OBJCXX_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks")
