# Lec31 Blending

课程 : [here](https://www.youtube.com/watch?v=N94fHNZEHas&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=44)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/962c1fa9f6dc1ffdabdd930375e4f54f2083ef23/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

先修课 : [here](https://www.youtube.com/watch?v=o1_yJ60UIxs&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=18)

## Render 作用域

### RenderAPI

```c++
virtual void Init() = 0;
```

### RenderCommand 

```c++
inline static void Init()
{
	s_RendererAPI->Init();
}
```

### Render 

```c++
static void Init();
void Renderer::Init()
{
    RenderCommand::Init();
}
```

### OpenGLRenderAPI 

```c++
virtual void Init() override;
void OpenGLRendererAPI::Init()
{//enable blend mode
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
```

## Application

```c++
Application::Application()
{
    //...instance and set window
    Renderer::Init();//init our render,enable blend in it
    //...imguilayers...
}
```

## Build and Result

### 颜色通道错误

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/3a754a490deb9a0277fd846d91c8511aa77e137a/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

![image-20211021134601952](https://i.loli.net/2021/10/21/9HdZlahJF8M3rSs.png)

### 混合错误

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/39896f79294d52b1069b389660d9a24451fab0c9/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

![image-20211021135118120](https://i.loli.net/2021/10/21/FvGPBk8WJlm3YZX.png)

### 支持混合模式

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/962c1fa9f6dc1ffdabdd930375e4f54f2083ef23/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

![image-20211021140049078](https://i.loli.net/2021/10/21/6bivNdI9zWlAuqE.png)



