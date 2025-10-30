#include "include/ui.hpp"


Widget* ui::startMenuUi;

Widget* ui::gameMapMenuUi;

Widget* ui::settingsMenuUi;

Widget* ui::pauseMenuUi;

Widget* ui::playHUD;

void ui::initUi(){
  startMenuUi = new ColumnWidget({
    .flex = {1, 1},
    .children = {
      new ConstraintWidget({
        .width = 12.f,
        .child = new StackWidget({
          .children = {
            new TextWidget({
              .text = "Dungeon",
              .font = State::titleFont,
              .size = 200,
              .color = State::titleColor
            }),
            new TextWidget({
              .text = "Dungeon",
              .font = State::titleShadowFont,
              .size = 200,
              .color = State::titleShadowColor
            })
          }
        })
      }),
      new ConstraintWidget({
        .width = 10.f,
        .height = 9.f,
        .child = new AspectRadioWidget({
          .aspectRadio = 10.f / 9.f,
          .child = new ColumnWidget({
            .flex = {1, 1, 1},
            .children = {
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "startMenuPlayButton",
                  .texture = State::btnTexture,
                  .focusTexture = State::btnFocusTexture,
                  .text = State::tr("play"),
                  .onClick = [](Mouse::Button button){
                    State::state = StateType::gameMapMenu;
                  }
                })
              }),
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "startMenuSettingsButton",
                  .texture = State::btnTexture,
                  .focusTexture = State::btnFocusTexture,
                  .text = State::tr("setting"),
                  .onClick = [](Mouse::Button button){
                    State::state = StateType::settingsMenu;
                  }
                })
              }),
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "startMenuExitButton",
                  .texture = State::btnTexture,
                  .focusTexture = State::btnFocusTexture,
                  .text = State::tr("exit"),
                  .onClick = [](Mouse::Button button){
                    State::close();
                  }
                })
              })
            }
          })
        })
      })
    }
  });



  /// gameMapMenuUi

  gameMapMenuUi = new RowWidget({
    .flex = {1, 1, 1},
    .children = {
      nullptr,
      new ConstraintWidget({
        .width = 10.f,
        .height = 9.f,
        .child = new AspectRadioWidget({
          .aspectRadio = 10.f / 9.f,
          .child = new ColumnWidget({
            .flex = {1, 1, 1},
            .children = {
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "gameMapMenuContinueButton",
                  .text = State::tr("continue"),
                  .onClick = [](Mouse::Button){
                    State::state = StateType::play;
                    // State::fps = State::fps % 120 + 30;
                    // State::window->setFramerateLimit(State::fps);

                    // Widget* widget = Widget::find("settingsMenuFPSButtonText");
                    // TextWidget* text = static_cast<TextWidget*>(widget);
                    // text->setText("FPS: " + to_string(State::fps));
                  }
                })
              }),
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "gameMapMenuCreateButton",
                  .text = State::tr("create"),
                  .onClick = [](Mouse::Button){
                    Game::generate(
                      State::floorTexture,
                      State::playerBigTexture,
                      State::treasureBigTexture,
                      [](vector<vector<int>> map, Texture mapTexture, Entity* player, vector<Entity*> treasure){
                        State::map = move(map);
                        State::mapTexture = move(mapTexture);
                        State::player = move(player);
                        State::treasure = move(treasure);
                      }
                    );
                    State::state = StateType::play;
                  }
                })
              }),
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "gameMapMenuBackButton",
                  .text = State::tr("back"),
                  .onClick = [](Mouse::Button){
                    State::state = StateType::startMenu;
                  }
                })
              })
            }
          })
        })
      }),
      nullptr
    }
  });


  /// settingsMenuUi

  settingsMenuUi = new RowWidget({
    .flex = {1, 1, 1},
    .children = {
      nullptr,
      new ConstraintWidget({
        .width = 10.f,
        .height = 9.f,
        .child = new AspectRadioWidget({
          .aspectRadio = 10.f / 9.f,
          .child = new ColumnWidget({
            .flex = {1, 1, 1},
            .children = {
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "settingsMenuFPSButton",
                  .text = "FPS: " + to_string(State::fps),
                  .onClick = [](Mouse::Button){
                    State::fps = State::fps % 120 + 30;
                    State::window->setFramerateLimit(State::fps);

                    Widget* widget = Widget::find("settingsMenuFPSButtonText");
                    TextWidget* text = static_cast<TextWidget*>(widget);
                    text->setText("FPS: " + to_string(State::fps));
                  }
                })
              }),
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "settingsMenuLanguageButton",
                  .text = State::tr("language"),
                  .onClick = [](Mouse::Button){
                    if(State::lang == "zh"){
                      State::lang = "en";
                    }else{
                      State::lang = "zh";
                    }
                  }
                })
              }),
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "settingsMenuFinishButton",
                  .text = State::tr("finish"),
                  .onClick = [](Mouse::Button){
                    State::state = StateType::startMenu;
                  }
                })
              })
            }
          })
        })
      }),
      nullptr
    }
  });



  /// pauseMenuUi

  pauseMenuUi = new RowWidget({
    .flex = {1, 1, 1},
    .children = {
      nullptr,
      new ConstraintWidget({
        .width = 10.f,
        .height = 9.f,
        .child = new AspectRadioWidget({
          .aspectRadio = 10.f / 9.f,
          .child = new ColumnWidget({
            .flex = {1, 1, 1},
            .children = {
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "pauseMenuContinueButton",
                  .text = State::tr("continue"),
                  .onClick = [](Mouse::Button){
                    State::state = StateType::play;
                  }
                })
              }),
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "pauseMenuSettingsButton",
                  .text = State::tr("setting"),
                  .onClick = [](Mouse::Button){
                    State::state = StateType::settingsMenu;
                  }
                })
              }),
              new PaddingWidget({
                .left = 0.2f,
                .top = 0.2f,
                .right = 0.2f,
                .bottom = 0.2f,
                .child = new packageWidget::DefaultButton({
                  .id = "pauseMenuMenuButton",
                  .text = State::tr("menu"),
                  .onClick = [](Mouse::Button){
                    State::state = StateType::startMenu;
                  }
                })
              })
            }
          })
        })
      }),
      nullptr
    }
  });

  playHUD = new ConstraintWidget({
    .width = 6.f,
    .height = 0.6f,
    .alignment = Alignment::centerTop,
    .child = new RowWidget({
      .flex = {1, 1},
      .children = {
        new AspectRadioWidget({
          .aspectRadio = 1,
          .child = new TextureWidget({
            .texture = State::treasureBigTexture,
            .rectangle = Rect<int>{{0, 0}, {72, 72}}
          })
        }),
        new TextWidget({
          .id = "scoreboard",
          .text = "score",
          .font = State::font,
          .color = Color::White
        })
      }
    })
  });

}