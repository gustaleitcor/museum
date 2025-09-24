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
#define STB_IMAGE_IMPLEMENTATION
#include "include/utils/stb_image.hpp"


using json = nlohmann::json;
void Game::State::update() {
  static bool F11_toggled = false;

  if (isSpecialPressed(Key::Special::F11)) {
    if (!F11_toggled) {
      m_isFullScreen = !m_isFullScreen;
      if (m_isFullScreen) {
        glutFullScreen();
      } else {
        glutReshapeWindow(800, 600);
      }

      F11_toggled = true;
    }
  } else {
    F11_toggled = false;
  }

  if (isKeyPressed((char)27)) {
    exit(0);
  }

}

void Game::State::reset() {
  gen_left = std::mt19937(seed_left);
  gen_right = std::mt19937(seed_right);
}

void Game::State::mouseCallback(int button, int state, int x, int y) {
  // TODO
}

void Game::State::mouseMotionCallback(GLint x, GLint y) {
  auto instance = getInstance();
  instance->m_mouseDelta = {x - instance->m_windowSize.X() / 2,
                            y - instance->m_windowSize.Y() / 2};
}

void Game::State::keyboardCallback(unsigned char key, int x, int y) {
  getInstance()->m_asciiKeys[key] = true;
}

void Game::State::keyboardUpCallback(unsigned char key, int x, int y) {
  getInstance()->m_asciiKeys[key] = false;

  if (key >= 'a' && key <= 'z') {
    getInstance()->m_asciiKeys[key - ('a' - 'A')] = false;
  } else if (key >= 'A' && key <= 'Z') {
    getInstance()->m_asciiKeys[key + ('a' - 'A')] = false;
  } else if (key == 0) {
    getInstance()->m_asciiKeys[32] = false;
  } else if (key == 32) {
    getInstance()->m_asciiKeys[0] = false;
  }
}

void Game::State::specialCallback(int key, int x, int y) {
  getInstance()->m_specialKeys[key] = true;
}

void Game::State::specialUpCallback(int key, int x, int y) {
  getInstance()->m_specialKeys[key] = false;
}

Game::State::State() {
  reset();
}

void Game::State::load_texture() {
  glGenTextures(MAX_TEXTURES, m_texIds);
  glGenTextures(24, m_photosIds);

  for (int i = 0; i < MAX_TEXTURES; i++) {
    int width, height, nrChannels;
    std::string image_path = std::format("assets/textures/{}.png", i + 1);
    unsigned char *data =
        stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
      glBindTexture(GL_TEXTURE_2D, m_texIds[i]);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (nrChannels == 3) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
                          GL_UNSIGNED_BYTE, data);
      } else if (nrChannels == 4) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA,
                          GL_UNSIGNED_BYTE, data);
      } else {
        std::cout << std::format("Unsupported texture format: {} channels",
                                 nrChannels)
                  << std::endl;
      }

      stbi_image_free(data);
    } else {
      std::cout << std::format("Failed to load texture: {}", image_path)
                << std::endl;
    }
  }
}

//TODO: side = 0 -> left, side = 1 -> right
void Game::State::load_photos(bool side)
{
  std::vector<std::vector<uint8_t>> images;
  if(side)
    get_images(images, random_number_left(250) + 1);
  else
    get_images(images, random_number_right(250) + 252);
  
  for (int i = 0; i < images.size(); i++) {
    int width, height, nrChannels;

    std::cout << "index: " << i << "tamanho de images" << images.size() << std::endl;
    // std::string image_path = std::format("assets/textures/{}.png", i + 1);
    unsigned char *data =
        stbi_load_from_memory(images[i].data(), static_cast<int>(images[i].size()), &width, &height, &nrChannels, 0);
    //TODO: struct width and height

    std::cout << "W: " << width << std::endl;
    std::cout << "H: " << height << std::endl;
    if (data) {
      glBindTexture(GL_TEXTURE_2D, m_photosIds[i]);
      std::cout << "W: " << width << std::endl;
      std::cout << "H: " << height << std::endl;
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      if (nrChannels == 3) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
                          GL_UNSIGNED_BYTE, data);
      } else if (nrChannels == 4) {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA,
                          GL_UNSIGNED_BYTE, data);
      } else {
        std::cout << std::format("Unsupported texture format: {} channels",
                                 nrChannels)
                  << std::endl;
      }

      stbi_image_free(data);
    } else {
      std::cout << std::format("Failed to load texture: id {}", i)
                << std::endl;
    }
  }
}

//escreve dados no vetor contents
size_t Game::State::memory_writeCb(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t total_size = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    if (str == nullptr) return 0;
    str->append(static_cast<char*>(contents), total_size);
    return total_size;
}
size_t Game::State::curl_imageCb(void* contents, size_t size, size_t nmemb, void* userp)
{
  size_t total_size = size * nmemb;
  auto* buffer = static_cast<std::vector<uint8_t>*>(userp);
  if (!buffer) return 0;

  uint8_t* data = static_cast<uint8_t*>(contents);
  buffer->insert(buffer->end(), data, data + total_size);

  return total_size;
}

//realiza get e pega a string de resposta
bool Game::State::http_get(const std::string& url, std::string& out) {
    
    CURL* curl;
    const std::string token = "eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiI2OGQ0NDg4MmMyZThiYjAwMGYwZmNjMDkiLCJzYWx0X2hhc2giOiJjMjUxOTM1NTE2ZDc3YmVjMDc0NDk3MjY3YTM0MTFlMiIsInJvbGVzIjoidXNlciIsInBhcnRuZXJfaWRzIjpbXSwib3RwIjpmYWxzZSwiZXhwIjoxNzkwMjc4NjY1LCJpYXQiOjE3NTg3NDI2NjUsImF1ZCI6IjUzZmYxYmNjNzc2ZjcyNDBkOTAwMDAwMCIsImlzcyI6IkdyYXZpdHkiLCJqdGkiOiI2OGQ0NDg4OTNhYmFkMGMwZDhiNWIyMWQifQ.j65ZRPbMzWJn6ci5lULdu7YWBze5HIGoUm2iJi6O3e8";
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) return false;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "accept: application/json");

    std::string auth_header = "X-Access-Token: " + token;
    headers = curl_slist_append(headers, auth_header.c_str());

    std::cout << "URL: " << url << std::endl;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    //coleta string de resposta
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memory_writeCb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
    // std::cout << "C" << std::endl;
    CURLcode res = curl_easy_perform(curl);
    // if (res != CURLE_OK)
    //     std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
    // else
    //     std::cout << "Response:\n" << out << '\n';
    // std::cout << "D" << std::endl;
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}

//coloca a imagem em um vector
bool Game::State::download_image(const std::string& url, std::vector<uint8_t>& buffer) {
  CURL* curl = curl_easy_init();
  if (!curl) return false;

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

bool Game::State::get_images(std::vector<std::vector<uint8_t>>& images_data, int random_page)
{
  std::string api_url = std::format("https://api.artsy.net/api/v1/artworks?page={}&size=12&sort=-iconicity", random_page); //12 imagens de uma pagina aleatoria
  // std::format(api_url, random_page);
  std::string json_response;
  if (!http_get(api_url, json_response)) {
      std::cerr << "Failed to GET JSON from API.\n";
      return false;
  }
    try {
        auto data = json::parse(json_response);
        std::binary_semaphore bs{1};
        std::barrier b(data.size() + 1);
        std::cout << "QUANTIDADE DE IMAGENS: " << data.size() << std::endl;
        // std::cout << data << std::endl;
        for (const auto& item : data) {
            // std::cout << item << std::endl;
            std::thread([&](){
                if (item.contains("images") && item["images"].is_array()) {
                  const auto& image = item["images"][0];
                  std::cout << image["image_urls"]["main"] << std::endl;
                  if (image.contains("image_urls") && image["image_urls"].contains("main")) {
                      std::string main_url = image["image_urls"]["main"];
                      std::cout << "Found main image URL: " << main_url << "\n";

                      std::vector<uint8_t> image_data;
                      if (download_image(main_url, image_data)) {
                          std::cout << "Image downloaded into RAM. Size: " << image_data.size() << " bytes\n";
                          bs.acquire();
                          images_data.push_back(image_data);
                          bs.release();
                        } else {
                          std::cerr << "Failed to download image from: " << main_url << "\n";
                      }
                }
                b.arrive();
            }
            }).detach();
          }
          //TODO: THREAD JOIN
          b.arrive_and_wait();
    } catch (const std::exception& ex) {
        std::cerr << "JSON parsing failed: " << ex.what() << "\n";
        return false;
    }

    return true;


}
