add_executable(bitmap-testmalloc
        "testmalloc.c"
        "mymalloc.c"
        "bitmap.c")
target_link_libraries(bitmap-testmalloc m)
add_executable(bitmap-harness
        "harness.c"
        "mymalloc.c"
        "bitmap.c")
target_link_libraries(bitmap-harness m)