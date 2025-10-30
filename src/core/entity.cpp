#include "include/entity.hpp"

Entity::Entity(EntityStruct&& entityStruct) :
  _health(move(entityStruct.health)),
  _moveSpeed(move(entityStruct.moveSpeed)),
  _direction(move(entityStruct.direction)),
  _frame(move(entityStruct.frame)),
  _spf(move(entityStruct.spf)),
  _pos(move(entityStruct.pos)),
  _isSingleTextureDirection(move(entityStruct.isSingleTextureDirection))
{
  _textureSize = {
    _isSingleTextureDirection ? static_cast<int>(entityStruct.bigTexture.getSize().x) : static_cast<int>(entityStruct.bigTexture.getSize().x) / 4,
    static_cast<int>(entityStruct.bigTexture.getSize().y) / _frame,
  };
  _sprite = new Sprite(entityStruct.bigTexture, {{0, 0}, {_textureSize.x, _textureSize.y}});
  _sprite->setOrigin({0.5f * static_cast<float>(_textureSize.x), static_cast<float>(_textureSize.y)});
  _sprite->setPosition(_pos);
  _sprite->setScale({1.f / _textureSize.x, 1.f / _textureSize.y});
}

Entity::~Entity(){
  delete _sprite;
}

Vector2f Entity::getPosition(){
  return _pos;
}


void Entity::setPosition(Vector2f& pos){
  _pos = pos;
};


void Entity::setDirection(Direction direction){
  _direction = direction;
}


void Entity::step(float dt, bool isMove){
  _timer += dt;
  if(_timer > _spf){
    _timer = 0;
    _currentFrame = (_currentFrame + 1) % _frame;
  };


  // move

  float d = _moveSpeed * dt;

  if(!isMove){
    _timer = 0;
    _currentFrame = 0;
    d = 0;
  }

  int offsetX = 0;
  switch(_direction){
    case Direction::left:
      _pos += {- d, 0};
      offsetX = 0 * _textureSize.x;
      break;
    case Direction::up:
      _pos += {0, - d};
      offsetX = 1 * _textureSize.x;
      break;
    case Direction::right:
      _pos += {d, 0};
      offsetX = 2 * _textureSize.x;
      break;
    case Direction::bottom:
      _pos += {0, d};
      offsetX = 3 * _textureSize.x;
      break;
  }
  if(_isSingleTextureDirection) offsetX = 0;
  _sprite->setTextureRect({{offsetX, _currentFrame * _textureSize.y}, _textureSize});

  _sprite->setPosition(_pos);
}


void Entity::draw(RenderWindow* window){
  window->draw(*_sprite);
};


bool Entity::isCollision(Entity* entity1, Entity* entity2){
  Vector2f pos1 = entity1->getPosition();
  Vector2f pos2 = entity2->getPosition();

  if((pos1 - pos2).lengthSquared() < 0.4){
    return true;
  }
  return false;
}






DisposableEntity::DisposableEntity(EntityStruct&& entityStruct, function<void()> onDispose) :
  _onDispose(onDispose),
  Entity(move(entityStruct)){

};

void DisposableEntity::step(float dt, bool isMove){
  _runtime += dt;
  if(_runtime > _frame * _spf){
    isDispose = true;
    _onDispose();
  }else{
    Entity::step(dt, isMove);
  }
}

void DisposableEntity::draw(RenderWindow* window){
  if(!isDispose){
    Entity::draw(window);
  }
};