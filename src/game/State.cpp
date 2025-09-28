// Game::State class implementation.

#include "include/game/State.hpp"
#include <GL/freeglut_std.h>
#include <GL/glut.h>
#include <format>
#include <iostream>
#include <string>
#include <semaphore>
#include <barrier>
#include <thread>
#include <fstream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "include/utils/stb_image.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/utils/stb_image_write.h"

using json = nlohmann::json;
void Game::State::update()
{
  static bool F11_toggled = false;

  if (isSpecialPressed(Key::Special::F11))
  {
    if (!F11_toggled)
    {
      m_isFullScreen = !m_isFullScreen;
      if (m_isFullScreen)
      {
        glutFullScreen();
      }
      else
      {
        glutReshapeWindow(800, 600);
      }

      F11_toggled = true;
    }
  }
  else
  {
    F11_toggled = false;
  }

  if (isKeyPressed((char)27))
  {
    exit(0);
  }
}

void Game::State::reset()
{
  gen_left = std::mt19937(seed_left);
  gen_right = std::mt19937(seed_right);
}

void Game::State::mouseMotionCallback(GLint x, GLint y)
{
  auto instance = getInstance();
  // calcula a variação da posição do mouse entre frames
  instance->m_mouseDelta = {x - instance->m_windowSize.X() / 2,
                            y - instance->m_windowSize.Y() / 2};
}

void Game::State::keyboardCallback(unsigned char key, int x, int y)
{
  getInstance()->m_asciiKeys[key] = true;
}

void Game::State::keyboardUpCallback(unsigned char key, int x, int y)
{
  getInstance()->m_asciiKeys[key] = false;

  if (key >= 'a' && key <= 'z')
  {
    getInstance()->m_asciiKeys[key - ('a' - 'A')] = false;
  }
  else if (key >= 'A' && key <= 'Z')
  {
    getInstance()->m_asciiKeys[key + ('a' - 'A')] = false;
  }
  else if (key == 0)
  {
    getInstance()->m_asciiKeys[32] = false;
  }
  else if (key == 32)
  {
    getInstance()->m_asciiKeys[0] = false;
  }
}

void Game::State::specialCallback(int key, int x, int y)
{
  getInstance()->m_specialKeys[key] = true;
}

void Game::State::specialUpCallback(int key, int x, int y)
{
  getInstance()->m_specialKeys[key] = false;
}

Game::State::State()
{
  reset();

  // Lógica da construção da estrutura das salas e seus vizinhos respectivamente
  neigh = std::vector<std::vector<Vertice>>((HALL_LENGTH + 2), std::vector<Vertice>(HALL_LENGTH + 2));
  neigh[0][0] = {{ROOM_WIDTH, 0}, {0, 0}, {0, ROOM_WIDTH}};
  neigh[0][HALL_LENGTH + 1] = {{0, HALL_LENGTH * ROOM_WIDTH}, {0, (HALL_LENGTH + 1) * ROOM_WIDTH}, {ROOM_WIDTH, (HALL_LENGTH + 1) * ROOM_WIDTH}};
  neigh[HALL_LENGTH + 1][HALL_LENGTH + 1] = {{HALL_LENGTH * ROOM_WIDTH, (HALL_LENGTH + 1) * ROOM_WIDTH}, {(HALL_LENGTH + 1) * ROOM_WIDTH, (HALL_LENGTH + 1) * ROOM_WIDTH}, {(HALL_LENGTH + 1) * ROOM_WIDTH, HALL_LENGTH * ROOM_WIDTH}};
  neigh[HALL_LENGTH + 1][0] = {{HALL_LENGTH * ROOM_WIDTH, 0}, {(HALL_LENGTH + 1) * ROOM_WIDTH, 0}, {(HALL_LENGTH + 1) * ROOM_WIDTH, ROOM_WIDTH}};

  for (int i = 1; i <= HALL_LENGTH; i++)
  {
    neigh[0][i] = {{0, ROOM_WIDTH * (i - 1)}, {0, ROOM_WIDTH * i}, {0, ROOM_WIDTH * (i + 1)}};
    neigh[i][HALL_LENGTH + 1] = {{(i - 1) * ROOM_WIDTH, (HALL_LENGTH + 1) * ROOM_WIDTH}, {i * ROOM_WIDTH, (HALL_LENGTH + 1) * ROOM_WIDTH}, {(i + 1) * ROOM_WIDTH, (HALL_LENGTH + 1) * ROOM_WIDTH}};
    neigh[HALL_LENGTH + 1][HALL_LENGTH + 1 - i] = {{(HALL_LENGTH + 1) * ROOM_WIDTH, (HALL_LENGTH - i) * ROOM_WIDTH}, {(HALL_LENGTH + 1) * ROOM_WIDTH, (HALL_LENGTH + 1 - i) * ROOM_WIDTH}, {(HALL_LENGTH + 1) * ROOM_WIDTH, (HALL_LENGTH + 2 - i) * ROOM_WIDTH}};
    neigh[HALL_LENGTH + 1 - i][0] = {{(HALL_LENGTH - i) * ROOM_WIDTH, 0}, {(HALL_LENGTH + 1 - i) * ROOM_WIDTH, 0}, {(HALL_LENGTH + 2 - i) * ROOM_WIDTH, 0}};
  }
}

void Game::State::load_texture()
{
  // Inicialização dos vetores id de textura
  glGenTextures(MAX_TEXTURES, m_texIds);
  GLuint ids[HALL_LENGTH * 4] = {0};
  glGenTextures(HALL_LENGTH * 4, ids);
  for (int i = 0; i < HALL_LENGTH * 4; i++)
  {
    m_photosInfo[i].id = ids[i];
  }

  // Carregamento das texturas de ambiente
  for (int i = 0; i < MAX_TEXTURES; i++)
  {
    int width, height, nrChannels;
    std::string image_path = std::format("assets/textures/{}.jpeg", i + 1);
    unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
      // Configuração de bind de texture
      glBindTexture(GL_TEXTURE_2D, m_texIds[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      if (nrChannels == 3)
      {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
                          GL_UNSIGNED_BYTE, data);
      }
      else if (nrChannels == 4)
      {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA,
                          GL_UNSIGNED_BYTE, data);
      }
      else
      {
        std::cout << std::format("Unsupported texture format: {} channels ID: {}",
                                 nrChannels, i)
                  << std::endl;
      }

      stbi_image_free(data);
    }
    else
    {
      std::cout << std::format("Failed to load texture: {}", image_path)
                << std::endl;
    }
  }
}

void Game::State::load_photos(bool side)
{
  int initial = 0;
  if (side){
    initial = seed_right * HALL_LENGTH * 2;
  } else{
    initial = seed_left * HALL_LENGTH * 2;
  }
  
  // Carregamento das artes via textura
  for (int i = 0; i < HALL_LENGTH * 2; i++)
  {
    int width, height, nrChannels;
    std::string image_path = std::format("assets/arts/{}.jpeg", i + initial);
    unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
      m_photosInfo[i + side * HALL_LENGTH * 2].w = width;
      m_photosInfo[i + side * HALL_LENGTH * 2].h = height;
      m_photosInfo[i + side * HALL_LENGTH * 2].id_orig = i + initial;
      
      // Configuração de carregamento das texturas
      glBindTexture(GL_TEXTURE_2D, m_photosInfo[i + side * HALL_LENGTH * 2].id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      if (nrChannels == 3)
      {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
                          GL_UNSIGNED_BYTE, data);
      }
      else if (nrChannels == 4)
      {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA,
                          GL_UNSIGNED_BYTE, data);
      }
      else
      {
        std::cout << std::format("Unsupported texture format: {} channels, id: {}",
                                 nrChannels, i + initial)
                  << std::endl;
      }
      stbi_image_free(data);
    }
    else
    {
      std::cout << std::format("Failed to load texture: id {}", i + initial)
                << std::endl;
    }
  }
}

// Callbacks necessarios para consultas HTTP utilizando a biblioteca cURL
size_t Game::State::memory_writeCb(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t total_size = size * nmemb;
  std::string *str = static_cast<std::string *>(userp);
  if (str == nullptr){
    return 0;
  }
  str->append(static_cast<char *>(contents), total_size);
  return total_size;
}
size_t Game::State::curl_imageCb(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t total_size = size * nmemb;
  auto *buffer = static_cast<std::vector<uint8_t> *>(userp);
  if (!buffer)
    return 0;

  uint8_t *data = static_cast<uint8_t *>(contents);
  buffer->insert(buffer->end(), data, data + total_size);

  return total_size;
}

// Realiza a requisição de imagens via api (https://www.artsy.net)
bool Game::State::http_get(const std::string &url, std::string &out)
{

  CURL *curl;
  const std::string token = "eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiI2OGQ0NDg4MmMyZThiYjAwMGYwZmNjMDkiLCJzYWx0X2hhc2giOiI1N2Q2ZDBmOGRlNTlmNjE4MGUyYWQ2ZjM1NmJkNjZiMSIsInJvbGVzIjoidXNlciIsInBhcnRuZXJfaWRzIjpbXSwib3RwIjpmYWxzZSwiZXhwIjoxNzkwNDMzNzQyLCJpYXQiOjE3NTg4OTc3NDIsImF1ZCI6IjUzZmYxYmNjNzc2ZjcyNDBkOTAwMDAwMCIsImlzcyI6IkdyYXZpdHkiLCJqdGkiOiI2OGQ2YTY0ZTIzOTRjYjAwMTAwMDc3YjIifQ.TFF9ZxNYa8V-oohT7n2xqOsNpeL0t5LsWNnK1Anm1pM";
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (!curl)
    return false;
  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "accept: application/json");

  std::string auth_header = "X-Access-Token: " + token;
  headers = curl_slist_append(headers, auth_header.c_str());

  std::cout << "URL: " << url << std::endl;
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memory_writeCb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
  CURLcode res = curl_easy_perform(curl);

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);

  return res == CURLE_OK;
}

// Realiza o download da imagem pela url via cURL
bool Game::State::download_image(const std::string &url, std::vector<uint8_t> &buffer)
{
  CURL *curl = curl_easy_init();
  if (!curl)
    return false;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_imageCb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK)
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
  curl_easy_cleanup(curl);
  return res == CURLE_OK;
}

// Realiza o download das imagens localmente em um diretorio
bool Game::State::get_images(std::vector<std::vector<uint8_t>> &images_data, int random_page)
{
  std::string api_url = std::format("https://api.artsy.net/api/v1/artworks?page=11&size=50&sort=-iconicity", random_page); // 12 imagens de uma pagina aleatoria
  std::string json_response;
  int count = 175;
  if (!http_get(api_url, json_response))
  {
    std::cerr << "Failed to GET JSON from API.\n";
    return false;
  }
  try
  {
    auto data = json::parse(json_response);
    std::binary_semaphore bs{1};
    std::barrier b(data.size() + 1);
    for (const auto &item : data)
    {
      std::thread([&]()
                  {
                if (item.contains("images") && item["images"].is_array()) {
                  const auto& image = item["images"][0];
                  if (image.contains("image_urls")) {
                    const auto& urls = image["image_urls"];
                    std::string url = "";
                    for (auto& [key, val] : urls.items()){
                      if (key == "main" || key == "square" || key == "medium"){
                        url = val;
                        break;
                      }
                    }
                    if (url == ""){
                      auto it = urls.begin();
                      url = it.value();
                    }

                    std::cout << url << std::endl;

                    std::vector<uint8_t> image_data;
                    if (download_image(url, image_data)) {
                        std::cout << "Image downloaded into RAM. Size: " << image_data.size() << " bytes\n";
                        bs.acquire();
                        images_data.push_back(image_data);
                        std::string image_name = std::format("assets/arts/{}.jpeg", count);
                        std::ofstream outFile(image_name, std::ios::binary);
                        count++;
                        outFile.write(reinterpret_cast<const char*>(image_data.data()), image_data.size());
                        outFile.close();
                        bs.release();
                      } else {
                        std::cerr << "Failed to download image from: " << url << "\n";
                    }
                }
                b.arrive_and_drop();
            } })
          .detach();
    }
    b.arrive_and_wait();
  }
  catch (const std::exception &ex)
  {
    std::cerr << "JSON parsing failed: " << ex.what() << "\n";
    return false;
  }

  return true;
}
