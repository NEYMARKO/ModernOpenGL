# ModernOpenGL
## Setup
### Requirements: 
  -  Visual Studio 2022 with C++17 or higher
  -  CMake
  -  OpenGL version 3.3 or higher
### Setup Proccess
1. clone repository using command ```git clone <repo_link>```
2. clone bullet3 repository using command ```git clone https://github.com/bulletphysics/bullet3.git```
3. make empty folder called buildCMake (name can be arbitary) 
4. open CMake:
  -  for a source code location provide path to the bullet3 directory (made after cloning repo)
  -  for binary build location choose your buildCMake folder
  -  ![CMake setup](https://github.com/[NEYMARKO]/[ModernOpenGL]/[main]/READMEIMAGES/CMake_options.png?raw=true)

5. press CONFIGURE button, then press it againg after new values have been displayed in red, after that press GENERATE
6. locate BULLET_PHYSICS.sln solution in your buildCMake folder and run it
7. build solution (make sure to build it in debug mode for x64 architecture)
8. for projects in 12th point change runtime library (Properties->C/C++->Code Generation->Runtime Library) to /MDd (since that is what Start.sln is using) and rebuild them to avoid missmatch error (all of the rebuilding has to be done manually for now. If I manage to configure CMakeLists.txt to build it in /MDd by default, this step will be skipped - for now it has to be done)
9. open original project (ModernOpenGL/Projects/Start.sln), navigate to project settings: Project->Properties (or right click on solution->properties)
10. Configuration Properties->VC++ Directories->General->Include Directories - EDIT and add new line - add <full_path>/bullet3/src (src folder located in directory got from cloning bullet3 repo)
11. Configuration Properties->VC++ Directories->General->Library Directories - EDIT and add new line - add <full_path>/bullet3/buildCMAKE/lib/Debug
12. Linker->Input->Additional Dependencies - EDIT and add lines:
  -  Bullet3Collision_Debug.lib
  -  Bullet3Common_Debug.lib
  -  Bullet3Dynamics_Debug.lib
  -  Bullet3Geometry_Debug.lib
  -  BulletCollision_Debug.lib
  -  BulletDynamics_Debug.lib
  -  LinearMath_Debug.lib


All of the other libs and include directories are already uploaded to git
