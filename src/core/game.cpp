#include "include/game.hpp"

namespace Game{

  void drawMap(RenderWindow* window, Texture& floorTexture, const vector<vector<int>>& map){
    Sprite sprite = Sprite(floorTexture);
    Vector2u size = floorTexture.getSize();
    sprite.setScale({1.f / size.x, 1.f / size.y});

    for(float y = 0; y < map.size(); y++){
      for(float x = 0; x < map[y].size(); x++){
        if(map[y][x]){
          // TODO 看不见的不画
          // const View& view = window->getView();

          sprite.setPosition({x, y});
          window->draw(sprite);
        }
      }
    }
  }


  void drawMap(RenderWindow* window, Texture& mapTexture, const Vector2f& mapSize){
    Sprite mapSprite(mapTexture);
    Vector2u mapTextureSize = mapTexture.getSize();
    mapSprite.setScale({mapSize.x / static_cast<float>(mapTextureSize.x), mapSize.y / static_cast<float>(mapTextureSize.y)});

    window->draw(mapSprite);
  }

  
  void drawDoor(RenderWindow* window, Texture& doorTexture, const vector<vector<int>>& map){
    Sprite sprite = Sprite(doorTexture);
    Vector2u size = doorTexture.getSize();
    sprite.setOrigin({0.5f * size.x, 0.5f * size.y});
    sprite.setScale({2.f / size.x, 2.f / size.y});

    for(float y = 0; y < map.size(); y++){
      for(float x = 0; x < map[y].size(); x++){
        if(map[y][x] == Tile::tExit){
          sprite.setPosition({x, y});
          window->draw(sprite);
        }
      }
    }
  }



  Texture generateMapTexture(const vector<vector<int>>& map, Texture& floorTexture){
    Sprite sprite = Sprite(floorTexture);
    Vector2u size = floorTexture.getSize();

    RenderTexture canvas({size.x * static_cast<unsigned int>(map[0].size()), size.y * static_cast<unsigned int>(map.size())});
    canvas.clear(Color::Black);

    for(float y = 0; y < map.size(); y++){
      for(float x = 0; x < map[y].size(); x++){
        if(map[y][x]){
          sprite.setPosition({x * size.x, y * size.y});
          canvas.draw(sprite);
        }
      }
    }

    canvas.display(); 
    return canvas.getTexture();
  }


  Entity* generatePlayer(Texture& playerBigTexture, const vector<vector<int>>& map){
    Vector2f pos;
    for(float y = 0; y < map.size(); y++){
      for(float x = 0; x < map[y].size(); x++){
        if(map[y][x] == Tile::tBirth){
          pos = {x, y};
          break;
        }
      }
    }

    return new Entity({
      .health = 10,
      .moveSpeed = 3,
      .pos = pos,
      .bigTexture = playerBigTexture,
      .frame = 3,
      .spf = 0.1
    });
  }

  vector<Entity*> generateTreasure(Texture& treasureBigTexture, const vector<vector<int>>& map){
    vector<Entity*> entities;

    for(float y = 0; y < map.size(); y++){
      for(float x = 0; x < map[y].size(); x++){
        if(map[y][x] == Tile::tTreasure){
          entities.emplace_back(new Entity({
            .pos = {x, y},
            .bigTexture = treasureBigTexture,
            .isSingleTextureDirection = true,
            .frame = 7,
            .spf = 0.1
          }));
          break;
        }
      }
    }

    return entities;
  }

  DisposableEntity* generateBomb(Texture& bombBigTexture, Vector2f pos, function<void()> onDispose){
    return new DisposableEntity({
      .pos = pos,
      .bigTexture = bombBigTexture,
      .isSingleTextureDirection = true,
      .frame = 23,
      .spf = 0.1
    },
    onDispose
    );
  }


  
  void generate(Texture& floorTexture, Texture& playerBigTexture, Texture& treasureBigTexture, function<void(vector<vector<int>>, Texture, Entity*, vector<Entity*>)> cb){
    vector<vector<int>> map = generateMap(50, 50);
    Texture mapTexture = Game::generateMapTexture(map, floorTexture);
    Entity* player = Game::generatePlayer(playerBigTexture, map);
    vector<Entity*> treasure = Game::generateTreasure(treasureBigTexture, map);
    cb(
      move(map),
      move(mapTexture),
      move(player),
      move(treasure)
    );
  }


  void followPlayer(RenderWindow* window, Entity* player){
    if(player){
      const View& view = window->getView();
      window->setView({player->getPosition(), view.getSize()});
    }
  }



  void updatePlayer(Entity* player, const vector<vector<int>>& map, float dt){
    if(player == nullptr) return;

    Vector2f oriPos = player->getPosition();

    if(Keyboard::isKeyPressed(Keyboard::Key::A)){
      player->setDirection(Direction::left);
      player->step(dt, true);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::W)){
      player->setDirection(Direction::up);
      player->step(dt, true);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::D)){
      player->setDirection(Direction::right);
      player->step(dt, true);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Key::S)){
      player->setDirection(Direction::bottom);
      player->step(dt, true);
    }else{
      player->step(dt, false);
      return;
    }


    Vector2f afterPos = player->getPosition();
    int x = static_cast<int>(afterPos.x);
    int y = static_cast<int>(afterPos.y);
    if(map[y][x] == Tile::tWall){
      player->setPosition(oriPos);
    }
  }

  
  void updateTreasure(vector<Entity*>& treasure, float dt){
    for(Entity* entity : treasure){
      entity->step(dt, true);
    }
  }

}
