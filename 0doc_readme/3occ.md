[TOC]

## [Quaoar课程](./add/Quaoar课程.md)

## [VS中配置VTK OCC](./add/VS中配置VTK)

## [常用简写](./add/简写)

## 0.[文档](https://dev.opencascade.org/doc/overview/html/index.html)主要看User Guides

熟悉step-stl-vtk(?)流程

![image-20240314094128978](D:\0work\学习资料\readme_doc\3occ.assets\image-20240314094128978.png)

### 0.1基础课程

`数据类型分为两类	`

- 由句柄（或引用）操作的数据类型
- 按值操作的数据类型

在Open CASCADE技术中，句柄`Handle(MyClass) anObject = new MyClass();`是特定的类，用于安全地操作通过引用在动态内存中分配的对象，提供引用计数机制并在对象未被引用时自动销毁该对象。

| C++ Types | OCCT Types         |
| :-------- | :----------------- |
| int       | Standard_Integer   |
| double    | Standard_Real      |
| float     | Standard_ShortReal |
| bool      | Standard_Boolean   |
| char      | Standard_Character |
| char16_t  | Standard_Utf16Char |
| char*     | Standard_CString   |
| void*     | Standard_Address   |
| char16_t* | Standard_ExtString |

`什么时候需要使用handle？`

当您设计一个对象时，可能很难选择如何操作该对象：按值还是按句柄。以下想法可以帮助您做出决定：

- 如果您的对象在应用程序中可能有很长的生命周期，并且您想要对其进行多次引用，那么最好使用句柄来操作该对象。对象的内存将在堆上分配。指向该内存的句柄是一个轻量对象，可以在参数中快速传递。这避免了复制大对象的损失。
- 如果您的对象的生命周期有限，例如在单个算法中使用，则最好按值操作该对象，而不考虑其大小，因为该对象是在堆栈上分配的，并且分配和取消分配内存的释放速度非常快，这避免了由于在堆上分配而引起的对*new*和*delete 的*隐式调用。
- 最后，如果一个对象仅在应用程序的整个生命周期中创建一次，但在整个应用程序的整个生命周期中都存在，那么最好的选择可能是由句柄操作的类或声明为全局变量的值。

`使用句柄编程`

句柄是OCC的智能指针;

与指针不同，**删除**运算符不适用于句柄；当不再使用时，引用的对象将自动销毁。







### 0.2数据建模

### 0.3建模算法

#### 约束中的直线与圆

对于每一种受限线或圆的几何构造，OCCT 提供两种类型的访问：

- *Geom2dGcc*包中的算法会自动选择最适合问题的算法，无论是在一般情况还是所有类型的特定情况下；使用的参数是*Geom2d*对象，而计算出的解决方案是*gp*对象；
- *GccAna*包中的算法可以分析解决问题，并且只有当要处理的几何形状是线或圆时才能使用；使用的参数和计算出的解都是*gp*对象。

所提供的算法计算与所述几何问题相对应的所有解，除非通过迭代算法找到解。

迭代算法仅计算最接近初始位置的一个解决方案。它们可用于以下情况：

- 当一个参数比一条线或一个圆更复杂，并且半径未知或难以确定时，构造一个圆：这是与三个几何元素相切的圆，或与两个几何元素相切并以曲线为中心的圆的情况；
- 当切线参数比线或圆更复杂时，构建一条线。

#### 约束中的曲线与曲面

```cpp
Standard_Integer NbCurFront=4, 
NbPointConstraint=1; // 定义整型变量 NbCurFront 和 NbPointConstraint，分别初始化为 4 和 1
gp_Pnt P1(0.,0.,0.); // 创建点 P1，并初始化坐标为 (0, 0, 0)
gp_Pnt P2(0.,10.,0.); // 创建点 P2，并初始化坐标为 (0, 10, 0)
gp_Pnt P3(0.,10.,10.); // 创建点 P3，并初始化坐标为 (0, 10, 10)
gp_Pnt P4(0.,0.,10.); // 创建点 P4，并初始化坐标为 (0, 0, 10)
gp_Pnt P5(5.,5.,5.); // 创建点 P5，并初始化坐标为 (5, 5, 5)

BRepBuilderAPI_MakePolygon W; // 创建 BRepBuilderAPI_MakePolygon 对象 W，用于构造多边形
W.Add(P1); // 将点 P1 添加到多边形中
W.Add(P2); // 将点 P2 添加到多边形中
W.Add(P3); // 将点 P3 添加到多边形中
W.Add(P4); // 将点 P4 添加到多边形中
W.Add(P1); // 将点 P1 添加到多边形中，形成闭合多边形

// 初始化一个 BuildPlateSurface
GeomPlate_BuildPlateSurface BPSurf(3,15,2);
// 创建一个 BuildPlateSurface 对象，初始化参数为 3, 15, 2
/*
3：表示建立平面拟合的次数。该参数指定了对曲面进行拟合的程度。
较高的数值意味着更高的拟合次数，因此表面将更接近原始曲线。
15：表示参数 v 的步长。这是在参数空间中创建均匀网格所使用的参数 v 的步长。步长越小，生成的网格越密集。
2：表示参数 u 的步长。与上述类似，这是在参数空间中创建均匀网格所使用的参数 u 的步长。
*/

// 创建曲线约束
BRepTools_WireExplorer anExp; // 创建 BRepTools_WireExplorer 对象
for(anExp.Init(W); anExp.More(); anExp.Next()) // 循环遍历多边形的边
{ 
    TopoDS_Edge E = anExp.Current(); // 获取当前边
    Handle(BRepAdaptor_HCurve) C = new BRepAdaptor_HCurve(); // 创建 BRepAdaptor_HCurve 对象
    C->ChangeCurve().Initialize(E); // 初始化曲线对象
    Handle(BRepFill_CurveConstraint) Cont= new BRepFill_CurveConstraint(C,0); // 创建 BRepFill_CurveConstraint 对象
    BPSurf.Add(Cont); // 将曲线约束添加到 BuildPlateSurface 中
}

// 点约束
Handle(GeomPlate_PointConstraint) PCont= new GeomPlate_PointConstraint(P5,0); // 创建 GeomPlate_PointConstraint 对象，表示点约束
BPSurf.Add(PCont); // 将点约束添加到 BuildPlateSurface 中

// 计算 Plate 表面
BPSurf.Perform(); // 执行 BuildPlateSurface 计算

// Plate 表面的近似
Standard_Integer MaxSeg=9; // 定义最大线段数
Standard_Integer MaxDegree=8; // 定义最大阶数
Standard_Integer CritOrder=0; // 定义关键次数
Standard_Real dmax,Tol; // 定义实数变量 dmax 和 Tol
Handle(GeomPlate_Surface) PSurf = BPSurf.Surface(); // 获取 BuildPlateSurface 的表面
dmax = Max(0.0001,10*BPSurf.G0Error()); // 计算 dmax
Tol=0.0001; // 设置公差 Tol
GeomPlate_MakeApprox Mapp(PSurf,Tol,MaxSeg,MaxDegree,dmax,CritOrder); // 创建 GeomPlate_MakeApprox 对象
Handle (Geom_Surface) Surf (Mapp.Surface()); // 获取近似表面

// 创建与近似 Plate 表面对应的面
Standard_Real Umin, Umax, Vmin, Vmax; // 定义参数 Umin、Umax、Vmin、Vmax
PSurf->Bounds( Umin, Umax, Vmin, Vmax); // 获取参数范围
BRepBuilderAPI_MakeFace MF(Surf,Umin, Umax, Vmin, Vmax); // 创建面对象 MF

```

#### 标准拓扑结构中的EDGE边缘

以下示例创建一个以原点为中心、尺寸为 H、L 且圆角半径为 R 的矩形。边和顶点存储在数组*theEdges*和*theVertices*中。我们使用类*Array1OfShape*（即不是边或顶点的数组）。参见下图。

![img](https://dev.opencascade.org/doc/overview/html/modeling_algos_image024.png)

```cpp
#include <BRepBuilderAPI_MakeEdge.hxx> 
#include <TopoDS_Shape.hxx> 
#include <gp_Circ.hxx> 
#include <gp.hxx> 
#include <TopoDS_Wire.hxx> 
#include <TopTools_Array1OfShape.hxx> 
#include <BRepBuilderAPI_MakeWire.hxx> 

// 使用 MakeArc 方法创建一个边和两个顶点
void MakeArc(Standard_Real x, Standard_Real y,
             Standard_Real R,
             Standard_Real ang,
             TopoDS_Shape& E,
             TopoDS_Shape& V1,
             TopoDS_Shape& V2)
{
    gp_Ax2 Origin = gp::XOY(); // 创建一个默认的坐标系
    gp_Vec Offset(x, y, 0.); // 创建一个位移向量，用于在 XOY 平面上移动坐标系的原点
    Origin.Translate(Offset); // 将坐标系原点移动到指定位置
    BRepBuilderAPI_MakeEdge ME(gp_Circ(Origin, R), ang, ang + PI / 2); // 使用圆心、半径和起始角度构建一个弧边
    E = ME; // 返回构建的边
    V1 = ME.Vertex1(); // 返回起点顶点
    V2 = ME.Vertex2(); // 返回终点顶点
}

// 创建圆角矩形的函数
TopoDS_Wire MakeFilletedRectangle(const Standard_Real H,
                                  const Standard_Real L,
                                  const Standard_Real R)
{
    TopTools_Array1OfShape theEdges(1, 8); // 创建一个存储边的数组
    TopTools_Array1OfShape theVertices(1, 8); // 创建一个存储顶点的数组

    // 使用上面的 MakeArc 函数创建圆弧边和顶点
    Standard_Real x = L / 2 - R, y = H / 2 - R;
    MakeArc(x, -y, R, 3. * PI / 2., theEdges(2), theVertices(2), theVertices(3)); //右下 
    MakeArc(x, y, R, 0., theEdges(4), theVertices(4), theVertices(5)); // 右上角
    MakeArc(-x, y, R, PI / 2., theEdges(6), theVertices(6), theVertices(7)); // 左上角
    MakeArc(-x, -y, R, PI, theEdges(8), theVertices(8), theVertices(1)); // 左下角

    // 创建直线边
    for (Standard_Integer i = 1; i <= 7; i += 2)
    {
        theEdges(i) = BRepBuilderAPI_MakeEdge(TopoDS::Vertex(theVertices(i)), TopoDS::Vertex(theVertices(i + 1)));
    }

    // 使用 BRepBuilderAPI_MakeWire 创建一个线框
    BRepBuilderAPI_MakeWire MW;
    for (Standard_Integer i = 1; i <= 8; i++)
    {
        MW.Add(TopoDS::Edge(theEdges(i)));
    }
    return MW.Wire(); // 返回创建的线框
}

```

#### 缝纫saw

![img](https://dev.opencascade.org/doc/overview/html/modeling_algos_image050.png)

```cpp
// 创建一个长方体
TopoDS_Shape S = BRepPrimAPI_MakeBox(400., 250., 300.);

// 初始化面的探测器，用于在长方体中探测面
TopExp_Explorer Ex;
Ex.Init(S, TopAbs_FACE);
Ex.Next(); // 跳过长方体的第一个面
Ex.Next(); // 获取长方体的第二个面
TopoDS_Face F1 = TopoDS::Face(Ex.Current()); // 获取第二个面

// 获取面F1的几何表面
Handle(Geom_Surface) surf = BRep_Tool::Surface(F1);

// 创建一个线
BRepBuilderAPI_MakeWire MW1;
gp_Pnt2d p1, p2;
p1 = gp_Pnt2d(100., 100.);
p2 = gp_Pnt2d(200., 100.);
Handle(Geom2d_Line) aline = GCE2d_MakeLine(p1, p2).Value();

// 将线转换为边，并添加到线性容器MW1中
MW1.Add(BRepBuilderAPI_MakeEdge(aline, surf, 0., p1.Distance(p2)));

// 重置点，创建新的线，添加到线性容器MW1中
p1 = p2;
p2 = gp_Pnt2d(150., 200.);
aline = GCE2d_MakeLine(p1, p2).Value();
MW1.Add(BRepBuilderAPI_MakeEdge(aline, surf, 0., p1.Distance(p2)));

// 重置点，创建新的线，添加到线性容器MW1中
p1 = p2;
p2 = gp_Pnt2d(100., 100.);
aline = GCE2d_MakeLine(p1, p2).Value();
MW1.Add(BRepBuilderAPI_MakeEdge(aline, surf, 0., p1.Distance(p2)));

// 创建一个面并初始化
BRepBuilderAPI_MakeFace MKF1;
MKF1.Init(surf, Standard_False);
MKF1.Add(MW1.Wire());
TopoDS_Face FP = MKF1.Face();

// 生成三维曲线
BRepLib::BuildCurves3d(FP);

// 创建一个三个控制点的数组
TColgp_Array1OfPnt CurvePoles(1, 3);
gp_Pnt pt = gp_Pnt(150., 0., 150.);
CurvePoles(1) = pt;
pt = gp_Pnt(200., 100., 150.);
CurvePoles(2) = pt;
pt = gp_Pnt(150., 200., 150.);
CurvePoles(3) = pt;

// 创建一个三次贝塞尔曲线
Handle(Geom_BezierCurve) curve = new Geom_BezierCurve(CurvePoles);

// 创建边
TopoDS_Edge E = BRepBuilderAPI_MakeEdge(curve);

// 创建线
TopoDS_Wire W = BRepBuilderAPI_MakeWire(E);

// 使用长方体、面FP、面F1和线W创建管道特征
BRepFeat_MakePipe MKPipe(S, FP, F1, W, Standard_False, Standard_True);

// 执行管道特征操作
MKPipe.Perform();

// 获取操作后的形状
TopoDS_Shape res1 = MKPipe.Shape();
```





## 1.例子

### 1.1[OCCQT](D:\0work\CODE2\occQt-master)(from github)

```
makeBox: 创建一个立方体的测试函数。
makeCone: 创建一个圆锥的测试函数。
makeSphere: 创建一个球体的测试函数。
makeCylinder: 创建一个圆柱体的测试函数。
makeTorus: 创建一个圆环体的测试函数。
makeFillet: 带有圆角的立方体
makeChamfer: 带有倒角的立方体
makeExtrude: 测试挤出算法的函数。
makeRevol: 测试旋转算法的函数。
makeLoft: 测试放样算法的函数。
testCut: 测试布尔切割操作的函数。
testFuse: 测试布尔融合操作的函数。
testCommon: 测试布尔相交操作的函数。
testHelix: 测试螺旋线的函数。
```



#### `TopoDS_Shape`

`TopoDS_Shape `是OpenCASCADE中表示任何几何形状的基类。它是一个通用的几何形状类型，可以代表诸如点、线、曲线、曲面、体积等的几何对象。在OpenCASCADE中，所有的几何对象都可以用 `TopoDS_Shape` 类来表示。

`TopoDS_Shape` 类是一个抽象类，不能直接实例化。它有多个派生类，分别用于表示不同类型的几何形状，包括：

- `TopoDS_Vertex`: 表示点
- `TopoDS_Edge`: 表示边
- `TopoDS_Wire`: 表示线
- `TopoDS_Face`: 表示面
- `TopoDS_Shell`: 表示壳
- `TopoDS_Solid`: 表示实体
- `TopoDS_Compound`: 表示组合体

在使用OpenCASCADE进行几何建模时，通常会使用 `TopoDS_Shape` 及其派生类来构建和操作几何对象。通过这些类，可以对几何对象进行创建、修改、查询和显示等操作。

#### `AIS`

在OpenCASCADE中，AIS（Interactive Application Structure，交互式应用程序结构）是用于交互式显示和操作几何对象的框架。AIS 类型是OpenCASCADE中的一组类，它们提供了一种机制，使得用户能够在3D场景中对几何对象进行可视化、选择和编辑。

1. **AIS_InteractiveContext**: 这是AIS框架的核心类之一。它管理着所有可显示的对象，并处理与用户交互相关的操作，如选择、拾取和修改对象。
2. **AIS_Shape**: 这个类用于在3D场景中显示几何形状。你可以创建一个AIS_Shape对象，并将几何形状（例如BRep的几何体）传递给它，然后AIS_Shape对象会在3D场景中显示该几何形状，并提供一些交互功能。
3. **AIS_Point**: 这个类用于在3D场景中显示点。它是AIS_Shape的一个特定子类，专门用于表示点。
4. **AIS_Line**: 这个类用于在3D场景中显示直线。类似地，它是AIS_Shape的一个特定子类，专门用于表示直线。
5. **AIS_Circle**: 这个类用于在3D场景中显示圆。同样地，它是AIS_Shape的一个特定子类，专门用于表示圆。
6. **AIS_TextLabel**: 这个类用于在3D场景中显示文本标签。它可以显示文本字符串，并提供一些样式设置选项。



#### `BRepPrimAPI_Makexx`

#### a.[makebox](./add/OCCQT.md)

--------------------------------------------

`TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();`

当涉及到OpenCASCADE时，类名通常以一种模式构建，该模式提供了关于类的功能的一些线索。在这种情况下，`BRepPrimAPI_MakeBox` 类名可以分解为几个部分：

1. **BRep**: 这代表了 OpenCASCADE 中的 "Boundary Representation"，它是一种表示实体（solid）和几何对象的方法。Boundary Representation 用一系列面来描述实体的外表面，而这些面是由边界曲线组成的。

2. **Prim**: 这表示 "Primitive"，意味着该类是用于创建基本几何形状的。基本几何形状通常是一些简单的、固定的几何对象，如立方体、球体、圆柱体等。

3. **API**: 这代表 "Application Programming Interface"，表示这个类是 OpenCASCADE 提供的一个程序接口，用于在代码中创建几何对象。

4. **MakeBox**: 这是该类的动作或者功能。在这个情况下，`MakeBox` 意味着该类的作用是创建一个盒子（Box）。

因此，`BRepPrimAPI_MakeBox` 类名的意思就是 "Boundary Representation Primitive Application Programming Interface - Make Box"，即一个用于创建盒子的 Boundary Representation 的程序接口。

====================================================================

`      Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);`

在OpenCASCADE中，`Handle` 是一个智能指针类模板，用于管理对象的生命周期。`Handle` 类提供了对对象的引用计数机制，以确保在没有引用时自动销毁对象，从而避免内存泄漏。

`AIS_Shape` 是OpenCASCADE中用于在3D场景中显示几何形状的类。`AIS` 代表 "Interactive Object Framework"，它提供了与用户交互的一组工具和对象，允许你在3D视图中显示、选择和编辑几何对象。

因此，`Handle(AIS_Shape)` 是一个智能指针，用于管理 `AIS_Shape` 类型的对象。在你的代码中，`anAisBox` 是一个指向 `AIS_Shape` 对象的智能指针，它指向一个表示盒子几何形状的 `AIS_Shape` 对象。

======================================================================

`myOccView->getContext()->Display(anAisBox, Standard_True);`

// 将AIS_Shape对象添加到OpenCASCADE视图的上下文中进行显示，第二个参数为True表示立即显示

#### ~~b.[makeCone](./add/OCCQT.md)~~

- `gp_Ax2`: 这是OpenCASCADE中表示轴线的类。在这里，我们使用它来指定圆锥和锥体的轴线。
- `gp_Pnt`: 这是OpenCASCADE中表示点的类。我们使用它来指定轴线的位置。
- `TopoDS_Shape`: 这是OpenCASCADE中表示任何几何形状的基类。在这里，我们使用它来表示圆锥和锥体的几何形状。
- `BRepPrimAPI_MakeCone`: 这是用于创建圆锥和锥体的函数。它接受轴线、底部半径、顶部半径和高度等参数，并返回一个代表所创建几何形状的 `TopoDS_Shape` 对象。
- `Handle(AIS_Shape)`: 这是OpenCASCADE中表示指向 `AIS_Shape` 对象的智能指针的类模板。`AIS_Shape` 类用于在3D场景中显示几何形状，并提供与之交互的功能。

#### c.[makeSphere](./add/OCCQT.md)

这段代码的作用是创建一个球体并将其显示在OpenCASCADE视图中。首先，我们使用 `gp_Ax2` 类创建一个坐标系，指定了球体的中心位置。然后，使用 `BRepPrimAPI_MakeSphere` 函数根据给定的轴线和半径创建球体的拓扑形状。接着，我们创建一个 `AIS_Shape` 对象，将球体的拓扑形状作为参数传递给它。然后，设置球体的显示颜色为蓝色。最后，我们使用 `Display` 函数将球体显示在OpenCASCADE视图中，第二个参数为 `True` 表示立即显示。



### ~~1.2[OCCTOverview](D:\OCCT-7_8_0\samples\qt\OCCTOverview)(OCC7.8.0自带的例子,配置[参考](https://dev.opencascade.org/doc/overview/html/samples_qt_overview.html#autotoc_md6))~~

![image-20240314140813784](D:\0work\学习资料\readme_doc\3occ.assets\image-20240314140813784.png)

OCC用来制作图形,vtk用来渲染显示图形

### 1.3构建一个球和点,求两者的交叉点





### 2.熟悉step-stl-vtk(?)流程

2.1下载freeCAD(是使用OCC写的)参考

2.2流程解释

1. **STEP 文件转换为 STL 文件**：
   - 首先，你需要从 STEP 格式转换为 STL 格式。STEP 格式是一种用于表示三维零件和装配体的标准，而STL 格式则是一种三角形网格格式，通常用于3D打印和图形渲染等领域。你可以使用OpenCascade（OCC）库中的工具或其他软件将STEP文件转换为STL文件。

2. **STL 文件加载到 VTK**：
   - 一旦你有了STL文件，你可以使用VTK库加载它并进行渲染。VTK（Visualization Toolkit）是一个用于三维可视化的强大工具库，它提供了许多用于加载、处理和渲染三维数据的类和方法。

3. **绘制图形和渲染**：
   - 使用VTK库中的类和方法，你可以创建一个可视化场景，并在其中加载STL文件。你可以设置光照、材质、相机位置等属性来调整渲染效果。最后，你可以在应用程序中显示渲染结果。

