#include "starlayers.hpp"

#include <cppitertools/itertools.hpp>

void StarLayers::initializeGL(GLuint program, int quantity) {
  terminateGL();

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // auto &re{m_randomEngine};
  std::uniform_real_distribution<float> distPos(-1.0f, 1.0f);
  std::uniform_real_distribution<float> distIntensity(0.5f, 1.0f);

  // p cada camada de estrelas
  for (auto &&[index, nuvem] : iter::enumerate(m_nuvens)) {
    nuvem.m_translation = glm::vec2(0);

    // std::vector<glm::vec3> data(0);
    // for ([[maybe_unused]] auto i : iter::range(0, layer.m_quantity)) {
    //   data.emplace_back(distPos(re), distPos(re), 0);
    //   data.push_back(glm::vec3(1) * distIntensity(re));
    // }
    /*
      // Generate VBO
      abcg::glGenBuffers(1, &layer.m_vbo);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, layer.m_vbo);
      abcg::glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(glm::vec3),
                         data.data(), GL_STATIC_DRAW);
      abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

      // Get location of attributes in the program
      GLint positionAttribute{abcg::glGetAttribLocation(m_program,
      "inPosition")}; GLint colorAttribute{abcg::glGetAttribLocation(m_program,
      "inColor")};

      // Create VAO
      abcg::glGenVertexArrays(1, &layer.m_vao);

      // Bind vertex attributes to current VAO
      abcg::glBindVertexArray(layer.m_vao);

      abcg::glBindBuffer(GL_ARRAY_BUFFER, layer.m_vbo);
      abcg::glEnableVertexAttribArray(positionAttribute);
      abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(glm::vec3) * 2, nullptr);
      abcg::glEnableVertexAttribArray(colorAttribute);
      abcg::glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(glm::vec3) * 2,
                                  reinterpret_cast<void *>(sizeof(glm::vec3)));
      abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

      // End of binding to current VAO
      abcg::glBindVertexArray(0);
      /*/
    nuvem = createNuvem();
  }
}

StarLayers::Nuvem StarLayers::createNuvem() {
  Nuvem nuvem;

  std::array<glm::vec2, 18> positions{
      glm::vec2{0, 1}, glm::vec2{0, 0}, glm::vec2{4, 0},

      glm::vec2{0, 1}, glm::vec2{4, 0}, glm::vec2{4, 1},

      glm::vec2{4, 1}, glm::vec2{3, 2}, glm::vec2{4, 2},

      glm::vec2{4, 1}, glm::vec2{3, 2}, glm::vec2{3, 1},

      glm::vec2{1, 2}, glm::vec2{2, 2}, glm::vec2{2, 1},

      glm::vec2{1, 2}, glm::vec2{2, 1}, glm::vec2{1, 1},

  };

  auto azul = glm::vec4{.84, .92, 1, .7};
  auto azulc = glm::vec4{.76, .82, .87, .7};
  std::array<glm::vec3, 18> colors{azulc,  azulc, azulc, azulc,  azulc,  azulc,
                                   azulc, azul, azulc, azul, azulc, azulc,
                                   azulc, azulc, azul,  azul, azulc, azulc};

  for (int i = 0; i < 18; i++) {
    positions[i] /= glm::vec2{4, 2};
    // positions[i] *= 0.2f;
  }

  // cria o VBO de posição
  abcg::glGenBuffers(1, &nuvem.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, nuvem.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // cria o VBO de cores
  abcg::glGenBuffers(1, &nuvem.m_color_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, nuvem.m_color_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4),
                     colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // pega os atributos
  const auto positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  const auto colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // cria o VAO
  abcg::glGenVertexArrays(1, &nuvem.m_vao);

  // vincula os VBOS ao VAO
  abcg::glBindVertexArray(nuvem.m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, nuvem.m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, nuvem.m_color_vbo);
  abcg::glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
  return nuvem;
}

void StarLayers::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &nuvem : m_nuvens) {
    abcg::glBindVertexArray(nuvem.m_vao);

    for (auto i : {-2, 0, 2}) {
      for (auto j : {-2, 0, 2}) {
        abcg::glUniform2f(m_translationLoc, nuvem.m_translation.x + j,
                          nuvem.m_translation.y + i);
        abcg::glEnable(GL_BLEND);
        abcg::glDrawArrays(GL_TRIANGLES, 0, 18);
      }
    }
    abcg::glBindVertexArray(0);
  }
  abcg::glUseProgram(0);
}

void StarLayers::terminateGL() {
  for (auto &nuvem : m_nuvens) {
    abcg::glDeleteBuffers(1, &nuvem.m_vbo);
    abcg::glDeleteBuffers(1, &nuvem.m_color_vbo);
    abcg::glDeleteVertexArrays(1, &nuvem.m_vao);
  }
}

void StarLayers::update(const Ship &ship, float deltaTime) {
  for (auto &&[index, layer] : iter::enumerate(m_nuvens)) {
    const auto layerSpeedScale{1.0f / (index + 2.0f)};
    layer.m_translation -= ship.m_velocity * deltaTime * layerSpeedScale;

    // Wrap-around
    if (layer.m_translation.x < -1.0f) layer.m_translation.x += 2.0f;
    if (layer.m_translation.x > +1.0f) layer.m_translation.x -= 2.0f;
    if (layer.m_translation.y < -1.0f) layer.m_translation.y += 2.0f;
    if (layer.m_translation.y > +1.0f) layer.m_translation.y -= 2.0f;
  }
}