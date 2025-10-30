#pragma once

#include "entity.hpp"
#include "game.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <optional>
using namespace std;

#include "../../../lib/json.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

enum StateType{
  play,
  startMenu,
  gameMapMenu,
  settingsMenu,
  pauseMenu
};

namespace State{
  
  // constant definitions

  // if a num < e, => num is zero
  constexpr float e = 1e-6f;

  // This defines how many units the window width has. (d)
  constexpr float absoluteUnitDistance = 18.f;

  extern StateType state;

  extern Font font;
  extern Font titleFont;
  extern Font titleShadowFont;

  extern Color titleColor;
  extern Color titleShadowColor;

  extern Texture btnTexture;
  extern Texture btnFocusTexture;
  extern Texture floorTexture;
  extern Texture playerBigTexture;
  extern Texture doorTexture;
  extern Texture treasureBigTexture;
  extern Texture bombBigTexture;

  extern nlohmann::json enLang;
  extern nlohmann::json zhLang;

  // global variables

  extern string lang;

  extern RenderWindow* window;

  extern Vector2u windowSize_px;  // (px)

  extern Vector2f windowSize;  // (d)

  extern float scale;  // (d/px)

  extern float pxpd; // (px/d)

  // game state

  extern int fps;

  extern optional<vector<vector<int>>> map;

  extern optional<Texture> mapTexture;

  extern vector<Entity*> treasure;

  extern vector<DisposableEntity*> bomb;

  extern Entity* player;

  extern int score;

  // extern 

  extern string tr(const string& key);
  
  extern void initState();

  extern void readState();

  extern void saveState();

  extern void close();
}