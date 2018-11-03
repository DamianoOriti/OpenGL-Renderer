# OpenGL Renderer

**Renderer based on OpenGL developed by Damiano Oriti**

## Description

This Renderer has been used for [Escape Room videogame project](https://github.com/DamianoOriti/Escape-Room) developed by Damiano Oriti and Sara Battelini.
This renderer allows the user to create some scenes and to render them on screen. A scene is composed of a collection of billboards, a collection of texts, a collection of shaded 3D models, a collection of shadeless (meaning they have just a texture and do not react to light) 3D models, and finally the lights, one point light and one directional light.

[Preview of the Renderer](https://github.com/DamianoOriti/OpenGL-Renderer/blob/master/preview.png)

[Video of Escape Room videogame project](https://www.youtube.com/watch?v=IKH6xkf1tJQ)

### Features:

- Phong shading
- Support for normal, diffuse and specular maps
- Environment map reflections
- Real-time shadows with shadow map technique (2D map for directional light and cube map for point light)
- Cascaded shadow maps technique (for directional light)
- Filtered shadows for soft edges
- Font rendering based on [signed distance field](http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf)
- Text effects such as drop shadows and glow
- Billboard rendering with transparency
- Render to texture technique

**Important note**

If you want to and will use this software, it would be greatly appreciated if you could spend three minutes for watching and giving a "Like" to the video linked above, thanks!

## Program

When you launch the program, two windows will appear: one shows the console output while the other shows the rendering of the scene. The rendered scene is composed of one shaded 3D model (a sofa), one billboard showing a dog and four texts showing the different visual effects which can been applied to text.

### Commands:

- 'W' key: move camera forward
- 'A' key: move camera backward
- 'S' and 'D' keys: move camera sideways
- 'R' key: move camera up
- 'F' key: move camera down
- Mouse: change looking direction

## Libraries

- [Assimp](https://github.com/assimp/assimp) for loading 3D model file formats such as fbx
- [libpng](https://github.com/glennrp/libpng) for loading png images

To install the above libraries, you could use [Vcpkg](https://github.com/Microsoft/vcpkg).

For window creation and input handling [GLUT](https://www.opengl.org/resources/libraries/glut/) has been used.

## Build

Alfter building the project, you need to copy glut32.lib in the same folder of the executable, be it Debug or Release.
