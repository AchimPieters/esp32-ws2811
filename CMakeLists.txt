idf_component_register(
    SRCS "ws2811.c"
    REQUIRES driver
    INCLUDE_DIRS "."
)

target_compile_options(${COMPONENT_LIB} PRIVATE -DESP_IDF)
