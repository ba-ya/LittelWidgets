# 配置

## 1.工作区选择项目目录

切换工作区可以在终端里面输入`code .`

## 2.json配置

launch.json可以点击左边'运行和调试按钮',在创建launch文件

![image-20260423112224315](D:\proj\LittelWidgets\2GPU编程\vscode远程244.assets\image-20260423112224315.png)

下面是一些参考配置,具体看[CUDA 番外篇 | Visual Studio Code的CUDA环境 - 知乎](https://zhuanlan.zhihu.com/p/508810115)

#### launch.json

```json
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "CUDA C++: Launch",
            "type": "cuda-gdb",
            "request": "launch",
            "program": "${fileDirname}/test",
            "args": [],
            "stopAtEntry": false,
            "preLaunchTask": "build CUDA"
        },
        {
            "name": "CUDA C++: Attach",
            "type": "cuda-gdb",
            "request": "attach"
        }
    ]
}
```

#### tasks.json

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build CUDA",
            "type": "shell",
            "command": "nvcc",
            "args": [
                "-g",
                "-G",
                "-o",
                "${fileDirname}/test",
                "${file}"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": "$msCompile",
            "detail": "Compile CUDA source with nvcc"
        }
    ]
}
```

#### c_cpp_properties.json

文件可以ctrl+shift+p输入edit自动生成

![image-20260423144004094](D:\proj\LittelWidgets\2GPU编程\vscode远程244.assets\image-20260423144004094.png)

c++相关头文件可通过在终端输入命令`g++ -v -E -x c++ -`得到

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "/usr/include/c++/11",
                "/usr/include/x86_64-linux-gnu/c++/11",
                "/usr/include/c++/11/backward",
                "/usr/lib/gcc/x86_64-linux-gnu/11/include",
                "/usr/local/include",
                "/usr/include/x86_64-linux-gnu",
                "/usr/include",
                "/usr/local/cuda-12.8/include",
                "${workspaceFolder}/**"
            ],
            "defines": [],
            // "compilerPath": "/usr/local/cuda-12.8/bin/nvcc",
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "gnu17",
            "cppStandard": "gnu++14", 
            "intelliSenseMode": "linux-gcc-x64",
            "configurationProvider": "ms-vscode.makefile-tools"
        }
    ],
    "version": 4
}
```

## 3.运行

#### 终端输入

```
nvcc -o test main.cu
./test
```

test是生成的可执行文件,可随意命名.main.cu则是cu文件,根据实际即可

#### 运行与调试按钮

目前配置必须打断点,在launch配置下运行

![image-20260423112801898](D:\proj\LittelWidgets\2GPU编程\vscode远程244.assets\image-20260423112801898.png)

在launch.json里面设置"stopAtEntry": false,就可以直接运行了

具体看官网文档[在 Linux 上使用 VS Code 开发 C++ - VSCode · AI 代码编辑器](https://vscode.js.cn/docs/cpp/config-linux)

# 其他

### 自动补全

把c/c++扩展卸载重装,再在c_cpp_properties.json把includePath相关目录补全

在远程工作区,智能感知因引擎打开

![image-20260423144212826](D:\proj\LittelWidgets\2GPU编程\vscode远程244.assets\image-20260423144212826.png)