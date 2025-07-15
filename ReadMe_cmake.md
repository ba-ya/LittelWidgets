- [区分构建方式](#区分构建方式)


# 区分构建方式
```cmake
target_link_directories(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:${HAL_LIB}/debug>
    $<$<CONFIG:Release>:${HAL_LIB}/release>
    $<$<CONFIG:Debug>:${OPENCV_PATH}/lib/debug>
    $<$<CONFIG:Release>:${OPENCV_PATH}/lib/release>
)
```

