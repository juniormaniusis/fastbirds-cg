#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "nuvens.hpp"
#include "pipe.hpp"
#include "bird.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;
  void checkCollisions();

 private:
  GLuint m_objectsProgram{};
  int m_viewportWidth{};
  int m_viewportHeight{};
  GameData m_gameData;

  nuvens m_nuvens;
  Bird m_bird;
  Pipes m_pipes;

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};
  ImFont* m_font_score{};
  std::default_random_engine m_randomEngine;

  void restart();
  void update();
};

#endif