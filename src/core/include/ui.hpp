#pragma once

#include "state.hpp"
#include "widget.hpp"
#include "packageWidget.hpp"

#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>
using namespace sf;

namespace ui{
  extern Widget* startMenuUi;

  extern Widget* gameMapMenuUi;

  extern Widget* settingsMenuUi;

  extern Widget* pauseMenuUi;

  extern Widget* playHUD;

  extern void initUi();
}