idf_component_register(
    SRCS "src/SystemTask.cpp"
    INCLUDE_DIRS "include"
    REQUIRES freertos
)

target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_20)
