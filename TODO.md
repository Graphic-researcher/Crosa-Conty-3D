- [ ] RendererQueue

  渲染队列，参考UNITY可编程渲染管线的DarwRenderer

  ![image-20211125135037369](C:\Users\darkmon\AppData\Roaming\Typora\typora-user-images\image-20211125135037369.png)

  1. 删除引擎中没有用的文件
     1. 删除了正交相机OrthogonalCamera
  2. 先封装类RenderContext存储所有renderer
     1. 架构上，RenderContext比Rendercommend更高级
  3. 基础类Renderer，所有Renderer都继承此类

  

- [ ] MeshRender

- [ ] ModelImporter

- [ ] Material

- [ ] Phong Shader

- [ ] PositionBasedDynamics

- [ ] Shadow Map

- [ ] PBR

- [ ] Fluid

- [ ] Bloom

  1. 先去找imGuizom怎么判断物体ID的那个部份

