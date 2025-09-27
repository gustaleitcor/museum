#pragma once

#include "Vector2.hpp"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <memory>
#include <random>
#include <string>
#include <curl/curl.h>
#include "include/utils/json.hpp"
#include <vector>
#include <limits>
#include <iostream>
#include "src/config.hpp"

// Namespace for key-related definitions.
namespace Key
{
  // Enum to map GLUT special key codes.
  enum Special
  {
    F1 = GLUT_KEY_F1,
    F2 = GLUT_KEY_F2,
    F3 = GLUT_KEY_F3,
    F4 = GLUT_KEY_F4,
    F5 = GLUT_KEY_F5,
    F6 = GLUT_KEY_F6,
    F7 = GLUT_KEY_F7,
    F8 = GLUT_KEY_F8,
    F9 = GLUT_KEY_F9,
    F10 = GLUT_KEY_F10,
    F11 = GLUT_KEY_F11,
    F12 = GLUT_KEY_F12,
    LEFT = GLUT_KEY_LEFT,
    UP = GLUT_KEY_UP,
    RIGHT = GLUT_KEY_RIGHT,
    DOWN = GLUT_KEY_DOWN,
    PAGE_UP = GLUT_KEY_PAGE_UP,
    PAGE_DOWN = GLUT_KEY_PAGE_DOWN,
    HOME = GLUT_KEY_HOME,
    END = GLUT_KEY_END,
    INSERT = GLUT_KEY_INSERT
  };
} // namespace Key

struct Image
{
  GLuint id, w, h;
};

struct Vertice
{
  Utils::Vector2<int> room_left;
  Utils::Vector2<int> room_mid;
  Utils::Vector2<int> room_right;
};

// Namespace for game-related components.
namespace Game
{
  // Manages global game state (input, window properties) as a Singleton.
  class State
  {
  public:

    Utils::Vector2<int> point_left;
    Utils::Vector2<int> point_mid;
    Utils::Vector2<int> point_right;

    // Returns the singleton instance.
    static std::shared_ptr<State> getInstance()
    {
      static std::shared_ptr<State> instance{new State()};
      return instance;
    }

    inline const Utils::Vector2<int> &mouseDelta() const { return m_mouseDelta; }
    inline Utils::Vector2<int> &mut_mouseDelta() { return m_mouseDelta; }

    inline const Utils::Vector2<int> &windowSize() const { return m_windowSize; }
    inline Utils::Vector2<int> &mut_windowSize() { return m_windowSize; }

    inline const GLuint texture(int i) const { return m_texIds[i]; }
    inline const Image photo(int i) const { return i < HALL_LENGTH*4 ? m_photosInfo[i] : (Image){.id = m_texIds[3], .w = 693, .h = 858}; }
    // Updates game state.
    void update();
    // Resets game state.
    void reset();
    // Load texture
    void load_texture();
    void load_photos(bool side);

    // Mouse button callback.
    static void mouseCallback(int button, int state, int x, int y);
    // Mouse motion callback.
    static void mouseMotionCallback(GLint x, GLint y);
    // ASCII key press callback.
    static void keyboardCallback(unsigned char key, int x, int y);
    // ASCII key release callback.
    static void keyboardUpCallback(unsigned char key, int x, int y);
    // Special key press callback.
    static void specialCallback(int key, int x, int y);
    // Special key release callback.
    static void specialUpCallback(int key, int x, int y);

    // Checks if ASCII key is pressed.
    inline bool isKeyPressed(char key) const
    {
      return m_asciiKeys[static_cast<unsigned char>(key)];
    }

    // Checks if special key is pressed.
    inline bool isSpecialPressed(Key::Special key) const
    {
      return m_specialKeys[key];
    }
    inline const uint random_number_left(int max)
    {
      return (gen_left() % (max + 1));
    }
    inline const int random_number_right(int max)
    {
      return (gen_right() % (max + 1));
    }
    inline const Vertice get_neigh(int x, int z)
    {
      return neigh[x][z];
    }

    inline const void add_seed_left()
    {
      seed_left += 2;
      if (seed_left > 18)
        seed_left = 0;
      std::cout << "seed esquerda soma " << seed_left << std::endl; 
    }

    inline const void add_seed_right()
    {
      seed_right += 2;
      if (seed_right > 19)
        seed_right = 1;
      std::cout << "seed direita soma " << seed_right << std::endl; 
    }

    inline const void sub_seed_left()
    {
      
      if (seed_left <= 0)
        seed_left = 20;
      seed_left -= 2;
      std::cout << "seed esquerda sub " << seed_left << std::endl; 
    }

    inline const void sub_seed_right()
    {
      if (seed_right <= 1)
        seed_right = 21;
      seed_right -= 2;
      std::cout << "seed direita sub " << seed_right << std::endl; 
    }


    inline const bool checkpoint()
    {
      return m_checkpoint;
    }

    inline const void toggle()
    {
      m_checkpoint = !m_checkpoint;
    }

  private:
    State();
    State(const State &) = delete;
    State &operator=(const State &) = delete;

    // escreve dados no vetor contents
    static size_t memory_writeCb(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t curl_imageCb(void *contents, size_t size, size_t nmemb, void *userp);
    // realiza get e pega a string de resposta
    static bool http_get(const std::string &url, std::string &out);
    // coloca a imagem em um vector
    static bool download_image(const std::string &url, std::vector<uint8_t> &buffer);
    bool get_images(std::vector<std::vector<uint8_t>> &images_data, int random_page);

    std::mt19937 gen_left, gen_right;
    uint seed_left = 0, seed_right = 1;
    bool m_checkpoint = false;

    Utils::Vector2<int> m_mouseDelta;
    // std::vector<Utils::Vector2<int>> points;
    std::vector<std::vector<Vertice>> neigh;


    bool m_asciiKeys[256];
    bool m_specialKeys[256];

    Utils::Vector2<int> m_windowSize;
    bool m_isFullScreen;

    GLuint m_texIds[MAX_TEXTURES] = {0};
    // TODO: DEFINE HALLWAY LENGTH
    Image m_photosInfo[HALL_LENGTH*4] = {0};
  };
} // namespace Game
