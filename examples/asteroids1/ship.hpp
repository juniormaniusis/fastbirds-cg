#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Asteroids;
class Bullets;
class OpenGLWindow;
class StarLayers;

class Ship {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);
  void setRotation(float rotation) { m_rotation = rotation; }

 private:
  friend Asteroids;
  friend Bullets;
  friend OpenGLWindow;
  friend StarLayers;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  
  GLuint m_vbo{};
  GLuint m_ebo{};

  GLuint m_color_vbo{};
  GLuint m_color_ebo{};

  glm::vec4 m_color{1};
  float m_rotation{};
  float m_scale{0.150f};
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(2, 0)};
  
  glm::vec2 m_acceleration{glm::vec2(0, 0)};

  abcg::ElapsedTimer m_trailBlinkTimer;
  abcg::ElapsedTimer m_bulletCoolDownTimer;
};
#endif