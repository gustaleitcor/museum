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
  // Enum para mapear GLUT special key codes.
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
  GLuint id_orig, id, w, h;
};

struct Vertice
{
  Utils::Vector2<int> room_left;
  Utils::Vector2<int> room_mid;
  Utils::Vector2<int> room_right;
};

namespace Game
{
  // Gerencia o estado global do game (entrada, resolução e configurações) seguindo o padrão Singleton.
  class State
  {
  public:
    static std::shared_ptr<State> getInstance()
    {
      static std::shared_ptr<State> instance{new State()};
      return instance;
    }

    // Getters e Setters
    inline const Utils::Vector2<int> &mouseDelta() const { return m_mouseDelta; }
    inline Utils::Vector2<int> &mut_mouseDelta() { return m_mouseDelta; }

    inline const Utils::Vector2<int> &windowSize() const { return m_windowSize; }
    inline Utils::Vector2<int> &mut_windowSize() { return m_windowSize; }

    inline const GLuint texture(int i) const { return m_texIds[i]; }
    inline const Image photo(int i) const { return i < HALL_LENGTH*4 ? m_photosInfo[i] : (Image){.id = m_texIds[3], .w = 693, .h = 858}; }
    
    inline const Utils::Vector2<int>& point_left() const { return m_point_left; }
    inline const Utils::Vector2<int>& point_mid() const { return m_point_mid; }
    inline const Utils::Vector2<int>& point_right() const { return m_point_right; }

    inline Utils::Vector2<int> &mut_point_left() { return m_point_left; }
    inline Utils::Vector2<int> &mut_point_mid() { return m_point_mid; }
    inline Utils::Vector2<int> &mut_point_right() { return m_point_right; }

    // Atualiza o estado do game
    void update();
    // Reseta o estado do game
    void reset();
    // Carrega texturas do ambiente
    void load_texture();
    // Carrega texturas dinamicamente dos quadros
    void load_photos(bool side);

    // Callbacks de entrada
    static void mouseMotionCallback(GLint x, GLint y);
    static void keyboardCallback(unsigned char key, int x, int y);
    static void keyboardUpCallback(unsigned char key, int x, int y);
    static void specialCallback(int key, int x, int y);
    static void specialUpCallback(int key, int x, int y);

    // Funções de consulta de estado do game
    inline const bool isKeyPressed(char key) const { return m_asciiKeys[static_cast<unsigned char>(key)]; }
    inline const bool isSpecialPressed(Key::Special key) const { return m_specialKeys[key]; }
    inline const uint random_number_left(int max) { return (gen_left() % (max + 1)); }
    inline const int random_number_right(int max) { return (gen_right() % (max + 1));}
    inline const Vertice get_neigh(int x, int z) { return neigh[x][z]; }
    inline const bool checkpoint() { return m_checkpoint; }

    // Funções de mudaça de estado do game
    inline const void toggle() { m_checkpoint = !m_checkpoint; }

    // Logica de atualização dos quadros via uma seed
    inline const void add_seed_left()
    {
      const int qtd_images = 175;
      seed_left += 2;
      if (seed_left > (qtd_images/(2*HALL_LENGTH)) - 2){
        seed_left = 0;
      }
    }
    inline const void add_seed_right()
    {
      const int qtd_images = 175;
      seed_right += 2;
      if (seed_right > (qtd_images/(2*HALL_LENGTH)) - 1){
        seed_right = 1;
      }
    }
    inline const void sub_seed_left()
    {
      if (seed_left <= 0){
        seed_left = 20;
      }
      seed_left -= 2;
    }
    inline const void sub_seed_right()
    {
      if (seed_right <= 1) {
        seed_right = 21;
      }
      seed_right -= 2;
    }

  private:
    // Construtores padrão singleton
    State();
    State(const State &) = delete;
    State &operator=(const State &) = delete;

    // Funções para fazer download e salvamento das imagens
    static size_t memory_writeCb(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t curl_imageCb(void *contents, size_t size, size_t nmemb, void *userp);
    static bool http_get(const std::string &url, std::string &out);
    static bool download_image(const std::string &url, std::vector<uint8_t> &buffer);
    static bool get_images(std::vector<std::vector<uint8_t>> &images_data, int random_page);

    // Variaveis para controle da aleatoriedade do jogo (lado do quadro)
    std::mt19937 gen_left, gen_right;
    uint seed_left = 0, seed_right = 1;
    bool m_checkpoint = false;

    // Estrutura de dados para guardar as salas e seus respectivos vizinhos
    std::vector<std::vector<Vertice>> neigh;
    Utils::Vector2<int> m_point_left;
    Utils::Vector2<int> m_point_mid;
    Utils::Vector2<int> m_point_right;

    // Variaveis de controle de entrada do usuario
    Utils::Vector2<int> m_mouseDelta;
    bool m_asciiKeys[256];
    bool m_specialKeys[256];

    Utils::Vector2<int> m_windowSize;
    bool m_isFullScreen;

    // Vetor dos ID's da textura dos ambientes e dos quadros, respectivamente
    GLuint m_texIds[MAX_TEXTURES] = {0};
    Image m_photosInfo[HALL_LENGTH*4] = {0};
  };
} // namespace Game
