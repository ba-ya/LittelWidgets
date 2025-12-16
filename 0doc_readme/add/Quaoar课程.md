[TOC]

# Quaoar课程

## 10课: OpenCascade OCAF 第 1 部分：使用 OCAF 的第一步

##                   ----------------------------------------------------------

## 17.1 课：使用 OpenCascade 进行点成员资格分类

PMC

`TopAbs_State`

```
TopAbs_State枚举描述了一个顶点或一组顶点相对于区域的位置。有四个术语：
IN ON OUT UNKNOWN
```

`TopoDS`

```
将形状分解为一组更简单的形状的知识。这其实就是 TopoDS 包的目的。
TopoDS_TShape类是所有形状描述的根。它包含形状列表。如有必要，继承TopoDS_TShape的类可以携带几何域的说明（例如，与 TVertex 关联的几何点）。TopoDS_TShape是形状在其定义参考系中的描述。此类通过引用进行操作。

TopoDS_Shape类描述对形状的引用。它包含对基础抽象形状的引用、方向和局部参考坐标。此类由值操作，因此无法共享。

TopoDS 包提供了两组类，一组继承了既没有方向也没有位置的基础形状，另一组继承了TopoDS_Shape，它们表示 TopAbs 包中枚举的标准拓扑形状。
```



![image-20240319101930464](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240319101930464.png)





## 14课 使用OpenCasCade使多边形相交

在VIS中显示出交点(黄色),感觉是固定写法

```cpp
    Intf_InterferencePolygon2d algo(poly0, poly1);
    const int numPts = algo.NbSectionPoints();
   
    //绘制出两条折线的交点
        //interference solusion
    for (int isol = 1; isol <= numPts; ++isol)
    {
        gp_Pnt2d sol2d = algo.Pnt2dValue(isol);//二维点
        gp_Pnt   sol3d = SimplePolygon::PLANE->Value(sol2d.X(), sol2d.Y());
        
        vout << BRepBuilderAPI_MakeVertex(sol3d);
    }

    std::cout << "交叉点的个数:" << numPts << std::endl;

    vout.StartMessageLoop();
//调用 StartMessageLoop() 方法会启动一个消息循环，使得程序可以接收并处理用户输入、系统消息等。这样可以保证程序在没有用户交互时不会立即退出，而是等待用户操作或者系统消息到来。
```

![image-20240319150409568](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240319150409568.png)



增加一个在该平面的圆去求交点

```cpp
    Viewer vout(50, 50, 500, 500);

    SimplePolygon::PLANE = new Geom_Plane(gp::XOY());

    //两条折线
    SimplePolygon poly0{ {0.0, 0.0}, {1.0, 0.0}, {0.25, 0.75}},
                  poly1{ {0.5, 1.2}, {0.5, -1.0}, {1.0, 3.0} };

    //圆形
    gp_Pnt circleCenter = SimplePolygon::PLANE->Location();
    gp_Dir circleNormal = SimplePolygon::PLANE->Axis().Direction();
    gp_Circ circleCirc(gp_Ax2(circleCenter, circleNormal), 0.3);

    //--------将gp_Circ转化为TopoDS_Shape类型-------------------
    // 使用 BRepBuilderAPI_MakeEdge 类创建圆的边缘
    BRepBuilderAPI_MakeEdge edgeMaker(circleCirc);

    // 获取圆的边缘
    TopoDS_Edge circleEdge = edgeMaker.Edge();

    // 将圆的边缘放入 TopoDS_Wire 中
    BRepBuilderAPI_MakeWire wireMaker(circleEdge);

    // 获取包含圆的边缘的线
    TopoDS_Wire circleWire = wireMaker.Wire();

    // 将 TopoDS_Wire 转换为 TopoDS_Shape
    TopoDS_Shape circleShape = circleWire;
   //----------------------------
    
    //将<<操作符重载了 对TopoDS_Shape进行操作
    vout << poly0 << poly1 << circleShape;
```



##                             ----------------------------------------------------------

## 02课 将OpenCasCade与VTK结合

`TopoDS_Shape` topo data struction

## 07 课：OpenCascade 和 CAD 建模内核简介 |什么是 OpenCascade

[ppt](D:\0work\CODE2\lessons-master\docs)

### 建模形式

![image-20240318104250543](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318104250543.png)

**构造几何实体**(CSG)   基础遗留的

物体由一组基元进行布尔操作得到(一组块 圆柱体 球等)

**边界表示(Brep) **   更多元 主流

PMC

![image-20240318104717724](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318104717724.png)

**刻面建模**  facetting

不需要参数方程 需要的只是某种非结构网格

缺点:为了提高效率而牺牲了精度

**体素建模**  voxel

总而言之:brep是主流的建模方式,原因:所见即所得：屏幕上显示的每个 EDGE 和 FACE 都显式保存在模型的数据结构中，使对象的编辑和询问变得简单明了。这似乎是一个微不足道的点，但它使属性（如加工公差）与元素的附加变得非常容易

## 08课  OCC库

![image-20240318105806986](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318105806986.png)



OCC库的真正功能是进行数学运算,有几十个图形库（如果不是数百个），但实体建模库很少，而 Open CASCADE 是唯一开源的实体建模器。

### MODULES

#### 1.基础模块

![image-20240318110139942](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318110139942.png)

- 智能指针
- 内存管理
- 操作系统相关:比如文件系统;线程进程处理;跨平台库
- 数学内核
- 基础服务

------------------------

![image-20240318113103390](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318113103390.png)

基础模块在项目中显示为TK内核和TK数学

![image-20240318113832383](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318113832383.png)

#### 2.建模

![image-20240318110727626](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318110727626.png)

- 实体建模
- 表面面建模

都是使用Brep边界表示

#### 3.数据交换

![image-20240318110958459](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318110958459.png)

两个转换器,都可以读和写,可以读取几何图形还可以读取一些元数据

- step

- IGES

OCC最常用的就是数据交换功能是读取和写入step,不仅能读取数据,还能读取装配层次结构颜色名称和图层,但不能存储任何用户定义的属性---因此需要使用OCAF

#### 4.啮合mesh

![image-20240318111245859](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318111245859.png)

想要显示图形,不会一开始就渲染brep而是从分析形状再切换到面

#### 5.OCAF

![image-20240318111554405](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318111554405.png)

很方便,可以看做快速应用程序开发框架

open/save:可以从文件系统读取工程数据,然后将其序列化回到文件系统

undo/redo事务机制:出现异常或错误时,可以回滚到相应事务

-----

可视为一种分层数据库引擎

![image-20240318115412619](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318115412619.png)

#### 6.可视化

![image-20240318112001793](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318112001793.png)

- AIS

- mesh VS

用于可视化的应用程序交互服务和可视化网格的mesh VS小框架

---

---

![image-20240318115634962](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318115634962.png)

## 09 课：在 DRAW 中使用 OpenCascade 进行原型设计 |在 OpenCascade 中编写脚本

### 使用draw

用cmake打开,按f5使用

![image-20240318125433202](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318125433202.png)

### axo-轴测视图

适合显示不完美的几何图形

```'
Draw[16]> box b  2 2 2
Draw[17]> whatis b
b is a shape SOLID FORWARD Free Modified

Draw[18]>
Draw[19]> axo
Draw[20]> clear
Draw[21]> donly b
b
Draw[22]> fit
Draw[23]>
```

![image-20240318125721731](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318125721731.png)

黄色代表是两个面共享的线

```
键盘驱动,不可拖拽
Draw[44]> u
Draw[45]> d
u----向上
d----向下
Draw[50]> l
Draw[51]> r
l----向左
r----向右
```

### vis

如果是构造异常的几何图形,可能构造失败,异常不完美的建议使用axo![image-20240318132214380](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318132214380.png)

```
vinit
vdisplay b


ctrl+w 线框模式,如上图  对应 setup WireFrame display mode
ctrl+s 阴影模式,如下图  对应 setup Shaded display mode
f 适配窗口大小


```

![image-20240318132553536](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240318132553536.png)



#### 创建棱镜

`总`

```
Draw[1]> pload ALL
1
Draw[2]> top
Draw[3]> vertex v1 0 0 0
Draw[4]> vertex v2 10 0 0
Draw[5]> vertex v3 10 10 0
Draw[6]> vertex v4 0 10 0
Draw[7]> fit
Draw[8]> vertex v3 7 8 0
Draw[9]> fit
Draw[10]> top
Draw[11]> fit
Draw[12]> fit
Draw[13]> edge e1 v1 v2
Draw[14]> edge e2 v2 v3
Draw[15]> edge e3 v3 v4
Draw[16]> edge e4 v4 v1
Draw[17]> wire w e1 e2 e3 e4
Draw[18]> whatis w
```

```
Draw[19]> plane p 0 0 0 0 0 1
Draw[20]> whatis p
p is a a surface

Draw[21]> fit
//无限的平面,需要使用轮廓去限制
```

![image-20240320105807061](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240320105807061.png)

```
Draw[22]> mkface f p w
Draw[23]> whatis f
f is a shape FACE FORWARD Free Modified Orientable

Draw[24]> donly f
f
Draw[25]> fit
```

![image-20240320105934681](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240320105934681.png)

 棱镜

```
Draw[26]> axo
Draw[27]> fit
Draw[28]> prism prism f 0 0 1
Draw[29]> prism prism f 0 0 2
Draw[30]> prism prism f 0 0 3
Draw[31]> prism prism f 1 1 3
Draw[32]> axo;fit
Draw[33]> vdisplay prism
```

![image-20240320134502007](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240320134502007.png)

`想要把其中一条边改成圆角`

```
Draw[35]> explode prism e
prism_1 prism_2 prism_3 prism_4 prism_5 prism_6 prism_7 prism_8 prism_9 prism_10 prism_11 prism_12
Draw[36]> help explode
explode         : explode name [Cd/C/So/Sh/F/W/E/V]
Draw[37]>
//显示所有边e 

Draw[37]> erase .
Pick an object
Draw[39]> erase .
Pick an object
prism_12
Draw[40]> erase .
Pick an object
prism_4
//交互边来拿查看需要的边名称

```

![image-20240320135105673](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240320135105673.png)



```
Draw[43]> renamevar prism_4 e4
Draw[44]> renamevar prism_12 e12
Draw[45]>
//修改名称,两条边又重新显示了
```

![image-20240320135428318](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240320135428318.png)



```
Draw[45]> help fillet
fillet          :  Perform fillet on compounds of edges :fillet result object rad1 comp1 rad2 comp2 ...
Draw[47]> fillet res prism 1 e4
Draw[48]> fillet res prism 1 e4 2 e12
```

![image-20240320135915328](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240320135915328.png)

```
Draw[57]> axo;fit
Draw[58]> vdisplay res
Display res

Draw[62]> whatis .
Pick an object
res is a shape COMPOUND FORWARD Free Modified

Draw[63]> explode res
res_1
Draw[64]> whatis res_1
res_1 is a shape SOLID FORWARD Modified

Draw[65]> help explode
explode         : explode name [Cd/C/So/Sh/F/W/E/V]
Draw[66]> tolerance res_1
Tolerance MAX=1.557294273268895e-05 AVG=2.9930970020562143e-06 MIN=9.9999999999999995e-08
FACE    : MAX=9.9999999999999995e-08 AVG=9.9999999999999995e-08 MIN=9.9999999999999995e-08
EDGE    : MAX=1.5572942732688944e-05 AVG=1.3049252908535856e-06 MIN=9.9999999999999995e-08
VERTEX  : MAX=1.557294273268895e-05 AVG=4.1586380801082181e-06 MIN=9.9999999999999995e-08
```

![image-20240320140249844](D:\0work\学习资料\readme\add\Quaoar课程.assets\image-20240320140249844.png)

### 其他

#### 打印图形信息

```

Draw[77]> nbshape b
Number of shapes in b
 VERTEX    : 8    //8个顶点
 EDGE      : 12   //12边
 WIRE      : 6    //6条线
 FACE      : 6    //6条面线,蓝色的
 SHELL     : 1
 SOLID     : 1
 COMPSOLID : 0
 COMPOUND  : 0
 SHAPE     : 34  //34个形状
```

#### 获取源代码

```
Draw[79]> getsource box
box                  src/BRepTest/BRepTest_PrimitiveCommands.cxx
```

#### help获取帮助

```
Draw[21]> help wire
wire            : wire wirename [-unsorted] e1/w1 [e2/w2 ...]
Draw[22]> help edge
edge            : edge edgename v1 v2
Draw[23]>
```

#### del

```
Draw[34]> help del
del             : unset (remove) variables matched by glob pattern
Draw[35]> del v5
v5
```

