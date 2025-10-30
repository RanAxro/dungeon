#include "core/include/state.hpp"
#include "core/include/widget.hpp"
#include "core/include/packageWidget.hpp"
#include "core/include/ui.hpp"
#include "core/include/generator.hpp"
#include "core/include/game.hpp"


#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

int main(){

  State::initState();
  ui::initUi();


  Game::generate(
    State::floorTexture,
    State::playerBigTexture,
    State::treasureBigTexture,
    [](vector<vector<int>> map, Texture mapTexture, Entity* player, vector<Entity*> treasure){
      State::map = move(map);
      State::mapTexture = move(mapTexture);
      State::player = move(player);
      State::treasure = move(treasure);
      State::bomb = {};
      State::score = 0;
    }
  );

  for(auto &row : *State::map){
    for(int v : row){
      if(v== 0) std::cout<<"0";
      else if(v== 1) std::cout<<".";
      else if(v==10) std::cout<<"S";
      else if(v==11) std::cout<<"E";
      else if(v==20) std::cout<<"T";
      else if(v==30) std::cout<<"B";
    }
    std::cout<<"\n";
  }




  Texture bg("../assets/textures/bg.png");
  bg.setRepeated(true);
  Sprite bgSpr(bg);
  bgSpr.setTextureRect({{0, 0}, {static_cast<int>(State::windowSize_px.x), static_cast<int>(State::windowSize_px.y)}});
  bgSpr.setScale({3 * State::scale, 3 * State::scale});

  Clock clock; 
  while(State::window->isOpen()){
    float dt = clock.restart().asSeconds();


    EventWidget::clear();
    while(const optional<Event> event = State::window->pollEvent()){
      EventWidget::putEvent(State::window, *event);
      if(event->is<Event::Closed>()){
        State::close();
      }

      if(auto e = event->getIf<Event::KeyPressed>()){
        if(e->code == Keyboard::Key::Space){
          State::bomb.emplace_back(Game::generateBomb(
            State::bombBigTexture,
            (*State::player).getPosition(),
            [](){}
          ));
        }

        switch(e->code){
          case Keyboard::Key::Space:
            State::bomb.emplace_back(Game::generateBomb(
              State::bombBigTexture,
              (*State::player).getPosition(),
              [](){}
            ));
            break;
          case Keyboard::Key::Escape:
            if(State::state == StateType::play){
              State::state = StateType::pauseMenu;
            }else if(State::state == StateType::pauseMenu){
              State::state = StateType::play;
            }
            break;
        }
      }
    }

    // Escape key to close the window
    if(Keyboard::isKeyPressed(Keyboard::Key::F)) State::close();





    State::window->clear();

    
    // const View& view = State::window->getView();
    // float width = view.getSize().x;
    // float height = view.getSize().y;
    // float left = view.getCenter().x - 0.5f * width;
    // float top = view.getCenter().y - 0.5f * height;

    switch(State::state){
      case StateType::play:
        Game::updatePlayer(State::player, *State::map, dt);
        Game::followPlayer(State::window, State::player);
        Game::updateTreasure(State::treasure, dt);

        Game::drawMap(State::window, *State::mapTexture, {static_cast<float>((*State::map)[0].size()), static_cast<float>((*State::map).size())});

        if(State::map) Game::drawDoor(State::window, State::doorTexture, *State::map);

        ///////////////////////////
        
        for(Entity* entity : State::treasure){
          if(Entity::isCollision(entity, State::player)){
            State::score++;
            auto it = find(State::treasure.begin(), State::treasure.end(), entity);
            if(it != State::treasure.end()){
              State::treasure.erase(it);
            }
          }else{
            entity->draw(State::window);
          }
        }
        static_cast<TextWidget*>(Widget::find("scoreboard"))->setText("score:" + to_string(State::score));

        //////////////////////////
        for(auto bomb : State::bomb){
          if(bomb && !bomb->isDispose){
            bomb->step(dt, true);
            bomb->draw(State::window);
          }
        }

        if(State::player) State::player->draw(State::window);


        break;
      case StateType::startMenu:
        State::window->setView(View(Rect<float>({0.f, 0.f}, {State::windowSize.x, State::windowSize.y})));
        ui::startMenuUi->render(State::window, 0, 0, State::windowSize.x, State::windowSize.y);
        break;
      case StateType::gameMapMenu:
        State::window->setView(View(Rect<float>({0.f, 0.f}, {State::windowSize.x, State::windowSize.y})));
        ui::gameMapMenuUi->render(State::window, 0, 0, State::windowSize.x, State::windowSize.y);
        break;
      case StateType::settingsMenu:
        State::window->setView(View(Rect<float>({0.f, 0.f}, {State::windowSize.x, State::windowSize.y})));
        ui::settingsMenuUi->render(State::window, 0, 0, State::windowSize.x, State::windowSize.y);
        break;
      case StateType::pauseMenu:
        State::window->setView(View(Rect<float>({0.f, 0.f}, {State::windowSize.x, State::windowSize.y})));
        ui::pauseMenuUi->render(State::window, 0, 0, State::windowSize.x, State::windowSize.y);
        break;
    }

        State::window->setView(View(Rect<float>({0.f, 0.f}, {State::windowSize.x, State::windowSize.y})));
    ui::playHUD->render(State::window, 0, 0, State::windowSize.x, State::windowSize.y);

    State::window->display();
  }
}