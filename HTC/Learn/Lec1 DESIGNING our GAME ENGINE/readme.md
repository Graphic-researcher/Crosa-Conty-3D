# 设计游戏引擎框架

## Entry Point

程序启动后，开始执行的地方（例如main函数，Engine控制函数等）

## Applicaton Layer

程序何时准备绘制画面

保持时间刷新

执行哪些方法和事件（如窗口调整大小，关闭窗口，鼠标事件）

## Window Layer

### Input

例如鼠标位置

### Events

窗口区域所触发的逻辑

## Render

## Render API

跨平台

多平台api之间的调节（例如vulkan，opengl，dx）