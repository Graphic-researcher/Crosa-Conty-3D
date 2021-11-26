- [ ] SceneViewPort

  - [x] 新增一个Scene视图窗口，
  - [x] 1. FrameBuffer是怎么读取的，在FrameBuffer里
    2. framebuffer的工作方式是读取当前帧缓存区的图片，那么如何读取两个不同角度的图片
    3. 分两次绘制

  - [ ] 把Gizmos改为点击就开启
  - [ ] 把读取两个frameBuffer都放在scene里

- [ ] RendererQueue

  渲染队列，参考UNITY可编程渲染管线的DarwRenderer

  ![image-20211125135037369](C:\Users\darkmon\AppData\Roaming\Typora\typora-user-images\image-20211125135037369.png)

  1. 删除引擎中没有用的文件
     - [x] 删除了正交相机OrthogonalCamera
  2. 先封装类RenderContext存储所有renderer
     1. 把Renderer该名为Context
     2. 架构上，RenderContext比Rendercommend更高级
  3. 基础类Renderer，所有Renderer都继承此类

  

- [ ] MeshRender

- [ ] 1. 我需要的是顶点的数据，一个mesh，然后
  2. 可能并不需要模型类
  3. 在mesh中存储点的数据，然后放到Renerer里渲染
  4. 材质是材质，mesh是mesh，两个类
  4. 子实体系统
  
- [ ] 1. 把所有渲染器的渲染对象都放在BatchRender上 

- [ ] ModelImporter

- [ ] Material

- [ ] Phong Shader

- [ ] PositionBasedDynamics

- [ ] Shadow Map

- [ ] PBR

- [ ] Fluid

- [ ] Bloom

  1. 先去找imGuizom怎么判断物体ID的那个部份

