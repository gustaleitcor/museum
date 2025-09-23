#pragma once
// #include <GL/gl.h>
// #include <GL/glu.h>
// #include <format>
// #include <iostream>
// #include <string>
// #include <print>



// TODO: FIND A BETTER PLACE TO LOAD TEXTURE
// Loads an image and creates an OpenGL 2D texture.


// inline void loadTexture() {

// }

// static GLuint texName = 0;
// inline void loadTexture(const char *filename) {
//   int width, height, nrChannels;
//   unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
//   if (data) {
//     if (texName == 0) {
//       glGenTextures(1, &texName);
//     }
//     glBindTexture(GL_TEXTURE_2D, texName);

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
//                     GL_LINEAR_MIPMAP_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//     if (nrChannels == 3) {
//       gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
//                         GL_UNSIGNED_BYTE, data);
//     } else if (nrChannels == 4) {
//       gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA,
//                         GL_UNSIGNED_BYTE, data);
//     } else {
//       // std::println("Unsupported texture format: {} channels", nrChannels);
//     }

//     stbi_image_free(data);
//   } else {
//     // std::println("Failed to load texture: {}", filename);
//   }
// }
