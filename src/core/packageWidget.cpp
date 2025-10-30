#include "include/packageWidget.hpp"


namespace packageWidget{

  DefaultButton::DefaultButton(DefaultButtonStruct&& defaultButtonStruct) :
    _texture(defaultButtonStruct.texture),
    _focusTexture(defaultButtonStruct.focusTexture),
    _onClick(move(defaultButtonStruct.onClick)),
    Widget(move(defaultButtonStruct.id))
  {
    if(!this->getId().has_value()){
      throw runtime_error("find no id when new DefaultButton");
    }

    _widget = new ButtonWidget({
      .texture = {
        .id = *this->getId() + "Texture",
        .texture = _texture
      },
      .textPadding = {
        .id = *this->getId() + "TextPadding",
        .left = 0.4f,
        .top = 0.4f,
        .right = 0.4f,
        .bottom = 0.4f
      },
      .text = {
        .id = *this->getId() + "Text",
        .text = move(defaultButtonStruct.text),
        .font = State::font,
        .size = 100,
        .color = Color::Black
      },
      .onClick = _onClick,
      .onFocus = [this](){
        Widget* btnW = Widget::find(*this->getId() + "Texture");
        TextureWidget* textureWidget = static_cast<TextureWidget*>(btnW);
        textureWidget->setTexture(this->_focusTexture);

        Widget* textPaddingW = Widget::find(*this->getId() + "TextPadding");
        PaddingWidget* paddingWidget = static_cast<PaddingWidget*>(textPaddingW);
        paddingWidget->topPadding = 0.5f;
        paddingWidget->bottomPadding = 0.3f;
      },
      .onLoseFocus = [this](){
        Widget* btnW = Widget::find(*this->getId() + "Texture");
        TextureWidget* textureWidget = static_cast<TextureWidget*>(btnW);
        textureWidget->setTexture(this->_texture);

        Widget* textPaddingW = Widget::find(*this->getId() + "TextPadding");
        PaddingWidget* paddingWidget = static_cast<PaddingWidget*>(textPaddingW);
        paddingWidget->topPadding = 0.4f;
        paddingWidget->bottomPadding = 0.4f;
      }
    });
  }

  DefaultButton::~DefaultButton(){
    delete _widget;
  }

  void DefaultButton::render(RenderWindow* window, float left, float top, float width, float height){
    _widget->render(window, left, top, width, height);
  }

}
