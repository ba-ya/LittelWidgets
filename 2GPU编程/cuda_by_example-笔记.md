## C3, cuda简介

### 3.2第一个程序

不能对cudaMalloc返回的指针进行解引用,来读取或写入内存

![image-20260429095029028](D:\proj\LittelWidgets\2GPU编程\cuda_by_example-笔记.assets\image-20260429095029028.png)

总的来说,主机指针只能访问主机代码中的内存,设备指针只能访问设备代码中的内存

FIX ME: 不知道为啥std::cout或者printf打印都少第一个字符

### 3.3查询设备

![image-20260429101938234](D:\proj\LittelWidgets\2GPU编程\cuda_by_example-笔记.assets\image-20260429101938234.png)

```
192.168.115.244属性
-- General Information for device0--
名称:NVIDIA GeForce RTX 5090
计算功能集版本:12.0
时钟频率:2407000
设备复制重叠:Enabled
核执行超时:Enabled
-- Memory Information for device0--
总的全局内存大小:33668988928
总的常量内存大小:65536
最大内存修正量:2147483647
纹理对齐:512
-- SM Information for device0--
SM数量:170
每个SM对应的共享内存大小:102400
每个SM对应的寄存器数量:65536
线程束大小:32
每个线程块对应的最大线程数目:1024
最大线程维度:(1024, 1024, 64)
最大网格维度:(2147483647, 65535, 65535)
```

### 3.4设备属性的使用

```c
int main() {
    cudaDeviceProp prop;
    int dev;

    HANDLE_ERROR(cudaGetDevice(&dev));
    std::cout << "当前设备ID:" << dev << std::endl;

    memset(&prop, 0, sizeof(cudaDeviceProp));
    prop.major = 12;
    prop.minor = 0;
    HANDLE_ERROR( cudaChooseDevice(&dev, &prop));
    std::cout << "最接近版本号12.0的设备id:" << dev << std::endl;
    HANDLE_ERROR( cudaSetDevice(dev));
}
```

cudaChooseDevice函数,寻找最接近prop要求的设备

## C4, CUDA C并行编程

### 4.2.1矢量求和运算

```c
__global__ void add(int *a, int *b, int *c) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) 
        c[i] = a[i] + b[i];
}

int main() {
    int a[N], b[N], c[N];
    int *dev_a, *dev_b, *dev_c;

    // 分配内存
    HANDLE_ERROR( cudaMalloc((void**)&dev_a, N * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_b, N * sizeof(int)));
    HANDLE_ERROR( cudaMalloc((void**)&dev_c, N * sizeof(int)));

    // 初始化a,b数组
    for (int i = 0; i < N; i++) {
        a[i] = -i;
        b[i] = i * i;
    }
    // host->device
    HANDLE_ERROR( cudaMemcpy(dev_a, a, N * sizeof(int), cudaMemcpyHostToDevice));
    HANDLE_ERROR( cudaMemcpy(dev_b, b, N * sizeof(int), cudaMemcpyHostToDevice));
    // kernel
    add<<<N, 1>>>(dev_a, dev_b, dev_c);
    // device->host
    HANDLE_ERROR( cudaMemcpy(c, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost));

    // show rst
    for (int i = 0; i < N; i++) {
        std::cout << a[i] << " + " << b[i] << " = " << c[i] << std::endl;
    }
    // free
    cudaFree(dev_a);
    cudaFree(dev_b);
    cudaFree(dev_c);

    return 0;
}

```

 if (i < N) 记得加限制

### 4.22.2一个有趣的示例集Julia

和CPU版本区别,除kernel函数加`__global__`, 其他函数需要添加`__device__`前缀

![image-20260429161950436](D:\proj\LittelWidgets\2GPU编程\cuda_by_example-笔记.assets\image-20260429161950436.png)

注意:二维,代码y的部分直接复制x,忘记修改,导致图片一直不对,要注意id

远程看不到图形界面,把图片保存为ppm格式,再用vscode扩展查看

tasks里面没有写参数glut相关参数,直接使用终端编译,再执行test,具体操作如下

```
nvcc -o test c4.cu -lglut -lGLU -lGL
```

```
./test
```

点击vscode左边的 result.ppm 文件查看

保存ppm函数如下:

```c
#include <cstdio> 

// 将像素数据保存为 PPM 图片
// filename: 保存的文件名
// pixels: 像素数组指针（通常是 unsigned char*）
// width: 图片宽度
// height: 图片高度
void savePPM(const char* filename, unsigned char* pixels, int width, int height) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        printf("无法创建文件: %s\n", filename);
        return;
    }

    // 1. 写入 PPM 文件头
    // P6 表示二进制格式的 PPM 图片
    fprintf(fp, "P6\n%d %d\n255\n", width, height);

    // 2. 写入像素数据 (RGB 格式)
    // 假设你的 pixels 数组是按照 R, G, B, R, G, B... 的顺序排列的
    // fwrite(pixels, 3, width * height, fp);
    // 将 4 字节 (BGRA) 数据转换为 3 字节 (RGB) 写入
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 4; // 假设源数据是 4 字节对齐
            unsigned char r = pixels[index];
            unsigned char g = pixels[index + 1];
            unsigned char b = pixels[index + 2];
            // 写入 R, G, B (跳过 A)
            fputc(r, fp);
            fputc(g, fp);
            fputc(b, fp);
        }
    }

    fclose(fp);
    printf("图片已成功保存为: %s\n", filename);
}

```

## C5,线程协作

### 5.2并行线程块的分解

<<<N,1>>>变成<<<1, N>>>, 一个块内可以有多个线程

线程块限制65535, 每个线程块限制线程512

线程id = blockIdx.x * blockDim.x + threadIdx.x

**计算线程块**

<<<(N + 127) / 128, 128>>>, 计算大于或等于N的128的最小倍数

**波纹**

FIX ME:生成动画,不过不知道为什么生成的目录不在build目录而是上一级目录

可以生成波纹视频

![image-20260430150111887](D:\proj\LittelWidgets\2GPU编程\cuda_by_example-笔记.assets\image-20260430150111887.png)

注意:

vscode远程不是图形界面.不能直接预览,新加了函数保存为视频

传指针的时候,想要修改指针,需要传指针的指针,类似下面这种

```
void init_video_recording(FILE** ffmpeg_pipe, int width, int height) {
```

### 5.3共享内存和同步

核函数

```c++
__global__ void dot(float *a, float *b, float *c) {
    // 共享内存, 块内共享
    __shared__ float cache[threadsPerBlock];
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    int cacheIdx = threadIdx.x;
    float tmp = 0;
    while (tid < N) {
        tmp += a[tid] * b[tid];
        tid += blockDim.x * gridDim.x;
    }
    // 设置cache中对应位置的值
    cache[cacheIdx] = tmp;
    // 线程同步,保证cache填充完毕才进入下面
    __syncthreads();

    // 归约运算
    int i = blockDim.x / 2;
    while (i != 0) {
        if (cacheIdx < i) {
            cache[cacheIdx] += cache[cacheIdx + i];
        }
        __syncthreads();
        i /= 2;
    }
    if (cacheIdx == 0) {
        c[blockIdx.x] = cache[0];
    }
}
```

输出:

```
Does GPU value 2.57236e+13 = 2.57236e+13?
```

如果归约运算总__syncthreads放到if里面会造成控制流发散,❌

CUDA保证了所有线程都执行了__syncthreads才会执行后面的语句,如果移入if,处理器会挂起

```
 while (i != 0) {
        if (cacheIdx < i) {
            cache[cacheIdx] += cache[cacheIdx + i];
            __syncthreads();
        }
        i /= 2;
    }
```

**julia**

```c++
__global__ void kernel(unsigned char *ptr) {
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    int offset = x + y * blockDim.x * gridDim.x;

    __shared__ float shared[16][16];
    const float period = 128.0f;
    shared[threadIdx.x][threadIdx.y] = 255 * (sinf(x * 2.0f * PI / period) + 1.0f) *
    (sinf(y * 2.0f * PI / period) + 1.0f) / 4.0f;

    // __syncthreads(); // 注释就是julia1,放开就是julia2
    ptr[offset * 4 + 0] = 0;
    ptr[offset * 4 + 1] = shared[15 - threadIdx.x][15-threadIdx.y];
    ptr[offset * 4 + 2] = 0;
    ptr[offset * 4 + 3] = 255;
}
```

![image-20260430163852115](D:\proj\LittelWidgets\2GPU编程\cuda_by_example-笔记.assets\image-20260430163852115.png)

区别就在于一个有__syncthreads,一个没有,Julia1没有就会有一些没同步