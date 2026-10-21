## BUG RECORD

- 拖动窗口，人物会跟着窗口移动，但是怪物的跟随动作理应不应该移动。现在移动完窗口后会自动动一下。
- 怪物移动时，偶尔会出现得到错误的坐标值，访问错误的地址。
  - 暂时在 ItemEntity 中判断是否 >0 进行保护，但是理应怪物在框内的时候，不应该出现负数.
- ~~FPS 从 200 降低到 100~~ 

  原因：循环的边界判断不要用函数，用确定变量，否则循环的时候会导致大量函数跳转

  ```c++
  auto viewportX = viewport.x();
  auto viewPortY = viewport.y();
  auto viewPortBoundaryX = viewportX + viewport.width();
  auto viewPortBoundaryY = viewPortY + viewport.height();
  for (int i = viewportX; i < viewPortBoundaryX; i++)
  {
  	for (int j = viewPortY; j < viewPortBoundaryY; j++) {}
  }
  ```

  