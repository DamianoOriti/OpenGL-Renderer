# OpenGL Renderer

**Renderer based on OpenGL developed by Damiano Oriti**

## Description

This Renderer has been used for [Escape Room videogame project](https://github.com/DamianoOriti/Escape-Room) developed by Damiano Oriti and Sara Battelini.

[Preview](https://github.com/DamianoOriti/OpenGL-Renderer/blob/master/preview.png)

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

## Libraries

- [Assimp](https://github.com/assimp/assimp) for loading 3D model file formats such as fbx
- [libpng](https://github.com/glennrp/libpng) for loading png images

To install the above libraries, you could use [Vcpkg](https://github.com/Microsoft/vcpkg).

For window creation and input handling [GLUT](https://www.opengl.org/resources/libraries/glut/) has been used.

## Build

Alfter building the project, you need to copy glut32.lib in the same folder of the executable, be it Debug or Release.
