# Lec24 Creating am Orthographic Camera

tutorial : [here](https://www.youtube.com/watch?v=NjKv-HWstxA&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=35)

code version : 

prerequisite : [Camera and How they work](https://www.youtube.com/watch?v=LfbqtmqxX04&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=34)

## Orthographic Camera Scope





## Build and Result

```c++
OrthographicCamera test2(-1.0f, 1.0f, -1.0f, 1.0f);
```

![image-20211017123145643](https://i.loli.net/2021/10/17/h2gwv531FjYMxVA.png)

```c++
OrthographicCamera test2(-1.0f, 1.0f, -1.0f, 1.0f);
///...code...
m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
m_Camera.SetRotation(45.0f);
```

![image-20211017124042992](https://i.loli.net/2021/10/17/g42paKM9twRT3LC.png)