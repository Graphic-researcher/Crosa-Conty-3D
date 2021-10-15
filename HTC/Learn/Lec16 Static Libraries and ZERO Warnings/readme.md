# Lec16 Static Libraries and ZERO Warnings

tutorial : [here](https://www.youtube.com/watch?v=TlvmnoDlrI0&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=26)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/41a9dfd4c38d1bc2f7c924bfac62726023c19cf1/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

prerequisite : [Intro to render](https://www.youtube.com/watch?v=Hoi-Gzk-How&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=24)	[render architecture](https://www.youtube.com/watch?v=YPWNNmlIUIo&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=25) [Rendering and Maintenance](https://www.youtube.com/watch?v=695SyEyFwWU&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=26)

Now our engine will build with static library,it's easier to maintain because we just build it for learning and fun (●'◡'●)

Clean your project first and we will switch our build mode to static library.

## Config

You need to copy the configure code in dll to lib

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

## Build and Result

Add the function in sandbox scope :

```c++
virtual void OnImGuiRender() override
{
    ImGui::Begin("Test");
    ImGui::Text("Hello CC3D!");
    ImGui::End();
}
```

![image-20211015135433168](https://i.loli.net/2021/10/15/yDP6MtiplQjKrg9.png)