add_executable(lab3p1
        "lab3p1-sem.c")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(lab3p1 PRIVATE Threads::Threads)