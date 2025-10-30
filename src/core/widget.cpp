#include "include/widget.hpp"


/// Widget

unordered_map<string, Widget*>& Widget::_registry(){
  static unordered_map<string, Widget*> m;
  return m;
}

Widget::Widget(optional<string> id) : _id(move(id)){
  if(id.has_value()){
    auto [it, inserted] = _registry().emplace(*_id, this);
    if(!inserted) throw runtime_error("duplicate id: " + id.value());
  }
}

Widget::~Widget(){
  if(_id.has_value()) _registry().erase(*_id);
}

Widget::Widget(Widget&& rhs) noexcept : _id(move(rhs._id)){
  // rhs._id.clear();
  if(_id.has_value()) _registry()[*_id] = this;
}

optional<string> Widget::getId(){
  return _id;
}

void Widget::render(RenderWindow* window, float left, float top, float width, float height){

}

Widget* Widget::find(const string& id){
  auto it = _registry().find(id);
  return it == _registry().end() ? nullptr : it->second;
}



/// LayoutWidget

LayoutWidget::LayoutWidget(LayoutWidgetStruct&& layoutWidgetStruct) :
  _children(move(layoutWidgetStruct.children)),
  Widget(move(layoutWidgetStruct.id)){
  
}

LayoutWidget::~LayoutWidget(){
  for(Widget* child : _children){
    delete child;
  }
}


/// RowWidget

RowWidget::RowWidget(RowWidgetStruct&& rowWidgetStruct) :
  _flex(move(rowWidgetStruct.flex)),
  LayoutWidget({.id = move(rowWidgetStruct.id), .children = move(rowWidgetStruct.children)}){

}

void RowWidget::render(RenderWindow* window, float left, float top, float width, float height){
  uint64_t sumFlex = accumulate(_flex.begin(), _flex.end(), 0.f);
  float unit = 1 / static_cast<float>(sumFlex);

  float counter = 0;
  for(uint16_t i = 0; i < _children.size(); i++){
    float proportion = static_cast<float>(_flex[i]) * unit;
    if(_children[i]){
      _children[i]->render(window, left + counter * width, top, proportion * width, height);
    }

    counter += proportion;
  }
}


/// ColumnWidget

ColumnWidget::ColumnWidget(ColumnWidgetStruct&& columnWidgetStruct) :
  _flex(move(columnWidgetStruct.flex)),
  LayoutWidget({.id = move(columnWidgetStruct.id), .children = move(columnWidgetStruct.children)}){

}

void ColumnWidget::render(RenderWindow* window, float left, float top, float width, float height){
  uint64_t sumFlex = accumulate(_flex.begin(), _flex.end(), 0.f);
  float unit = 1 / static_cast<float>(sumFlex);

  float counter = 0;
  for(uint16_t i = 0; i < _children.size(); i++){
    float proportion = static_cast<float>(_flex[i]) * unit;
    if(_children[i]){
      _children[i]->render(window, left, top + counter * height, width, proportion * height);
    }

    counter += proportion;
  }
}


/// StackWidget
StackWidget::StackWidget(StackWidgetStruct&& stackWidgetStruct) :
  LayoutWidget({.id = move(stackWidgetStruct.id), .children = move(stackWidgetStruct.children)}){

}

void StackWidget::render(RenderWindow* window, float left, float top, float width, float height){
  for(Widget* child : _children){
    if(child){
      child->render(window, left, top, width, height);
    }
  }
}


/// ConstraintWidget

ConstraintWidget::ConstraintWidget(ConstraintWidgetStruct&& constraintWidgetStruct) :
  _width(move(constraintWidgetStruct.width)),
  _height(move(constraintWidgetStruct.height)),
  _alignment(move(constraintWidgetStruct.alignment)),
  _child(move(constraintWidgetStruct.child)),
  Widget(move(constraintWidgetStruct.id)){

}

ConstraintWidget::~ConstraintWidget(){
  delete _child;
}

void ConstraintWidget::setChild(Widget* child){
  delete _child;
  _child = child;
}

void ConstraintWidget::render(RenderWindow* window, float left, float top, float width, float height){
  if(_child == nullptr) return;

  float drawWidth = width;
  float drawHeight = height;

  if(_width.has_value() &&_width >= 0 && _width < width) drawWidth = *_width;
  if(_height.has_value() && _height >= 0 && _height < height)  drawHeight = *_height;

  switch(_alignment)
  {
  case Alignment::leftTop:
    _child->render(window, left, top, drawWidth, drawHeight);
    break;
  case Alignment::leftCenter:
    _child->render(window, left, top + 0.5 * (height - drawHeight), drawWidth, drawHeight);
    break;
  case Alignment::leftBottom:
    _child->render(window, left, top + height - drawHeight, drawWidth, drawHeight);
    break;
  case Alignment::centerTop:
    _child->render(window, left + 0.5 * (width - drawWidth), top, drawWidth, drawHeight);
    break;
  case Alignment::center:
    _child->render(window, left + 0.5 * (width - drawWidth), top + 0.5 * (height - drawHeight), drawWidth, drawHeight);
    break;
  case Alignment::centerBottom:
    _child->render(window, left + 0.5 * (width - drawWidth), top + height - drawHeight, drawWidth, drawHeight);
    break;
  case Alignment::rightTop:
    _child->render(window, left + width - drawWidth, top, drawWidth, drawHeight);
    break;
  case Alignment::rightCenter:
    _child->render(window, left + width - drawWidth, top + 0.5 * (height - drawHeight), drawWidth, drawHeight);
    break;
  case Alignment::rightBottom:
    _child->render(window, left + width - drawWidth, top + height - drawHeight, drawWidth, drawHeight);
    break;
  }
}

/// AspectRadioWidget

AspectRadioWidget::AspectRadioWidget(AspectRadioWidgetStruct&& aspectRadioWidgetStruct) :
  _aspectRadio(move(aspectRadioWidgetStruct.aspectRadio)),
  ConstraintWidget({
    .id = move(aspectRadioWidgetStruct.id),
    .alignment = move(aspectRadioWidgetStruct.alignment),
    .child = move(aspectRadioWidgetStruct.child)
  }){

}

void AspectRadioWidget::render(RenderWindow* window, float left, float top, float width, float height){
  if(_child == nullptr) return;

  if(!_aspectRadio.has_value()) {
    _child->render(window, left, top, width, height);
    return;
  }

  float targetWidth = width;
  float targetHeight = height;

  if(width / height > _aspectRadio){
    targetWidth = height * *_aspectRadio;
  }else{
    targetHeight = width / *_aspectRadio;
  }

  switch(_alignment){
    case Alignment::leftTop:
      _child->render(window, left, top, targetWidth, targetHeight);
      break;
    case Alignment::leftCenter:
      _child->render(window, left, top + 0.5 * (height - targetHeight), targetWidth, targetHeight);
      break;
    case Alignment::leftBottom:
      _child->render(window, left, top + height - targetHeight, targetWidth, targetHeight);
      break;
    case Alignment::centerTop:
      _child->render(window, left + 0.5 * (width - targetWidth), top, targetWidth, targetHeight);
      break;
    case Alignment::center:
      _child->render(window, left + 0.5 * (width - targetWidth), top + 0.5 * (height - targetHeight), targetWidth, targetHeight);
      break;
    case Alignment::centerBottom:
      _child->render(window, left + 0.5 * (width - targetWidth), top + height - targetHeight, targetWidth, targetHeight);
      break;
    case Alignment::rightTop:
      _child->render(window, left + width - targetWidth, top, targetWidth, targetHeight);
      break;
    case Alignment::rightCenter:
      _child->render(window, left + width - targetWidth, top + 0.5 * (height - targetHeight), targetWidth, targetHeight);
      break;
    case Alignment::rightBottom:
      _child->render(window, left + width - targetWidth, top + height - targetHeight, targetWidth, targetHeight);
      break;
  }
}


/// PaddingWidget

PaddingWidget::PaddingWidget(PaddingWidgetStruct&& paddingWidgetStruct) :
  leftPadding(paddingWidgetStruct.left),
  topPadding(paddingWidgetStruct.top),
  rightPadding(paddingWidgetStruct.right),
  bottomPadding(paddingWidgetStruct.bottom),
  ConstraintWidget({
    .id = move(paddingWidgetStruct.id),
    .child = move(paddingWidgetStruct.child)
  }){

}

void PaddingWidget::render(RenderWindow* window, float left, float top, float width, float height){
  if(_child == nullptr) return;

  float targetLeft = leftPadding;
  float targetTop = topPadding;
  float targetRight = rightPadding;
  float targetBottom = bottomPadding;

  if(leftPadding + rightPadding > width){
    float scaleX = width / (leftPadding + rightPadding);
    targetLeft *= scaleX;
    targetRight *= scaleX;
  }
  if(topPadding + bottomPadding > height){
    float scaleY = height / (topPadding + bottomPadding);
    targetTop *= scaleY;
    targetBottom *= scaleY;
  }

  _child->render(
    window,
    left + targetLeft,
    top + targetTop,
    width - targetLeft - targetRight,
    height - targetTop - targetBottom
  );
}



/// EventWidget

unordered_map<RenderWindow*, queue<Event>> EventWidget::_eventPool;

EventWidget::EventWidget(EventWidgetStruct&& eventWidgetStruct) :
  _listeners(move(eventWidgetStruct.listeners)),
  _child(move(eventWidgetStruct.child)),
  Widget(move(eventWidgetStruct.id)){

}

EventWidget::~EventWidget(){
  delete _child;
}

void EventWidget::setListener(const string& eventId, function<void(const Event&, RenderWindow*, float, float, float, float)> listener){
  _listeners[eventId] = listener;
}

void EventWidget::removeListener(const string& eventId){
  _listeners.erase(eventId);
}

void EventWidget::render(RenderWindow* window, float left, float top, float width, float height){
  if(_eventPool.count(window)){
    queue<Event> eventQueue = _eventPool[window];
    while(!eventQueue.empty()){
      const Event& event = eventQueue.front();
      eventQueue.pop();
      for(auto listener : _listeners){
        listener.second(event, window, left, top, width, height);
      }
    }
  }

  if(_child) _child->render(window, left, top, width, height);
}

void EventWidget::putEvent(RenderWindow* window, const Event& event){
  _eventPool[window].push(event);
};

void EventWidget::clear(){
  _eventPool.clear();
};


function<void(const Event&, RenderWindow*, float, float, float, float)> EventProcessor::onMouseTap(function<void(Mouse::Button, const Vector2f&)> mouseTap){
  return [mouseTap](const Event& event, RenderWindow* window, float left, float top, float width, float height){
    if(auto e = event.getIf<Event::MouseButtonPressed>()){
      Vector2f pos = {static_cast<float>(e->position.x) * State::scale, static_cast<float>(e->position.y) * State::scale};
      
      if(pos.x >= left && pos.x <= left + width && pos.y >= top && pos.y <= top + height){
        mouseTap(e->button, pos);
      }
    }
  };
};

function<void(const Event&, RenderWindow*, float, float, float, float)> EventProcessor::onMouseRelease(function<void(Mouse::Button, const Vector2f&, bool isInWidget)> mouseRelease){
  return [mouseRelease](const Event& event, RenderWindow* window, float left, float top, float width, float height){
    if(auto e = event.getIf<Event::MouseButtonReleased>()){
      Vector2f pos = {static_cast<float>(e->position.x) * State::scale, static_cast<float>(e->position.y) * State::scale};
      
      if(pos.x >= left && pos.x <= left + width && pos.y >= top && pos.y <= top + height){
        mouseRelease(e->button, pos, true);
      }else{
        mouseRelease(e->button, pos, false);
      }
    }
  };
};

function<void(const Event&, RenderWindow*, float, float, float, float)> EventProcessor::onMouseEnter(bool isInWidget, function<void()> mouseEnter){
  return [isInWidget, mouseEnter](const Event& event, RenderWindow* window, float left, float top, float width, float height){
    if(auto e = event.getIf<Event::MouseMoved>()){
      Vector2f pos = {static_cast<float>(e->position.x) * State::scale, static_cast<float>(e->position.y) * State::scale};
      
      if(isInWidget == false && pos.x >= left && pos.x <= left + width && pos.y >= top && pos.y <= top + height){
        mouseEnter();
      }
    }
  };
};

function<void(const Event&, RenderWindow*, float, float, float, float)> EventProcessor::onMouseLeave(bool isInWidget, function<void()> mouseLeave){
  return [isInWidget, mouseLeave](const Event& event, RenderWindow* window, float left, float top, float width, float height){
    if(auto e = event.getIf<Event::MouseMoved>()){
      Vector2f pos = {static_cast<float>(e->position.x) * State::scale, static_cast<float>(e->position.y) * State::scale};
      
      if(isInWidget == true && pos.x < left || pos.x > left + width || pos.y < top || pos.y > top + height){
        mouseLeave();
      }
    }
  };
};


/// RenderWidget

RenderWidget::RenderWidget(RenderWidgetStruct&& renderWidgetStruct) :
  _drawable(renderWidgetStruct.drawable),
  _child(renderWidgetStruct.child),
  Widget(move(renderWidgetStruct.id)){

}

RenderWidget::~RenderWidget(){
  delete _child;
}

void RenderWidget::setChild(Widget* child){
  delete _child;
  _child = child;
}

void RenderWidget::render(RenderWindow* window, float left, float top, float width, float height){
  window->draw(*_drawable);
  if(_child) _child->render(window, left, top, width, height);
}


/// ColorWidget

ColorWidget::ColorWidget(ColorWidgetStruct&& colorWidgetStruct) :
  RenderWidget({
    .id = move(colorWidgetStruct.id),
    .drawable = new RectangleShape(),
    .child = move(colorWidgetStruct.child)
  })
{
  _shape()->setFillColor(move(colorWidgetStruct.color));
}

ColorWidget::~ColorWidget(){
  delete _shape();
  RenderWidget::~RenderWidget();
}

void ColorWidget::render(RenderWindow* window, float left, float top, float width, float height){
  RectangleShape* shape = _shape();
  shape->setPosition({left, top});
  shape->setSize({width, height});
  RenderWidget::render(window, left, top, width, height);
}

RectangleShape* ColorWidget::_shape(){
  return static_cast<RectangleShape*>(_drawable);
}



/// TextWidget
TextWidget::TextWidget(TextWidgetStruct&& textWidgetStruct) :
  _alignment(move(textWidgetStruct.alignment)),
  RenderWidget({
    .id = move(textWidgetStruct.id),
    .drawable = new Text(
      textWidgetStruct.font,
      textWidgetStruct.text,
      textWidgetStruct.size
    ),
    .child = move(textWidgetStruct.child)
  })
{
  _text()->setFillColor(move(textWidgetStruct.color));
}

TextWidget::~TextWidget(){
  delete _text();
  RenderWidget::~RenderWidget();
}

Text* TextWidget::_text(){
  return static_cast<Text*>(_drawable);
}

void TextWidget::setText(const string& text){
  Text* textWidget = _text();
  textWidget->setString(text);
}

void TextWidget::render(RenderWindow* window, float left, float top, float width, float height){
  Text* text = _text();
  Rect<float> textRect = text->getLocalBounds();
  text->setOrigin({textRect.position.x, textRect.position.y});
  
  float scaleX = width / textRect.size.x;
  float scaleY = height / textRect.size.y;
  float scale = min(scaleX, scaleY);
  text->setScale({scale, scale});

  float targetLeft = left;
  float targetTop = top;
  float targetWidth = textRect.size.x * scale;
  float targetHeight = textRect.size.y * scale;
  
  switch(_alignment){
    case Alignment::leftTop:
      text->setPosition({left, top});
      break;
    case Alignment::leftCenter:
      text->setPosition({left, top + 0.5f * (height - targetHeight)});
      break;
    case Alignment::leftBottom:
      text->setPosition({left, top + height - targetHeight});
      break;
    case Alignment::centerTop:
      text->setPosition({left + 0.5f * (width - targetWidth), top});
      break;
    case Alignment::center:
      text->setPosition({left + 0.5f * (width - targetWidth), top + 0.5f * (height - targetHeight)});
      break;
    case Alignment::centerBottom:
      text->setPosition({left + 0.5f * (width - targetWidth), top + height - targetHeight});
      break;
    case Alignment::rightTop:
      text->setPosition({left + width - targetWidth, top});
      break;
    case Alignment::rightCenter:
      text->setPosition({left + width - targetWidth, top + 0.5f * (height - targetHeight)});
      break;
    case Alignment::rightBottom:
      text->setPosition({left + width - targetWidth, top + height - targetHeight});
      break;
  }

  RenderWidget::render(window, left, top, width, height);
}



/// TextureWidget

TextureWidget::TextureWidget(TextureWidgetStruct&& textureWidgetStruct) :
  RenderWidget({
    .id = move(textureWidgetStruct.id),
    .drawable = new Sprite(textureWidgetStruct.texture),
    .child = move(textureWidgetStruct.child)
  })
{
  if(textureWidgetStruct.rectangle.has_value()) _sprite()->setTextureRect(move(*textureWidgetStruct.rectangle));
}

TextureWidget::~TextureWidget(){
  delete _sprite();
  RenderWidget::~RenderWidget();
}

Sprite* TextureWidget::_sprite(){
  return static_cast<Sprite*>(_drawable);
}

void TextureWidget::render(RenderWindow* window, float left, float top, float width, float height){
  Sprite* sprite = _sprite();
  sprite->setPosition({left, top});
  Rect<int> textureRect = sprite->getTextureRect();
  sprite->setScale({width / textureRect.size.x, height / textureRect.size.y});
  RenderWidget::render(window, left, top, width, height);
}

void TextureWidget::setTexture(const Texture& texture){
  Sprite* sprite = _sprite();
  sprite->setTexture(texture);
}

void TextureWidget::setTexture(const Texture& texture, const IntRect& rectangle){
  Sprite* sprite = _sprite();
  sprite->setTexture(texture);
  sprite->setTextureRect(rectangle);
}

void TextureWidget::setTextureRect(const IntRect& rectangle){
  Sprite* sprite = _sprite();
  sprite->setTextureRect(rectangle);
}



/// ButtonWidget

ButtonWidget::ButtonWidget(ButtonWidgetStruct&& buttonWidgetStruct) :
  _onClick(move(buttonWidgetStruct.onClick)),
  _onFocus(move(buttonWidgetStruct.onFocus)),
  _onLoseFocus(move(buttonWidgetStruct.onLoseFocus)),
  Widget(move(buttonWidgetStruct.id))
{


  _textWidget = new TextWidget(move(buttonWidgetStruct.text));
  _textWidget->setChild(nullptr);

  _paddingWidget = new PaddingWidget(move(buttonWidgetStruct.textPadding));
  _paddingWidget->setChild(_textWidget);

  _textureWidget = new TextureWidget(move(buttonWidgetStruct.texture));
  _textureWidget->setChild(_paddingWidget);


  _widget = new EventWidget({
    .listeners = {
      {
        "MouseTap",
        EventProcessor::onMouseTap(
          [this](Mouse::Button button, const Vector2f& pos){
            this->_isTap = true;
            this->setFouse(true);
            // _textureWidget->setTexture(this->_focusTexture);
          }
        )
      },
      {
        "MouseRelease",
        EventProcessor::onMouseRelease(
          [this](Mouse::Button button, const Vector2f& pos, bool isInWidget){
            this->_isTap = false;
            this->setFouse(false);
            // _textureWidget->setTexture(this->_texture);
            if(this->_onClick && isInWidget) this->_onClick(button);
          }
        )
      },
      {
        "MouseEnter",
        EventProcessor::onMouseEnter(
          false,
          [this](){
            this->setFouse(true);
            // _textureWidget->setTexture(this->_focusTexture);
          }
        )
      },
      {
        "MouseLeave",
        EventProcessor::onMouseLeave(
          true,
          [this](){
            this->setFouse(this->_isTap);
            // _textureWidget->setTexture(this->_isTap ? this->_focusTexture : this->_texture);
          }
        )
      }
    },
    .child = _textureWidget
  });
}

ButtonWidget::~ButtonWidget(){
  delete _widget;
}

void ButtonWidget::setFouse(bool isFocus){
  if(_isFocus == isFocus) return;

  _isFocus = isFocus;
  if(_isFocus){
    if(_onFocus) _onFocus();
  }else{
    if(_onLoseFocus) _onLoseFocus();
  }
}

void ButtonWidget::render(RenderWindow* window, float left, float top, float width, float height){
  _widget->render(window, left, top, width, height);
}