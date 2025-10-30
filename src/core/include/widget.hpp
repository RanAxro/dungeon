#pragma once

#include "state.hpp"

#include <queue>
#include <optional>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <numeric>
using namespace std;
#include <SFML/Graphics.hpp>
using namespace sf;

// The alignment of widgets
enum Alignment{
  leftTop,
  leftCenter,
  leftBottom,
  centerTop,
  center,
  centerBottom,
  rightTop,
  rightCenter,
  rightBottom
};

/// Base class for all widgets

class Widget{
  private:
    static unordered_map<string, Widget*>& _registry();
    optional<string> _id;
  protected:

  public:
    Widget() = default;

    explicit Widget(optional<string> id);

    ~Widget();

    Widget(const Widget&) = delete;

    Widget& operator=(const Widget&) = delete;

    Widget(Widget&& rhs) noexcept;

    optional<string> getId();

    virtual void render(RenderWindow* window, float left, float top, float width, float height);

    static Widget* find(const string& id);
};

/// A Widget used to describe the layout

struct LayoutWidgetStruct{
  optional<string> id;
  vector<Widget*> children;
};

class LayoutWidget : public Widget{
  protected:
    vector<Widget*> _children;

  public:
    explicit LayoutWidget(LayoutWidgetStruct&& layoutWidgetStruct);

    ~LayoutWidget();
};


/// RowWidget

struct RowWidgetStruct{
  optional<string> id;
  vector<uint8_t> flex;
  vector<Widget*> children;
};

class RowWidget : public LayoutWidget{
  protected:
    vector<uint8_t> _flex;
  public:
    explicit RowWidget(RowWidgetStruct&& rowWidgetStruct);
    
    void render(RenderWindow* window, float left, float top, float width, float height) override;
};


/// ColumnWidget

struct ColumnWidgetStruct{
  optional<string> id;
  vector<uint8_t> flex;
  vector<Widget*> children;
};

class ColumnWidget : public LayoutWidget{
  protected:
    vector<uint8_t> _flex;
  public:
    explicit ColumnWidget(ColumnWidgetStruct&& columnWidgetStruct);
    
    void render(RenderWindow* window, float left, float top, float width, float height) override;
};


/// StackWidget

struct StackWidgetStruct{
  optional<string> id;
  vector<Widget*> children;
};

class StackWidget : public LayoutWidget{
  public:
    explicit StackWidget(StackWidgetStruct&& stackWidgetStruct);
    
    void render(RenderWindow* window, float left, float top, float width, float height) override;
};


/// A Widget used to constrain the layout

struct ConstraintWidgetStruct{
  optional<string> id;
  optional<float> width;
  optional<float> height;
  Alignment alignment = Alignment::center;
  Widget* child = nullptr;
};

class ConstraintWidget : public Widget{
  protected:
    optional<float> _width;
    optional<float> _height;
    Alignment _alignment;
    Widget* _child;
  public:
    explicit ConstraintWidget(ConstraintWidgetStruct&& constraintWidgetStruct);

    ~ConstraintWidget();

    void setChild(Widget* child);
    
    void render(RenderWindow* window, float left, float top, float width, float height) override;
};

/// A Widget to maintain a fixed aspect radio

struct AspectRadioWidgetStruct{
  optional<string> id;
  optional<float> aspectRadio;
  Alignment alignment = Alignment::center;
  Widget* child = nullptr;
};

class AspectRadioWidget : public ConstraintWidget{
  protected:
    optional<float> _aspectRadio;
  public:
    explicit AspectRadioWidget(AspectRadioWidgetStruct&& aspectRadioWidgetStruct);

    void render(RenderWindow* window, float left, float top, float width, float height) override;
};

/// PaddingWidget

struct PaddingWidgetStruct{
  optional<string> id;
  float left = 0;
  float top = 0;
  float right = 0;
  float bottom = 0;
  Widget* child = nullptr;
};

class PaddingWidget : public ConstraintWidget{
  public:
    float leftPadding;
    float topPadding;
    float rightPadding;
    float bottomPadding;

    explicit PaddingWidget(PaddingWidgetStruct&& paddingWidgetStruct);

    void render(RenderWindow* window, float left, float top, float width, float height) override;
};


/// A Widget for handling events

struct EventWidgetStruct{
  optional<string> id;
  unordered_map<string, function<void(const Event&, RenderWindow*, float, float, float, float)>> listeners;
  Widget* child = nullptr;
};

class EventWidget : public Widget{
  protected:
    static unordered_map<RenderWindow*, queue<Event>> _eventPool;

    unordered_map<string, function<void(const Event&, RenderWindow*, float, float, float, float)>>  _listeners;
    Widget* _child;
  public:
    explicit EventWidget(EventWidgetStruct&& eventWidgetStruct);

    ~EventWidget();

    void setListener(const string& eventId, function<void(const Event&, RenderWindow*, float, float, float, float)> listener);

    void removeListener(const string& eventId);
    
    void render(RenderWindow* window, float left, float top, float width, float height) override;

    static void putEvent(RenderWindow* window, const Event& event);

    static void clear();
};


class EventProcessor{
  public:
    static function<void(const Event&, RenderWindow*, float, float, float, float)> onMouseTap(function<void(Mouse::Button, const Vector2f&)> mouseTap);

    static function<void(const Event&, RenderWindow*, float, float, float, float)> onMouseRelease(function<void(Mouse::Button, const Vector2f&, bool isInWidget)> mouseRelease);

    static function<void(const Event&, RenderWindow*, float, float, float, float)> onMouseEnter(bool isInWidget, function<void()> mouseEnter);

    static function<void(const Event&, RenderWindow*, float, float, float, float)> onMouseLeave(bool isInWidget, function<void()> mouseLeave);
};


/// A Widget for drawing patterns

struct RenderWidgetStruct{
  optional<string> id;
  Drawable* drawable = nullptr;
  Widget* child = nullptr;
};

class RenderWidget : public Widget{
  protected:
    Drawable* _drawable;
    Widget* _child;
  public:
    explicit RenderWidget(RenderWidgetStruct&& renderWidgetStruct);

    ~RenderWidget();

    void setChild(Widget* child);

    void render(RenderWindow* window, float left, float top, float width, float height) override;
};


/// A Widget for drawing a colored rectangle

struct ColorWidgetStruct{
  optional<string> id;
  Color color = Color(0, 0, 0, 0);
  Widget* child = nullptr;
};

class ColorWidget : public RenderWidget{
  protected:
    RectangleShape* _shape();
  public:
    ColorWidget(ColorWidgetStruct&& colorWidgetStruct);

    ~ColorWidget();

    void render(RenderWindow* window, float left, float top, float width, float height) override;
};


/// A Widget for drawing text

struct TextWidgetStruct{
  optional<string> id;
  string text = "";
  Font& font;
  unsigned int size = 30;
  Color color = Color::Black;
  Alignment alignment = Alignment::center;
  Widget* child = nullptr;
};

class TextWidget : public RenderWidget{
  protected:
    Text* _text();
    Alignment _alignment;
  public:
    TextWidget(TextWidgetStruct&& textWidgetStruct);

    ~TextWidget();

    void setText(const string& text);

    Text getText();

    void render(RenderWindow* window, float left, float top, float width, float height) override;
};


/// A Widget for drawing a texture

struct TextureWidgetStruct{
  optional<string> id;
  Texture& texture;
  optional<IntRect> rectangle;
  Widget* child = nullptr;
};

class TextureWidget : public RenderWidget{
  protected:
    Sprite* _sprite();
  public:
    TextureWidget(TextureWidgetStruct&& textureWidgetStruct);

    ~TextureWidget();

    void setTexture(const Texture& texture);

    void setTexture(const Texture& texture, const IntRect& rectangle);

    void setTextureRect(const IntRect& rectangle);

    void render(RenderWindow* window, float left, float top, float width, float height) override;
};




/// packaged widgets

struct ButtonWidgetStruct{
  optional<string> id;
  TextureWidgetStruct texture;
  PaddingWidgetStruct textPadding;
  TextWidgetStruct text;
  function<void(Mouse::Button)> onClick = nullptr;
  function<void()> onFocus = nullptr;
  function<void()> onLoseFocus = nullptr;
};

class ButtonWidget : public Widget{
  protected:
    bool _isTap = false;
    bool _isFocus = false;

    Widget* _widget;
    TextureWidget* _textureWidget;
    PaddingWidget* _paddingWidget;
    TextWidget* _textWidget;

    function<void(Mouse::Button)> _onClick;
    function<void()> _onFocus;
    function<void()> _onLoseFocus;

    void setFouse(bool isFocus);
  public:
    ButtonWidget(ButtonWidgetStruct&& buttonWidgetStruct);

    ~ButtonWidget();

    void render(RenderWindow* window, float left, float top, float width, float height) override;
};