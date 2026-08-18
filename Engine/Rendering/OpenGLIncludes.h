#pragma once

// This file contains the necessary includes for OpenGL and GLFW
// It is used so I wouldn't have to worry about their order in other files
// (it can get complicated if they are included through transitive includes - 
// order might get reversed and cause compilation errors)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
