add_executable(lab3p1-sem
        "lab3p1-sem.c")
add_executable(lab3p1-lock
        "lab3p1-lock.c")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lab3p1-sem PRIVATE Threads::Threads)
target_link_libraries(lab3p1-lock PRIVATE Threads::Threads)