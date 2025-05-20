# ===============================================
# [ Simultaneous enable list ]
# AddressSanitizer + UndefinedBehaviorSanitizer
# ThreadSanitizer + UndefinedBehaviorSanitizer
# ===============================================

option(ENABLE_CODE_COVERAGE "Enable code coverage inspection" OFF)
option(ENABLE_ADDRESS_COVERAGE "Enable address coverage inspection(overflow, use-after-free..)" OFF)
option(ENABLE_MEMORY_COVERAGE "Enable memory coverage inspection(uninit memory)" OFF)
option(ENABLE_MEMORY_LEAKAGE "Enable memory leakage inspection" OFF)
option(ENABLE_UNDEFINED_BEHAVIOR "Enable undefined behavior inspection" OFF)
option(ENABLE_THREAD_COVERAGE "Enable thread inspection for race condition" OFF)

if (ENABLE_CODE_COVERAGE)
    message(STATUS "Using clang-tidy for static analysis")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if (NOT CLANG_TIDY_EXE)
        add_clangtidy()
    endif()
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

if (ENABLE_ADDRESS_COVERAGE)
    message(STATUS "Using ASAN(address sanitizer) for memory analysis")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

if (ENABLE_MEMORY_COVERAGE)
    message(STATUS "Using MSAN(memory sanitizer) for memory analysis")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fno-omit-frame-pointer -fsanitize-memory-track-origins=2 -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=memory -fno-omit-frame-pointer -fsanitize-memory-track-origins=2 -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")
endif()

if (ENABLE_MEMORY_LEAKAGE)
    message(STATUS "Using LSAN(leak sanitizer) for memory analysis")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -fno-omit-frame-pointer -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak -fno-omit-frame-pointer -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
endif()

if (ENABLE_UNDEFINED_BEHAVIOR)
    message(STATUS "Using UBSAN(undefined sanitizer) for memory analysis")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()

if (ENABLE_THREAD_COVERAGE)
    message(STATUS "Using TSAN(thread sanitizer) for race condition per threads")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()
