[TOC]



# [*Qt*](./1Qt)

# [*VTK*](./2vtk)

# [*OCC*](./3occ)

# *项目*

## 1.tube

**学习过程**
Qt-VTK-OpenCasCade-

--(具体功能函数-声速：把下面这些参数搞清楚是怎么计算的就行了

![image-20240313093441709](D:\0work\学习资料\readme_doc\C++.assets\image-20240313093441709.png)

![image-20240313142818006](D:\0work\学习资料\readme_doc\readme.assets\image-20240313142818006.png)

VTK与Opencascade都是开源库，下载源文件，添加环境变量，在Pro文件包含path如下

```makefile
# vtk component
VTK_ROOT = $$PWD/../../3rdparty/windows/vtk
win32:CONFIG(debug, debug|release): INCLUDEPATH += $${VTK_ROOT}/install_d/include/vtk-9.1
win32:CONFIG(release, debug|release): INCLUDEPATH += $${VTK_ROOT}/install/include/vtk-9.1

!isEmpty(target.path): INSTALLS += target

win32: INCLUDEPATH += $$PWD/../../3rdparty/windows/include
else:unix:!macx: INCLUDEPATH += /usr/include/opencv4
```

### [0.相关函数](./add/0相关函数-2tof.md)

### [1.外径 内径 长度 声速](./add/0相关函数-2tof.md)

### [2.飞行时间Time of flight](./add/0相关函数-2tof.md)

### 3.看代码

#### 3.1`GeoModel::create_piece_tube()`

通过`GeoModel::create_piece_tube()`函数来修改`create_ball()`函数,具体看[myVTKApp.cpp](./add/2.examples.md)

![image-20240313141239124](D:\0work\学习资料\readme_doc\readme.assets\image-20240313141239124.png)

正式来看` create_piece_tube`

在函数内部，首先使用 VTK 提供的类来创建管道的几何形状，

然后使用 `vtkRotationalExtrusionFilter` 类对几何形状进行旋转挤压，以创建管道的体积。

接着，建了一个 `vtkPolyDataMapper` 对象来将几何数据映射到渲染器，并创建一个 `vtkActor` 对象来呈现几何形状。

最后，使用 `std::make_shared` 创建了一个 `Tube` 对象，并将创建的 `vtkActor` 添加到 `vtk_actor` 向量中，同时将管道的属性信息存储在 `TubeProp` 对象中，并将其赋值给 `Tube` 对象的 `prop` 成员变量。

```cpp
std::shared_ptr<Tube> create_piece_tube(double r_outer, double r_inner, double len)
{
    //存储点坐标的vtkPoints对象
    auto points = vtkSmartPointer<vtkPoints>::New();
    
    //添加管道的四个顶点到points,xyz坐标
    //外径 100; 内径 70
    points->InsertNextPoint(r_inner, 0, len / 2);//70 0 50
    points->InsertNextPoint(r_outer, 0, len / 2);//100 0 50
    points->InsertNextPoint(r_outer, 0, -len / 2);//100 0 -50
    points->InsertNextPoint(r_inner, 0, -len / 2);//70 0 -50
    
    //创建一个vtkCellArray对象,用来存储单元格信息
    auto cells = vtkSmartPointer<vtkCellArray>::New();
    
    //创建一个vtkPolygon对象,表示一个多边形单元格
    auto polygon = vtkSmartPointer<vtkPolygon>::New();
    polygon->GetPointIds()->SetNumberOfIds(4);//设置多边形的顶点数
    for (std::size_t i = 0; i < 4; i++) {
        polygon->GetPointIds()->SetId(i, i);//设置多边形的顶点索引
    }
    cells->InsertNextCell(polygon);//将多边形单元格添加到cells中
    
    //创建一个vtkPolyData对象,用来储存几何数据和属性数据
    auto polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);//<vtkPoints>   点数据
    polydata->SetPolys(cells);//<vtkCellArray>  单元格数据

    // 创建一个vtkRotataionExtrusionFliter对象,旋转挤压体
    auto ext = vtkSmartPointer<vtkRotationalExtrusionFilter>::New();
    ext->CappingOn();//封顶
    ext->SetResolution(100);//分辨率
    ext->SetAngle(180);//挤压角度
    ext->SetRotationAxis(0, 0, 1);//挤压旋转轴
    ext->SetInputData(polydata);//输入数据polydata

    //创建一个vtkPolyDataMapper对象,用于将数据映射到图形上
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(ext->GetOutputPort());
    // 设置输入连接到旋转挤出体的输出端口
    
    //创建对象
    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);//映射器添加到actor
    actor->GetProperty()->SetOpacity(0.5);//透明度
    actor->GetProperty()->SetColor(1, 1, 1);//颜色
    actor->GetProperty()->SetRepresentationToWireframe();//设置渲染对象的表示方式为线框模式。

    //创建一个Tube的智能指针对象,并初始化数据
    auto p = std::make_shared<Tube>();
    p->vtk_actor.push_back(actor);//将actor添加到Tube中的vtk_actor
    
    //创建一个TubeProp的智能指针对象,并初始化数据
    auto pt = std::make_shared<TubeProp>();
    pt->len = len;//设置长度
    pt->r_inner = r_inner;//内径
    pt->r_outer = r_outer;//外径
    p->prop = pt;
    return p;
}
}
```

旋转挤压-[vtk用户指南p231](D:/0work/学习资料/VTK/VTKUsersGuide.pdf)

旋转挤压体（Rotational Extrusion）是一种几何变换，它通过旋转并拉伸一个二维几何形状（通常是一个封闭曲线）来创建一个三维几何体。在 VTK 中，`vtkRotationalExtrusionFilter` 是用于实现旋转挤压体操作的过滤器，它可以将一个二维几何形状沿指定轴进行旋转挤压，从而生成一个三维的几何体。

![image-20240313155325649](D:\0work\学习资料\readme_doc\readme.assets\image-20240313155325649.png)

在 `create_piece_tube` 函数中，过滤器（filter）的作用体现在以下几个地方：

1. **旋转挤压体过滤器**：
   在这个函数中，通过使用 `vtkRotationalExtrusionFilter` 类来创建一个旋转挤压体过滤器。这个过滤器的作用是将一个二维的几何形状沿着指定的旋转轴进行旋转挤压，生成一个三维的几何体。在这里，旋转挤压体过滤器的创建体现了过滤器的应用。

2. **设置过滤器的输入数据**：
   在函数中，通过 `ext->SetInputData(polydata)` 将旋转挤压体过滤器的输入数据设置为 `polydata`。这里的 `polydata` 是一个 `vtkPolyData` 对象，表示二维的几何形状。设置输入数据是过滤器的基本操作之一，它告诉过滤器要对哪些数据进行处理。

3. **过滤器的应用**：
   通过将输入数据设置到旋转挤压体过滤器中，并调用过滤器的相应方法，如设置分辨率、挤压角度、旋转轴等，实际上是在应用过滤器，对输入数据进行处理，并生成新的输出数据。在这个函数中，过滤器的具体操作体现在设置过滤器的各种参数和属性上。

过滤器在 VTK 流程中扮演着重要的角色，它们用于对数据进行处理、转换和修改，以满足特定的需求。在 `create_piece_tube` 函数中，旋转挤压体过滤器的应用使得输入的二维几何形状被转换为了一个三维的旋转挤压体，为后续的数据处理和可视化提供了基础。

#### 3.2`GeoModel::create_probe()`创建探测器:楔块+阵列

```cpp
std::shared_ptr<ProbeWARect> create_probe(
    double wedge_h,      //楔块属性
    double wedge_l,
    double wedge_w,
    double wedge_angle,
    int cnt_x,          //探头阵元数量
    double pitch_x,
    double len_x,
    double len_y,
    double array_1st_x,  //探头阵列第一个阵元的x坐标
    double array_1st_z)  //探头阵列第一个阵元的z坐标
{
    auto array = std::make_shared<ArrayRect>();
    auto wedge = std::make_shared<Wedge>();

    // wedge长 宽
    wedge->length = wedge_l;
    wedge->width = wedge_w;

    auto c_sys_w = std::make_shared<CoordSys>();
    auto w = std::make_shared<ModelWedgeRoof>();  
    //ModelWedgeRoof是namespace GeoModel中的结构体
     // set_wedge_geometry(double height, double length, double width)：设置楔块的几何参数（高度、长度、宽度）。
    // set_wedge_angle(double angle)：设置楔块的角度。
    // set_wedge_angle_roof(double angle)：设置楔块的屋顶角度。
    // set_transducer_position(double x, double y, double z)：设置换能器位置。
    // setup_complete()：完成楔块模型的设置。
    // get_vertex(std::vector<std::vector<Point>>& vertex)：获取楔块的顶点坐标。
    //  c_sys_1、c_sys_2、c_sys_3：楔块的坐标系。  
    
    //楔块的几何属性和位置
    w->set_wedge_geometry(wedge_h, wedge_l, wedge_w);
    w->set_wedge_angle(wedge_angle);
    w->set_wedge_angle_roof(0);
    w->set_transducer_position(array_1st_x, 0, array_1st_z);
    //完成楔块模型的设置
    w->setup_complete();
    //获取楔块的顶点坐标,vertex_w是二维向量,但Point是三维坐标点
    //第一层向量存的是单个模型的顶点坐标,第二层存的是多个模型
    auto vertex_w = std::make_shared<std::vector<std::vector<Point>>>();
    w->get_vertex(*vertex_w);
    
    // 在给定的代码中，`w` 和 `wedge` 都是通过 `std::make_shared` 创建的智能指针，但是它们指向的是不同的类对象。让我们来区分一下它们的作用：
    //     1. `w` 是一个指向 `ModelWedgeRoof` 类对象的智能指针，即 `w` 指向的对象是一个楔块模型对象。这个对象可能包含了单个楔块的几何参数、角度、位置等信息。
    //     2. `wedge` 是一个指向 `Wedge` 类对象的智能指针，即 `wedge` 指向的对象是一个最终楔块模型对象。这个对象可能表示的是一个由多个楔块组成的阵列或结构，可能包含了更多的信息，如多个楔块的顶点坐标、坐标系等。    
    //     因此，`w` 可能代表单个楔块模型，而 `wedge` 可能代表一个包含多个楔块的最终模型。
    //这两个模型都是为了获取顶点坐标和坐标系
    
    //将顶点坐标和坐标系保存到 Wedge 对象中,pw:wedge的Point
    wedge->pw = vertex_w;
    wedge->ca_1 = w->c_sys_1;
    wedge->ca_2 = w->c_sys_2;
    wedge->ca_3 = w->c_sys_3;
    
    //ModelArrayRect同样是namespace GeoModel中的结构体
    // array模型,初始化阵列参数
    auto arr = std::make_shared<ModelArrayRect>();
    arr->set_dim(cnt_x, 1);					   //维度
    arr->set_element_size(len_x, len_y, 0.2);  //元素尺寸
    arr->set_group_symmetry(false);            //对称性
    arr->set_pitch(pitch_x, 0);                //间距
    //获取阵列的顶点坐标
    auto vertex_t = std::make_shared<std::vector<std::vector<Point>>>();
    arr->get_vertex(w->c_sys_1, *vertex_t);//维度是1,直接把坐标系传过来了
    
    //将顶点坐标和保存到wedge,pa:array的Point
    wedge->pa = vertex_t;
    //设置阵列的参数
    array->cnt_x = cnt_x;
    array->cnt_y = 1;
    array->pitch_x = pitch_x;
    array->pitch_y = 0;

    //探测器:由阵列和楔块组成,赋上属性返回即可
    auto p = std::make_shared<ProbeWARect>();
    p->array = array;
    p->wedge = wedge;
    return p;
}
```

#### 3.3`create_weld`

#### 根据给定的楔块属性和扫描类型，生成相应的焊接信息

```cpp
std::shared_ptr<Weld> create_weld(
    std::shared_ptr<TubeProp> tube,
    double g,
    double s,
    double angle,
    double h1,
    double h2)
{
    std::vector<vtkSmartPointer<vtkProp>> v;
    if (tube->scan_type == TubeProp::Scan::OnCircle) {
        std::vector<Point> v_p;
        on_scan_circle(tube, g, s, angle, h1, h2, v_p);
        for (auto &p : v_p) {
            p = Rotate(-M_PI / 2, Rotate::ROTATE_Y).trans({p.x, p.y, p.z}).end_point(Point{0, 0, 0});
        }
        on_weld_on_circle(v_p, v);
    } else if (tube->scan_type == TubeProp::Scan::OnY) {
        std::vector<Point> v_p;
        on_scan_y(tube, g, s, angle, h1, h2, v_p);
        for (auto &p : v_p) {
            p = Point{p.x, tube->len / -2, p.z};
        }
        on_weld_on_y(v_p, tube->len, v);
    } else {
        // do nothing,
    }
    auto w = std::make_shared<Weld>();
    w->vtk_actor = v;
    return w;
}

}
```

这段代码是一个创建焊接对象的函数，根据给定的楔块属性和扫描类型，生成相应的焊接信息，并保存到 Weld 对象中。根据不同的扫描类型，调用不同的函数生成顶点坐标，并进行相应的处理。最终将生成的焊接信息保存到 Weld 对象中并返回。

#### 3.4`create_beam();`

#### 3.5`create_focal();`

#### ----------------------------------------------------------

#### `3.6.Geo类中RotateMatrix`

```
/*
第一个构造函数 RotateMatrix(double A, RotateAxis a) 用于根据旋转角度和旋转轴创建旋转矩阵。
第二个构造函数 RotateMatrix(double ca, double sa, RotateAxis a) 用于根据旋转角度的余弦值和正弦值以及旋转轴创建旋转矩阵。
第三个构造函数 RotateMatrix(Vector ex, Vector ey, Vector ez) 用于根据三个向量表示的坐标系创建旋转矩阵。
成员函数：

trans(Vector r) 函数用于将给定向量 r 进行旋转变换，返回旋转后的向量。
trans_inv(Vector r) 函数用于将给定向量 r 进行逆向旋转变换，返回逆向旋转后的向量。
*/

    RotateMatrix(double A, RotateAxis a)
    {
        if (ROTATE_Z == a) {
            m[0][0] = cos(A);
            m[0][1] = -sin(A);
            m[0][2] = 0;

            m[1][0] = -m[0][1];
            m[1][1] = m[0][0];
            m[1][2] = 0;

            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
``````

[旋转矩阵](https://blog.csdn.net/weixin_45590473/article/details/122884112#%E6%97%8B%E8%BD%AC%E7%9F%A9%E9%98%B5)

对于一个三维空间的点 P(x,y,z)P(x,y,z)，要将其绕 zz 轴旋转 θθ 角度是可以很简单地用旋转矩阵来表示的,这就是上面二维数组m存储的值一致

![image-20240321085155544](D:\0work\学习资料\readme\readme.assets\image-20240321085155544.png)

坐标轴是右手系,所以画二维坐标系时候要符合规律,画正方向x->y->z->x

为了符合平移规律,在右边矩阵处多加1,变成齐次方程

![IMG_20240321_094007](D:\0work\学习资料\readme\readme.assets\IMG_20240321_094007.jpg)



#### 相对 基准坐标系 做变换是左乘，相对 当前坐标系 做变换是右乘

```
    //左乘(旋转矩阵的位置)
    Vector trans(Vector r)
    {
        return {m[0][0] * r.vx + m[0][1] * r.vy + m[0][2] * r.vz,
                m[1][0] * r.vx + m[1][1] * r.vy + m[1][2] * r.vz,
                m[2][0] * r.vx + m[2][1] * r.vy + m[2][2] * r.vz};
    }

    //右乘
    Vector trans_inv(Vector r)
    {
        return {r.vx * m[0][0] + r.vy * m[1][0] + r.vz * m[2][0],
                r.vx * m[0][1] + r.vy * m[1][1] + r.vz * m[2][1],
                r.vx * m[0][2] + r.vy * m[1][2] + r.vz * m[2][2]};
    }
/*
第一行代表了旋转后的 x 轴在原坐标系中的分量，
第二行代表了旋转后的 y 轴在原坐标系中的分量，
第三行代表了旋转后的 z 轴在原坐标系中的分量。
*/
    double m[3][3];
};
```



#### point_on_inc

```
bool TubeProp::point_on_inc(
    const Point &o,
    double ar,
    double c1,
    double c2,
    Point &p,
    Vector &d)
{
    if (scan_type == Scan::OnCircle) {
        auto z = Vector(Point{0, o.y, 0}, o).norm_l2();
        auto y = find_on_cly_y(z, ar, c1, c2);
        p = Point{o.x * r_outer / z, o.y + y, o.z * r_outer / z};
        auto d1 = Rotate(ar / 180 * M_PI, Rotate::ROTATE_X).trans({0, 0, -1});
        auto d2 = Rotate(-o.z / z, o.x / z, Rotate::ROTATE_Y).trans(d1);
        d = Vector{d2.vx, d2.vy, -d2.vz};
    } else if (scan_type == Scan::OnY) {
        //z表示向量长度  ar折射角角度  c1  c2分别代表入射介质和折射介质的声速
        //a表示的是旋转的角度
        auto z = Vector(Point{0, o.y, 0}, o).norm_l2();
        auto a = find_on_arc(z, ar, c1, c2);
        if (!valid_a(a)) {
            return false;
        }
        //                   cos      sin          绕y轴旋转
        auto rot = Rotate(-o.z / z, o.x / z, Rotate::ROTATE_Y);//使m[3][3]为绕y轴旋转对应的旋转矩阵
        auto r = r_outer;
        auto ea = Vector{std::sin(a), 0, std::cos(a)};
        auto pa = rot.trans(ea * r);
        p = Point{pa.vx, o.y, -pa.vz};
        //向量,折射角的向量
        auto da = rot.trans(Rotate(-ar / 180 * M_PI, Rotate::ROTATE_Y).trans(ea * -1));
        d = Vector{da.vx, da.vy, -da.vz};
    } else {
        // no nothing,
        return false;
    }
    return true;
}

```

逆时针是正方向

1. `auto rot = Rotate(-o.z / z, o.x / z, Rotate::ROTATE_Y);`：这行代码创建了一个旋转矩阵 `rot`，该矩阵表示绕 y 轴顺时针旋转的变换。旋转角度的计算涉及到了 `o` 点的坐标，其中 `-o.z / z` 表示绕 y 轴旋转的角度的正弦值，`o.x / z` 表示绕 y 轴旋转的角度的余弦值。
2. `auto r = r_outer;`：这行代码将管道的外径赋值给变量 `r`。
3. `auto ea = Vector{std::sin(a), 0, std::cos(a)};`：这行代码根据参数 `a`，也就是找到的焦点在管道上的角度，创建了一个单位向量 `ea`，表示从管道圆心指向焦点的方向。
4. `auto pa = rot.trans(ea * r);`：这行代码将单位向量 `ea` 乘以管道的外径 `r`，然后经过旋转矩阵 `rot` 的变换，得到了点 `pa`，表示管道上的焦点的位置。
5. `p = Point{pa.vx, o.y, -pa.vz};`：这行代码将得到的点 `pa` 的坐标分量 `pa.vx` 和 `-pa.vz`，与 `o` 点**的 y 坐标保持一致(OnY模式)**，构成了管道上的焦点的位置 `p`。
6. `auto da = rot.trans(Rotate(-ar / 180 * M_PI, Rotate::ROTATE_Y).trans(ea * -1));`：这行代码首先将单位向量 `ea` 乘以 -1，然后经过绕 y 轴逆时针旋转的变换（角度为 `ar`），再经过旋转矩阵 `rot` 的变换，得到了一个向量 `da`，表示入射光线的方向。
7. `d = Vector{da.vx, da.vy, -da.vz};`：这行代码将得到的向量 `da` 的坐标分量 `da.vx`、`da.vy` 和 `-da.vz` 组成了入射光线的方向 `d`。

总的来说，这段代码的目的是根据给定的焦点在管道上的位置和入射角度，计算出管道上的焦点位置和入射光线的方向。



