#pragma once

#include <iostream>
#include <optional>
#include <functional>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

enum Direction{
  left,
  up,
  right,
  bottom
};

struct EntityStruct{
  float health = -1;
  float moveSpeed = 0;  // (d/s)
  Direction direction = Direction::bottom;
  Vector2f pos = {0, 0};
  Texture& bigTexture;  // left up right bottom
  bool isSingleTextureDirection = false;
  uint8_t frame = 1;
  float spf = 0;
};

class Entity{
  protected:
    float _health;
    float _moveSpeed;
    Direction _direction;
    uint8_t _frame;
    float _spf;
    Vector2f _pos;
    bool _isSingleTextureDirection;
    Vector2i _textureSize;
    Sprite* _sprite;

    float _timer = 0;
    int _currentFrame = 0;

  public:
    Entity(EntityStruct&& entityStruct);

    ~Entity();

    Entity(const Entity&);

    Entity& operator=(const Entity&);

    Entity(Entity&& rhs);

    Vector2f getPosition();

    void setPosition(Vector2f& pos);

    void setDirection(Direction direction);

    void step(float dt, bool isMove = false);

    void draw(RenderWindow* window);

    static bool isCollision(Entity* entity1, Entity* entity2);
};


class DisposableEntity : public Entity{
  protected:
    function<void()> _onDispose;

    float _runtime = 0;
  public:
    bool isDispose = false;

    DisposableEntity(EntityStruct&& entityStruct, function<void()> onDispose);

    void step(float dt, bool isMove = false);

    void draw(RenderWindow* window);
};