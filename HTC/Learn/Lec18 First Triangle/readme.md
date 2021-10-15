# Lec 18 First Triangle

tutorial : [here](https://www.youtube.com/watch?v=bwFYXo0VgCc&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=28)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/0d451000efbcbf615bda91389e718b43f4ee7685/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

Now we will enter plenty of opengl method,so I recommend you to check cherno's opengl series when you follow the engine series

OpenGL series : [here](https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2)

## Application Scope

```c++
//Application.h
unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
//Application.cpp

glClearColor(1, 0, 1, 1);
glClear(GL_COLOR_BUFFER_BIT);

glGenVertexArrays(1, &m_VertexArray);
glBindVertexArray(m_VertexArray);

glGenBuffers(1, &m_VertexBuffer);
glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

float vertices[3 * 3] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

glGenBuffers(1, &m_IndexBuffer);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

unsigned int indices[3] = { 0, 1, 2 };
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
glBindVertexArray(m_VertexArray);
glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

```

![image-20211015161423723](https://i.loli.net/2021/10/15/OxzN2kGQTEZ1PDl.png)

## Render context Scope

```c++
//OpenGLContext.cpp
void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    CC3D_CORE_ASSERT(status, "Failed to initialize Glad!");

    CC3D_CORE_INFO("OpenGL Info:");
    CC3D_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
    CC3D_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
    CC3D_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
}
```

![image-20211015161625038](https://i.loli.net/2021/10/15/a4FpAhwByfxXQDg.png)