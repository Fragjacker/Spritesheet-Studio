![spritesheet_studio_image](https://user-images.githubusercontent.com/19975052/152341382-09965907-6432-4489-9711-81d4412252be.png)
# Spritesheet-Studio
C++ app that helps to quickly create spritesheets out of multiple images via a GUI. It uses OpenCV for the image manipulation and FLTK for the GUI. Precompiled binaries are available for windows only currently. However the multi-platform nature of this project and its dependacies should make it possible to build this app on Linux and MacOS natively too.
# Supported Image Formats
- **PNG (READ/WRITE)**
- **DDS (READ ONLY)**

The final spritesheet will be stored in the folder of the executable. The file format will be **PNG** with maximum compression by default. The filename can be changed via the input field.
# Platform Build Requirements
The app code is opted for multiplattform support. However the build requirements may vary and require user intervention to function.
## Windows
- Visual Studio 2022
- Cmake version 3.1 or higher
- Latest Git for Windows

Simply open Visual Studio and select clone repository and provide the URL to this repository. Then hit Build->Build all. Cmake should take care of any dependancies for you and build everything automatically.
## MacOS
- Homebrew
- Cmake
- Git

Clone this repository, then open a command prompt and `cd` into the repository. Once there invoke Cmake there to start building the project. (Untested)
## Linux
- Cmake
- Git

Clone this repository, then open a command prompt and `cd` into the repository. Once there invoke Cmake there to start building the project. (Untested)

# Tags
spritesheet,studio,tool,png,dds,c++,app,cmake,windows,linux,macos
