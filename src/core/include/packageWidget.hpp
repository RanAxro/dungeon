#pragma once

#include "state.hpp"
#include "widget.hpp"

#include <string>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

namespace packageWidget{

  struct DefaultButtonStruct{
    optional<string> id;
    Texture& texture = State::btnTexture;
    Texture& focusTexture = State::btnFocusTexture;
    string text = "";
    function<void(Mouse::Button)> onClick = nullptr;
  };

  class DefaultButton : public Widget{
    protected:
      ButtonWidget* _widget;

      Texture& _texture;
      Texture& _focusTexture;
      function<void(Mouse::Button)> _onClick;
    public:
      DefaultButton(DefaultButtonStruct&& defaultButtonStruct);

      ~DefaultButton();

      void render(RenderWindow* window, float left, float top, float width, float height) override;
  };

}
