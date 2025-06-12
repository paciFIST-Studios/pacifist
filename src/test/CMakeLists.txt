
target_sources(paciFIST PRIVATE main.cc)
target_sources(paciFIST PRIVATE core/common.h)



target_link_libraries(${PROJECT_NAME} PRIVATE vendor)
