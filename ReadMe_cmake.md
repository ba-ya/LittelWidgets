- [区分构建方式](#区分构建方式)
- [添加第三方库CMAKE\_PREFIX\_PATH](#添加第三方库cmake_prefix_path)
- [区分平台CMAKE\_SYSTEM\_NAME](#区分平台cmake_system_name)


# 区分构建方式
```cmake
target_link_directories(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${HAL_LIB}/debug>
    $<$<CONFIG:Release>:${HAL_LIB}/release>
    $<$<CONFIG:Debug>:${OPENCV_PATH}/lib/debug>
    $<$<CONFIG:Release>:${OPENCV_PATH}/lib/release>
)
```

# 添加第三方库CMAKE_PREFIX_PATH

`vkpkg`

```cmake
List(APPEND CMAKE_PREFIX_PATH "D:/software-work/vcpkg-master/installed/x64-windows")
```

# 区分平台CMAKE_SYSTEM_NAME

```cmake
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Platform: Windows")
    #...
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Platform: Linux")
    #...
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif()
```

