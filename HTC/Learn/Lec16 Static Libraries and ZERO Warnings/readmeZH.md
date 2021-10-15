# Lec16 Static Libraries and ZERO Warnings

教程 : [here](https://www.youtube.com/watch?v=TlvmnoDlrI0&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=26)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/41a9dfd4c38d1bc2f7c924bfac62726023c19cf1/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

先修课 : [Intro to render](https://www.youtube.com/watch?v=Hoi-Gzk-How&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=24)	[render architecture](https://www.youtube.com/watch?v=YPWNNmlIUIo&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=25) [Rendering and Maintenance](https://www.youtube.com/watch?v=695SyEyFwWU&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=26)

现在我们的引擎将利用静态库构建，这将更好开发和维护，因为我们的引擎起到学习作用，一切从简

清理项目解决方案然后使用静态库链接

## 配置

你需要将部分dll的配置复制到lib配置

### DLL

Debug：

gladd.lib
imguid.lib
opengl32.lib
glfw3d.lib

Release:

imgui.lib
opengl32.lib
glad.lib
glfw3.lib

![image-20211015134210605](https://i.loli.net/2021/10/15/2dcOA3wLhjK91ni.png)

![image-20211015133948914](https://i.loli.net/2021/10/15/GvM1DtB7PNUfH6h.png)

### LIB

![image-20211015134314999](https://i.loli.net/2021/10/15/tMR29ufeEcTINyj.png)

![image-20211015134525937](https://i.loli.net/2021/10/15/aipbBAl93vwYh6t.png)

## 构建和结果

在sandbox范围内加入下列函数 :

```c++
virtual void OnImGuiRender() override
{
    ImGui::Begin("Test");
    ImGui::Text("Hello CC3D!");
    ImGui::End();
}
```

![image-20211015135433168](https://i.loli.net/2021/10/15/yDP6MtiplQjKrg9.png)