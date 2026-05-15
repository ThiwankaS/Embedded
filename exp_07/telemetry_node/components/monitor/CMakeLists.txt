idf_component_register(
    SRCS "src/MonitorTask.cpp"
    INCLUDE_DIRS "include"
    REQUIRES system_manager freertos
)

target_compile_features(${COMPONENT_LIB} PRIVATE cxx_std_20)
