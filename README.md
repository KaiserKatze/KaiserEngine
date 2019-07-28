This solution loads OpenGL extensions manually,
instead of using extension loading library such as GLEW.

# Build

Edit `KaiserEngine/loadgl.txt` to specify OpenGL functions you need,
apart from those already defined in `gl/GL.h` or `gl/GLU.h`.
Specified function names should be seperated by newlines.

`loadgl.h` and `loadgl.cpp` are required to build *KaiserEngine* project.
Hence, before building solution, developer must execute the following
commands to generate necessary files.

```
cd KaiserEngine
python loadgl.py
```
