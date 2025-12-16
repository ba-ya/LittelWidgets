[TOC]

# *VTK*

# *code*

### 重置视角

```c++
   void do_render(bool is_reset_camera) {
        if (is_reset_camera) {
            auto camera = renderer->GetActiveCamera();
            camera->SetFocalPoint(0, 0, 0); // 这两个点确定camera看的方向.
            camera->SetPosition(0, 0, 100); // 例子是从z轴正方向向原点看 
            camera->SetViewUp(0, 1, 0);// 坐标系y轴正方向指向上方
            renderer->ResetCamera();
        }
        vtk_w->renderWindow()->Render();
    }
```

## 禁用鼠标操作

```c++
// std::shared_ptr<RenderWidget> rw[3];
//	 struct RenderWidget : QVTKOpenGLStereoWidget
// vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin;
 rw[i]->renWin->SetInteractor(nullptr);// 这一句
//QVTKOpenGLStereoWidget -> vtkGenericOpenGLRenderWindow -> SetInteractor(nullptr)
```



# 其他

数据渲染时的流程：
**source数据源→filter过滤器→mapper映射器→actor演员即可视化对象→render渲染器→renderwindow成像窗口→interactor交互**

注意看fliter

### 1.常用函数定义

#### 1.1`vtkSmartPointer` 智能指针

[与`std::shared_ptr`的区别](./add/add2/shard_ptr与vtkSmartPoint的区别.md)

智能指针会自动管理它拥有的引用。当智能指针变量超出范围且不再使用时（例如，当它作为局部变量的函数返回时），它会自动通过递减引用计数来通知对象。通过使用智能指针提供的静态 New（） 方法，原始指针不需要保存对对象的引用，因此不需要调用 Delete（）

```
vtkObjectBase* obj = vtkExampleClass::New();
otherObject->SetExample(obj);
//引用计数现在为 2，因为原始指针和存储在另一个对象中的指针都引用它。当不再需要最初存储对象的原始指针时，将使用 Delete（） 方法删除引用：
obj->Delete();

//使用智能指针可以改为
vtkSmartPointer<vtkObjectBase> obj = vtkSmartPointer<vtkExampleClass>::New();
otherObject->SetExample(obj);

//尽可能多用智能指针
```

#### 1.2`vtkSphereSource`成球体几何数据的类。

是VTK中用于生成球体几何数据的类。它可以生成包含球体的表面或体积的几何数据，以便在渲染器中显示。

#### 1.3`vtkGenericOpenGLRenderWindow`渲染的窗口类

是VTK中用于渲染的窗口类之一。它是一个通用的OpenGL渲染窗口，可以与各种平台和OpenGL版本一起使用。

`vtkGenericOpenGLRenderWindow`是VTK的一个抽象类，实际的渲染窗口由特定的渲染窗口类实现，如`vtkWin32OpenGLRenderWindow`（用于Windows平台）、`vtkCocoaRenderWindow`（用于macOS平台）等。`vtkGenericOpenGLRenderWindow`提供了一个通用的接口，允许在编译时决定使用哪个具体的渲染窗口类，这样可以增加VTK在不同平台上的可移植性。

#### 1.4`QVTKOpenGLStereoWidget`用于在 Qt 应用程序中嵌入 VTK 渲染窗口

`QVTKOpenGLStereoWidget` 是 VTK 为 Qt 提供的一个小部件（widget），用于在 Qt 应用程序中嵌入 VTK 渲染窗口。它是 VTK 中的 `QVTKWidget` 类的一个变体，支持 OpenGL 渲染，同时还支持立体（stereo）渲染），它允许你在 Qt 界面中显示 VTK 渲染结果。。

---------------------------

`setRenderWindow`是 `QVTKOpenGLStereoWidget` 类中的一个成员函数，它不是 Qt 中的标准函数。`setRenderWindow` 函数用于将一个 VTK 渲染窗口与 `QVTKOpenGLStereoWidget` 相关联。VTK 中的渲染窗口通常是一个 `vtkRenderWindow` 类的对象，它负责管理渲染过程、显示图形内容等。通过调用 `setRenderWindow` 函数，你可以将一个 `vtkRenderWindow` 对象与 `QVTKOpenGLStereoWidget` 关联起来，从而在 Qt 界面中显示 VTK 渲染结果。

```c++
// 创建VTK渲染窗口
        renWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
```

![image-20240313110008773](.\2vtk.assets\image-20240313110008773.png)

```c++
MyVTKApp::MyVTKApp(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyVTKApp)
{
    ui->setupUi(this);

    //MyVTKApp类中的私有成员
    //std::shared_ptr<RenderWidget> vtk_window;
    vtk_window = std::make_shared<RenderWidget>();
    ui->vtk_layout->addWidget(vtk_window.get());

    create_ball();
}
```

[make_shard](./add/C++)

#### 1.5`vtkProp` 是 VTK库中用于表示可视化对象（或者称之为图元、图元组件）的基类。

它继承自 `vtkObject`，因此具有了 VTK 对象的一些基本特性，比如引用计数和对象的类型信息。

在 VTK 中，几乎所有的可视化对象都是 `vtkProp` 或其子类的实例。`vtkProp` 提供了一些基本的属性和方法，用于描述和操作可视化对象的外观、位置、姿态等信息。例如，它可以表示一个简单的几何图形（如球体、立方体）、复杂的几何对象（如三角网格模型）、文本、图像等。

`vtkProp` 类具有以下一些重要的特点和方法：

1. **属性设置**：`vtkProp` 提供了一系列方法，用于设置对象的属性，如颜色、不透明度、显示状态等。这些属性会影响对象在渲染窗口中的呈现效果。
2. **变换**：`vtkProp` 具有 `SetPosition()`、`SetOrientation()`、`SetScale()` 等方法，可以对对象进行平移、旋转和缩放等变换操作，从而控制对象在场景中的位置和姿态。
3. **渲染**：`vtkProp` 具有 `Render()` 方法，用于在渲染窗口中绘制对象。
4. **选择性渲染**：`vtkProp` 可以通过 `Pick()` 方法来实现对象的选择性渲染，即只渲染指定范围内的对象，从而提高渲染效率。

#### 1.6`SetRepresentationToWireframe()` 用于设置渲染对象的表示方式为线框模式。

是 VTK 中 `vtkProperty` 类的一个成员函数，渲染的对象将会以线框的形式显示在场景中，而不是实心的形式。

这在某些情况下可以用于更清晰地显示对象的几何形状和结构，例如在调试、分析或演示时。



### [2.example](./add/2.examples)

