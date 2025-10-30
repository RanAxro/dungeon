#pragma once

#include "generator.hpp"
#include "entity.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

namespace Game{

  // draw

  extern void drawMap(RenderWindow* window, Texture& floorTexture, const vector<vector<int>>& map);

  extern void drawMap(RenderWindow* window, Texture& mapTexture, const Vector2f& mapSize);

  extern void drawDoor(RenderWindow* window, Texture& doorTexture, const vector<vector<int>>& map);

  // generate

  extern Texture generateMapTexture(const vector<vector<int>>& map, Texture& floorTexture);

  extern Entity* generatePlayer(Texture& playerBigTexture, const vector<vector<int>>& map);

  extern vector<Entity*> generateTreasure(Texture& treasureBigTexture, const vector<vector<int>>& map);

  extern DisposableEntity* generateBomb(Texture& bombBigTexture, Vector2f pos, function<void()> onDispose);

  extern Entity* generateBoss(Texture& bossBigTexture, Vector2f pos);

  extern void generate(Texture& floorTexture, Texture& playerBigTexture, Texture& treasureBigTexture, function<void(vector<vector<int>>, Texture, Entity*, vector<Entity*>)> cb);

  // update

  extern void updatePlayer(Entity* player, const vector<vector<int>>& map, float dt);

  extern void updateTreasure(vector<Entity*>& treasure, float dt);

  // set view

  extern void followPlayer(RenderWindow* window, Entity* player);
}