## VS中配置VTK

手动配置或者cmake配置



**调试环境    Debug x64**

1首先是配置调试环境，除了之前配置的occt的bin目录以外，还需要增加VTK的bin目录，以及依赖的第三方库freetype的bin目录。

```
PATH=D:\Program Files\opencascade-install\win64\vc14\bind;D:\Program Files\VTK-9.3.0-install\bin;D:\OpenCASCADE-7.7.0-vc14-64\products\freetype-2.5.5-vc14-64\bin;%PATH%
```

![image-20240315112353430](D:\0work\学习资料\readme\readme.assets\image-20240315112353430.png)

2接着是在C/C++——常规中增加VTK头文件配置。

```
D:\Program Files\VTK-9.3.0-install\include\vtk-9.3;D:\Program Files\opencascade-install\inc;%(AdditionalIncludeDirectories)
```

![image-20240315112511901](D:\0work\学习资料\readme\readme.assets\image-20240315112511901.png)

3接着是链接器——常规增加VTK的dll所在目录

```
D:\Program Files\opencascade-install\win64\vc14\libd;D:\Program Files\VTK-9.3.0-install\lib;%(AdditionalLibraryDirectories)
```

![image-20240315112626972](D:\0work\学习资料\readme\readme.assets\image-20240315112626972.png)

4最后输入里面的附加依赖

```
TKBin.lib
TKBinL.lib
TKBinTObj.lib
TKBinXCAF.lib
TKBO.lib
TKBool.lib
TKBRep.lib
TKCAF.lib
TKCDF.lib
TKDCAF.lib
TKDraw.lib
TKernel.lib
TKExpress.lib
TKFeat.lib
TKFillet.lib
TKG2d.lib
TKG3d.lib
TKGeomAlgo.lib
TKGeomBase.lib
TKHLR.lib
TKIGES.lib
TKLCAF.lib
TKMath.lib
TKMesh.lib
TKMeshVS.lib
TKOffset.lib
TKOpenGl.lib
TKOpenGlTest.lib
TKPrim.lib
TKQADraw.lib
TKRWMesh.lib
TKService.lib
TKShHealing.lib
TKStd.lib
TKStdL.lib
TKSTEP.lib
TKSTEP209.lib
TKSTEPAttr.lib
TKSTEPBase.lib
TKSTL.lib
TKTObj.lib
TKTObjDRAW.lib
TKTopAlgo.lib
TKTopTest.lib
TKV3d.lib
TKVCAF.lib
TKViewerTest.lib
TKVRML.lib
TKXCAF.lib
TKXDE.lib
TKXDECascade.lib
TKXDEDRAW.lib
TKXDEIGES.lib
TKXDESTEP.lib
TKXMesh.lib
TKXml.lib
TKXmlL.lib
TKXmlTObj.lib
TKXmlXCAF.lib
TKXSBase.lib
TKXSDRAW.lib


vtkInteractionStyle-9.3d.lib
vtkRenderingOpenGL2-9.3d.lib
vtkCommonCore-9.3d.lib
vtkRenderingCore-9.3d.lib
vtkCommonExecutionModel-9.3d.lib
vtksys-9.3d.lib
TKMath.lib
TKV3d.lib
TKernel.lib
TKTopAlgo.lib
TKPrim.lib
```

![image-20240315112654829](D:\0work\学习资料\readme\readme.assets\image-20240315112654829.png)

