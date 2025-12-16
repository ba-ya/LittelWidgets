[TOC]

###  

### 1.occQt::makeBox()

```cpp
// 创建一个立方体并在OpenCASCADE视图中显示
void occQt::makeBox()
{
    // 创建一个立方体的拓扑形状
    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();

    // 创建一个用于显示的AIS_Shape对象，将立方体拓扑形状作为参数传递
    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);

    // 设置显示颜色为蓝色
    anAisBox->SetColor(Quantity_NOC_AZURE);

    // 将AIS_Shape对象添加到OpenCASCADE视图的上下文中进行显示，第二个参数为True表示立即显示
    myOccView->getContext()->Display(anAisBox, Standard_True);
}

```

### 2.occQt::makeCone()

```cpp
// 创建一个圆锥和一个锥体，并在OpenCASCADE视图中显示它们
void occQt::makeCone()
{
    // 创建一个坐标系，指定圆锥的轴线
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 10.0, 0.0)); // 设置坐标系的位置为 (0.0, 10.0, 0.0)

    // 使用指定轴线和参数创建一个圆锥的拓扑形状
    TopoDS_Shape aTopoReducer = BRepPrimAPI_MakeCone(anAxis, 3.0, 1.5, 5.0).Shape();

    // 创建一个用于显示的AIS_Shape对象，将圆锥拓扑形状作为参数传递
    Handle(AIS_Shape) anAisReducer = new AIS_Shape(aTopoReducer);

    // 设置圆锥的显示颜色为浅橙色
    anAisReducer->SetColor(Quantity_NOC_BISQUE);

    // 创建另一个坐标系，指定锥体的轴线
    anAxis.SetLocation(gp_Pnt(8.0, 10.0, 0.0)); // 设置坐标系的位置为 (8.0, 10.0, 0.0)

    // 使用指定轴线和参数创建一个锥体的拓扑形状
    TopoDS_Shape aTopoCone = BRepPrimAPI_MakeCone(anAxis, 3.0, 0.0, 5.0).Shape();

    // 创建一个用于显示的AIS_Shape对象，将锥体拓扑形状作为参数传递
    Handle(AIS_Shape) anAisCone = new AIS_Shape(aTopoCone);

    // 设置锥体的显示颜色为巧克力色
    anAisCone->SetColor(Quantity_NOC_CHOCOLATE);

    // 在OpenCASCADE视图中显示圆锥和锥体，第二个参数为True表示立即显示
    myOccView->getContext()->Display(anAisReducer, Standard_True);
    myOccView->getContext()->Display(anAisCone, Standard_True);
}

```

### 3.occQt::makeSphere

```cpp
// 创建一个球体并在OpenCASCADE视图中显示
void occQt::makeSphere()
{
    // 创建一个坐标系，指定球体的中心位置
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 20.0, 0.0)); // 设置坐标系的位置为 (0.0, 20.0, 0.0)

    // 使用指定的轴线和半径创建一个球体的拓扑形状
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 3.0).Shape();

    // 创建一个用于显示的AIS_Shape对象，将球体的拓扑形状作为参数传递
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);

    // 设置球体的显示颜色为蓝色
    anAisSphere->SetColor(Quantity_NOC_BLUE1);

    // 在OpenCASCADE视图中显示球体，第二个参数为True表示立即显示
    myOccView->getContext()->Display(anAisSphere, Standard_True);
}

```

