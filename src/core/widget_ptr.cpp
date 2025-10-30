#include "include/widget.hpp"


unordered_map<string, Widget*>& Widget::_registry(){
  static unordered_map<string, Widget*> m;
  return m;
}

Widget::Widget(string id) : _id(move(id)){
  auto [it, inserted] = _registry().emplace(_id, this);
  // if(!inserted) throw runtime_error("duplicate id");
}

Widget::~Widget(){
  _registry().erase(_id);
}

Widget::Widget(Widget&& rhs) noexcept : _id(move(rhs._id)){
  // rhs._id.clear();
  _registry()[_id] = this;
}

void Widget::draw(float left, float top, float width, float height){

}

Widget* Widget::find(const string& id){
  auto it = _registry().find(id);
  return it == _registry().end() ? nullptr : it->second;
}




LayoutWidget::LayoutWidget(vector<uint8_t> flex, vector<std::unique_ptr<Widget>>&& children) :
  _flex(flex),
  _children(move(children)){

}

LayoutWidget::LayoutWidget(const string id, vector<uint8_t> flex, vector<std::unique_ptr<Widget>>&& children) :
  _flex(flex),
  _children(move(children)),
  Widget(id){

}



RowWidget::RowWidget(vector<uint8_t> flex, vector<std::unique_ptr<Widget>>&& children) :
  LayoutWidget(flex, move(children)){

}

RowWidget::RowWidget(const string id, vector<uint8_t> flex, vector<std::unique_ptr<Widget>>&& children) :
  LayoutWidget(id, flex, move(children)){

}

void RowWidget::draw(float left, float top, float width, float height){
  uint64_t sumFlex = accumulate(_flex.begin(), _flex.end(), 0.f);
  uint16_t flexLength = _flex.size();
  float unit = 1 / static_cast<float>(sumFlex);

  float counter = 0;
  for(uint16_t i = 0; i < _children.size(); i++){
    if(_children[i] == nullptr) continue;

    float proportion = i > flexLength ? unit : static_cast<float>(_flex[i]) * unit;
    _children[i]->draw(left + counter * width, top, proportion * width, height);
    counter += proportion;
  }
}



ColumnWidget::ColumnWidget(vector<uint8_t> flex, vector<std::unique_ptr<Widget>>&& children) :
  LayoutWidget(flex, move(children)){

}

ColumnWidget::ColumnWidget(const string id, vector<uint8_t> flex, vector<std::unique_ptr<Widget>>&& children) :
  LayoutWidget(id, flex, move(children)){

}

void ColumnWidget::draw(float left, float top, float width, float height){
  uint64_t sumFlex = accumulate(_flex.begin(), _flex.end(), 0.f);
  uint16_t flexLength = _flex.size();
  float unit = 1 / static_cast<float>(sumFlex);

  float counter = 0;
  for(uint16_t i = 0; i < _children.size(); i++){
    if(_children[i] == nullptr) continue;

    float proportion = i > flexLength ? unit : static_cast<float>(_flex[i]) * unit;
    _children[i]->draw(left, top + counter * height, width, proportion * height);
    counter += proportion;
  }
}


ConstraintWidget::ConstraintWidget(float width, float height, unique_ptr<Widget>&& child) :
  _width(width),
  _height(height),
  _child(move(child)){

}

ConstraintWidget::ConstraintWidget(const string id, float width, float height, unique_ptr<Widget>&& child) :
  _width(width),
  _height(height),
  _child(move(child)),
  Widget(id){

}

void ConstraintWidget::draw(float left, float top, float width, float height){
  if(_child == nullptr) return;

  float drawWidth = width;
  float drawHeight = height;

  if(_width >= 0 && _width < width) drawWidth = _width;
  if(_height >= 0 && _height < height)  drawHeight = _height;

  _child->draw(left, top, drawWidth, drawHeight);
}


RenderWidget::RenderWidget(unique_ptr<Drawable> drawable, unique_ptr<Widget>&& child) :
  _drawable(move(drawable)),
  _child(move(child)){

}

RenderWidget::RenderWidget(const string id, unique_ptr<Drawable> drawable, unique_ptr<Widget>&& child) :
  _drawable(move(drawable)),
  _child(move(child)),
  Widget(id){

}

void RenderWidget::draw(float left, float top, float width, float height){
  State::window->draw(*_drawable);
}



ColorWidget::ColorWidget(Color color, unique_ptr<Widget>&& child) :
  RenderWidget(
    make_unique<RectangleShape>(),
    move(child)
  )
{
  _shape()->setFillColor(color);
}

void ColorWidget::draw(float left, float top, float width, float height){
  _shape()->setPosition({left, top});
  _shape()->setSize({width, height});
  RenderWidget::draw(left, top, width, height);
  cout << "draw " << left << "\t" << top << "\t" << width << "\t" << height << endl;
}

RectangleShape* ColorWidget::_shape(){
  return static_cast<RectangleShape*>(_drawable.get());
}