#include "include/state.hpp"

StateType State::state;

Font State::font("../assets/fonts/defaultFont.ttf");
Font State::titleFont("../assets/fonts/titleFont.ttf");
Font State::titleShadowFont("../assets/fonts/titleShadowFont.ttf");

Color State::titleColor(192, 148, 115);
Color State::titleShadowColor(122, 72, 65);

Texture State::btnTexture("../assets/textures/btn.png");
Texture State::btnFocusTexture("../assets/textures/btn_focus.png");
Texture State::floorTexture("../assets/textures/floor.png");
Texture State::playerBigTexture("../assets/textures/player.png");
Texture State::doorTexture("../assets/textures/door.png");
Texture State::treasureBigTexture("../assets/textures/treasure.png");
Texture State::bombBigTexture("../assets/textures/bomb.png");

nlohmann::json State::enLang;
nlohmann::json State::zhLang;

string State::lang = "en";

RenderWindow* State::window = nullptr;

Vector2u State::windowSize_px;

Vector2f State::windowSize;

float State::scale = 0;

float State::pxpd = 0;


int State::fps;

optional<vector<vector<int>>> State::map;

optional<Texture> State::mapTexture;

vector<Entity*> State::treasure;

vector<DisposableEntity*> State::bomb;

Entity* State::player;

int State::score = 0;

string State::tr(const string& key){
  if(lang == "zh" && zhLang.contains(key)){
    return zhLang[key];
  }else if(lang == "en" && enLang.contains(key)){
    return enLang[key];
  }else{
    return key;
  }
}

void State::initState(){
  state = StateType::startMenu;

  window = new RenderWindow(
    VideoMode::getDesktopMode(),
    // VideoMode({700, 600}),
    "Dungeon"
    ,
    Style::None
  );

  fps = 60;

  window->setFramerateLimit(fps);

  // window->setIcon(icon);

  windowSize_px = window->getSize();

  windowSize = Vector2f(
    absoluteUnitDistance, 
    absoluteUnitDistance * static_cast<float>(windowSize_px.y) / static_cast<float>(windowSize_px.x)
  );

  scale = absoluteUnitDistance / static_cast<float>(windowSize_px.x);

  pxpd = static_cast<float>(windowSize_px.x) / absoluteUnitDistance;

  View view(Rect<float>({0.f, 0.f}, {windowSize.x, windowSize.y}));
  window->setView(view);

  // lang
  ifstream en("../assets/langs/en.json");
  ifstream zh("../assets/langs/zh.json");
  if(en && en.peek() != std::ifstream::traits_type::eof()){
    try{
      en >> enLang;
    }catch(const nlohmann::json::parse_error& e){
      cerr << "en.json parse error: " << e.what() << '\n';
    }
  }
  if(zh && zh.peek() != std::ifstream::traits_type::eof()){
    try{
      zh >> zhLang;
      cout << zhLang;
    }catch(const nlohmann::json::parse_error& e){
      cerr << "zh.json parse error: " << e.what() << '\n';
    }
  }



  readState();
}




void State::readState(){
  ifstream i("config.json");
  if(!i || i.peek() == std::ifstream::traits_type::eof()){
    return;
  }

  nlohmann::json j;
  try{
    i >> j;
  }catch(const nlohmann::json::parse_error& e){
    cerr << "JSON parse error: " << e.what() << '\n';
  }

  if(j.contains("fps")) fps = j["fps"];
}

void State::saveState(){
  nlohmann::json j;
  j["fps"] = State::fps;

  try{
    ofstream o("config.json");
    o << setw(2) << j << std::endl;
  }catch(...){
    cerr << "Failed to save to Config JSON" <<endl;
  }
}

void State::close(){
  saveState();
  window->close();
}