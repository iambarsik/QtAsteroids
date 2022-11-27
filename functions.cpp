#include "widget.h"

void Widget::SetStartValues()   {
        // first engine settings
    Settings.bDevMode = true;
    Settings.bEnglish = true;

        // init sounds
    sound_shoot = new QSound(":/sounds/shoot.wav");
    sound_explosive = new QSound(":/sounds/explosive.wav");
    sound_damage = new QSound(":/sounds/damage.wav");
    sound_bomb = new QSound(":/sounds/bomb.wav");

        // init menu
    QVector<myMenu::Punkt> menu;
    menu.push_back(myMenu::Punkt(QString("Start game easy"),QString("Начать легкую игру"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Start game medium"),QString("Начать нормальную игру"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Start game hard"),QString("Начать сложную игру"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Start game very hard"),QString("Начать очень сложную игру"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Start game suicide"),QString("Начать самоубийственную игру"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Exit"),QString("Выход"),MenuActions::None));
    game_main_menu.Init(menu,Settings.bEnglish);

    menu.clear();
    menu.push_back(myMenu::Punkt(QString("Try again"),QString("Попробовать еще раз"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Change dificult"),QString("Изменить сложность игры"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Exit"),QString("Выход"),MenuActions::None));
    game_dead_menu.Init(menu,Settings.bEnglish);

    menu.clear();
    menu.push_back(myMenu::Punkt(QString("Continue game"),QString("Продолжить игру"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Change dificult"),QString("Изменить сложность игры"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("Exit"),QString("Выход"),MenuActions::None));
    game_pause_menu.Init(menu,Settings.bEnglish);    

    menu.clear();
    menu.push_back(myMenu::Punkt(QString("Русская версия"),QString("Русская версия"),MenuActions::None));
    menu.push_back(myMenu::Punkt(QString("English version"),QString("English version"),MenuActions::None));
    game_language_menu.Init(menu,Settings.bEnglish);

        // first game settings
    game_hp = MAX_HP;
    game_score = 0;
    game_spawn_timer = SPAWN_TIMER_HARD;
    AsteroidTimer = 0;
    game_flash = false;

    game_results[0] = 0;
    game_results[1] = 0;
    game_results[2] = 0;
    game_results[3] = 0;
    game_results[4] = 0;
    loadresults();

    game_screen = GameScreens::Logo;
    game_play = false;

    setButton(Qt::Key_W,false);
    setButton(Qt::Key_D,false);
    setButton(Qt::Key_S,false);
    setButton(Qt::Key_A,false);
    setButton(Qt::Key_Escape,false);
    setButton(Qt::Key_U,false);
    setButton(Qt::Key_J,true);
}

bool Widget::event(QEvent *event)   {
    switch (event->type())  {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:  {
            QTouchEvent* pte = static_cast<QTouchEvent*>(event);
            listKeys = pte->touchPoints();
            update();
        } break;
        default: return QWidget::event(event);
    }
    return true;
}

void Widget::OnKeys(int Key, bool press)    {
    switch(Key) {
        case Qt::Key_Up:
        case Qt::Key_W: {
            myKey[BUTTON_UP] = press;
            if(press && game_screen == GameScreens::StartMenu) game_main_menu.goUp();
            if(press && game_screen == GameScreens::PauseMenu) game_pause_menu.goUp();
            if(press && game_screen == GameScreens::DeadMenu) game_dead_menu.goUp();
            if(press && game_screen == GameScreens::Language) game_language_menu.goUp();
        } break;
        case Qt::Key_Right:
        case Qt::Key_D: {
            myKey[BUTTON_RIGHT] = press;
        } break;
        case Qt::Key_Down:
        case Qt::Key_S: {
            myKey[BUTTON_DOWN] = press;
            if(press && game_screen == GameScreens::StartMenu) game_main_menu.goDown();
            if(press && game_screen == GameScreens::PauseMenu) game_pause_menu.goDown();
            if(press && game_screen == GameScreens::DeadMenu) game_dead_menu.goDown();
            if(press && game_screen == GameScreens::Language) game_language_menu.goDown();
        } break;
        case Qt::Key_Left:
        case Qt::Key_A: {
            myKey[BUTTON_LEFT] = press;
        } break;
        case Qt::Key_Space: {

        } break;
        case Qt::Key_Return:
        case Qt::Key_J: {
            myKey[BUTTON_ACT1] = press;
            if(press)   {
                switch(game_screen) {
                    case GameScreens::Logo:
                        game_screen = GameScreens::Language;
                        setButton(Qt::Key_W,true);
                        setButton(Qt::Key_D,false);
                        setButton(Qt::Key_S,true);
                        setButton(Qt::Key_A,false);
                        setButton(Qt::Key_Escape,false);
                        setButton(Qt::Key_U,false);
                        setButton(Qt::Key_J,true);
                    break;
                    case GameScreens::Language:
                        switch(game_language_menu.Position) {
                            case 0:
                                Settings.bEnglish = false;
                            break;
                            case 1:
                                Settings.bEnglish = true;
                            break;
                        }
                        game_main_menu.SetLanguage(Settings.bEnglish);
                        game_dead_menu.SetLanguage(Settings.bEnglish);
                        game_pause_menu.SetLanguage(Settings.bEnglish);
                        game_screen = GameScreens::StartMenu;
                        setButton(Qt::Key_W,true);
                        setButton(Qt::Key_D,false);
                        setButton(Qt::Key_S,true);
                        setButton(Qt::Key_A,false);
                        setButton(Qt::Key_Escape,false);
                        setButton(Qt::Key_U,false);
                        setButton(Qt::Key_J,true);
                    break;
                    case GameScreens::StartMenu: {
                        switch(game_main_menu.Position) {
                            case 0:
                                game_spawn_timer = SPAWN_TIMER_EASY;
                            break;
                            case 1:
                                game_spawn_timer = SPAWN_TIMER_MEDIUM;
                            break;
                            case 2:
                                game_spawn_timer = SPAWN_TIMER_HARD;
                            break;
                            case 3:
                                game_spawn_timer = SPAWN_TIMER_VERY;
                            break;
                            case 4:
                                game_spawn_timer = SPAWN_TIMER_SUICIDE;
                            break;
                            case 5:
                                exit(0);
                        }                        
                        setButton(Qt::Key_W,true);
                        setButton(Qt::Key_D,true);
                        setButton(Qt::Key_S,true);
                        setButton(Qt::Key_A,true);
                        setButton(Qt::Key_Escape,true);
                        setButton(Qt::Key_U,true);
                        setButton(Qt::Key_J,false);
                        game_score = 0;
                        game_hp = MAX_HP;
                        game_object.clear();
                        game_bullet.clear();
                        AsteroidTimer = 0;

                        QVector<QPoint> buff;
                        buff.push_back(QPoint(0*TILE_SIZE, -1.6*TILE_SIZE     ));
                        buff.push_back(QPoint(0.34*TILE_SIZE, -0.17*TILE_SIZE ));
                        buff.push_back(QPoint(0.82*TILE_SIZE, -0.13*TILE_SIZE ));
                        buff.push_back(QPoint(0.9*TILE_SIZE, -0.47*TILE_SIZE  ));
                        buff.push_back(QPoint(0.98*TILE_SIZE, -0.47*TILE_SIZE ));
                        buff.push_back(QPoint(1.3*TILE_SIZE, 0.53*TILE_SIZE   ));
                        buff.push_back(QPoint(1*TILE_SIZE, 0.98*TILE_SIZE     ));
                        buff.push_back(QPoint(0.82*TILE_SIZE, 0.2*TILE_SIZE   ));
                        buff.push_back(QPoint(0.34*TILE_SIZE, 0.29*TILE_SIZE  ));
                        buff.push_back(QPoint(0.28*TILE_SIZE, 0.55*TILE_SIZE  ));
                        buff.push_back(QPoint(-0.28*TILE_SIZE, 0.53*TILE_SIZE ));
                        buff.push_back(QPoint(-0.28*TILE_SIZE, 0.34*TILE_SIZE ));
                        buff.push_back(QPoint(-0.72*TILE_SIZE, 0.19*TILE_SIZE ));
                        buff.push_back(QPoint(-1*TILE_SIZE, 0.98*TILE_SIZE    ));
                        buff.push_back(QPoint(-1.28*TILE_SIZE, 0.51*TILE_SIZE ));
                        buff.push_back(QPoint(-1.01*TILE_SIZE, -0.46*TILE_SIZE));
                        buff.push_back(QPoint(-0.89*TILE_SIZE, -0.46*TILE_SIZE));
                        buff.push_back(QPoint(-0.83*TILE_SIZE, -0.2*TILE_SIZE ));
                        buff.push_back(QPoint(-0.36*TILE_SIZE, -0.26*TILE_SIZE));
                        Convex game_ship(QPoint(Resolution.x()/2, Resolution.y()/2), buff, Qt::yellow);
                        game_ship.setType(ConvexType::Player);
                        game_ship.setGameField(QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE));

                        game_object.push_back(game_ship);

                        game_screen = GameScreens::Game;
                        game_play = true;
                    } break;
                    case GameScreens::DeadMenu: 
                        switch(game_dead_menu.Position) {
                            case 0: {
                                setButton(Qt::Key_W,true);
                                setButton(Qt::Key_D,true);
                                setButton(Qt::Key_S,true);
                                setButton(Qt::Key_A,true);
                                setButton(Qt::Key_Escape,true);
                                setButton(Qt::Key_U,true);
                                setButton(Qt::Key_J,false);
                                game_score = 0;
                                game_hp = MAX_HP;
                                game_object.clear();
                                game_bullet.clear();
                                AsteroidTimer = 0;
                                QVector<QPoint> buff;
                                buff.push_back(QPoint(0*TILE_SIZE, -1.6*TILE_SIZE     ));
                                buff.push_back(QPoint(0.34*TILE_SIZE, -0.17*TILE_SIZE ));
                                buff.push_back(QPoint(0.82*TILE_SIZE, -0.13*TILE_SIZE ));
                                buff.push_back(QPoint(0.9*TILE_SIZE, -0.47*TILE_SIZE  ));
                                buff.push_back(QPoint(0.98*TILE_SIZE, -0.47*TILE_SIZE ));
                                buff.push_back(QPoint(1.3*TILE_SIZE, 0.53*TILE_SIZE   ));
                                buff.push_back(QPoint(1*TILE_SIZE, 0.98*TILE_SIZE     ));
                                buff.push_back(QPoint(0.82*TILE_SIZE, 0.2*TILE_SIZE   ));
                                buff.push_back(QPoint(0.34*TILE_SIZE, 0.29*TILE_SIZE  ));
                                buff.push_back(QPoint(0.28*TILE_SIZE, 0.55*TILE_SIZE  ));
                                buff.push_back(QPoint(-0.28*TILE_SIZE, 0.53*TILE_SIZE ));
                                buff.push_back(QPoint(-0.28*TILE_SIZE, 0.34*TILE_SIZE ));
                                buff.push_back(QPoint(-0.72*TILE_SIZE, 0.19*TILE_SIZE ));
                                buff.push_back(QPoint(-1*TILE_SIZE, 0.98*TILE_SIZE    ));
                                buff.push_back(QPoint(-1.28*TILE_SIZE, 0.51*TILE_SIZE ));
                                buff.push_back(QPoint(-1.01*TILE_SIZE, -0.46*TILE_SIZE));
                                buff.push_back(QPoint(-0.89*TILE_SIZE, -0.46*TILE_SIZE));
                                buff.push_back(QPoint(-0.83*TILE_SIZE, -0.2*TILE_SIZE ));
                                buff.push_back(QPoint(-0.36*TILE_SIZE, -0.26*TILE_SIZE));
                                Convex game_ship(QPoint(Resolution.x()/2, Resolution.y()/2), buff, Qt::yellow);
                                game_ship.setType(ConvexType::Player);
                                game_ship.setGameField(QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE));

                                game_object.push_back(game_ship);
                                game_screen = GameScreens::Game;
                                game_play = true;
                            } break;
                            case 1:
                                game_screen = GameScreens::StartMenu;
                            break;
                            case 2:
                                exit(0);
                        }
                    break;
                    case GameScreens::PauseMenu:
                        switch(game_pause_menu.Position) {
                            case 0:
                                setButton(Qt::Key_W,true);
                                setButton(Qt::Key_D,true);
                                setButton(Qt::Key_S,true);
                                setButton(Qt::Key_A,true);
                                setButton(Qt::Key_Escape,true);
                                setButton(Qt::Key_U,true);
                                setButton(Qt::Key_J,false);
                                game_screen = GameScreens::Game;
                                game_play = true;
                            break;
                            case 1:
                                game_screen = GameScreens::StartMenu;
                            break;
                            case 2:
                                exit(0);
                        }
                    break;
                    case GameScreens::Game:
                        //addBullet(0);
                    break;
                }
            }
        } break;
        case Qt::Key_K: {
            myKey[BUTTON_ACT2] = press;
        } break;
        case Qt::Key_U: {
            myKey[BUTTON_ACT3] = press;
            if(press && game_screen == GameScreens::Game) addBullet(0);
        } break;
        case Qt::Key_I: {
            myKey[BUTTON_ACT4] = press;
        } break;
        case Qt::Key_Q: {
            myKey[BUTTON_ACT5] = press;
        } break;
        case Qt::Key_E: {
            myKey[BUTTON_ACT6] = press;
        } break;
        case Qt::Key_H: {
            myKey[BUTTON_ACT7] = press;
        } break;
        case Qt::Key_Y: {
            myKey[BUTTON_ACT8] = press;
        } break;
        case Qt::Key_P: {

        } break;
        case Qt::Key_L: {

        } break;
        case Qt::Key_Escape: {
            if(press)   {
                switch(game_screen) {
                    case GameScreens::Game:
                        game_screen = GameScreens::PauseMenu;
                        game_play = false;
                        setButton(Qt::Key_W,true);
                        setButton(Qt::Key_D,false);
                        setButton(Qt::Key_S,true);
                        setButton(Qt::Key_A,false);
                        setButton(Qt::Key_Escape,true);
                        setButton(Qt::Key_U,false);
                        setButton(Qt::Key_J,true);
                    break;
                    case GameScreens::PauseMenu:
                        game_screen = GameScreens::Game;
                        setButton(Qt::Key_W,true);
                        setButton(Qt::Key_D,true);
                        setButton(Qt::Key_S,true);
                        setButton(Qt::Key_A,true);
                        setButton(Qt::Key_Escape,true);
                        setButton(Qt::Key_U,true);
                        setButton(Qt::Key_J,false);
                        game_play = true;
                    break;
                }
               //exit(0);
            }
        } break;
    }
}

void Widget::OnTouch()  {
    for(int i = 0; i < BUTTON_COUNT; i++) {
        TouchKeys[i]->Value = false;
    }
    int tpx;
    int tpy;
    float tpdX;
    float tpdY;
    foreach (QTouchEvent::TouchPoint tp, listKeys)  {
        if(bRotateScreen == false)  {
            tpx = tp.pos().x();
            tpy = tp.pos().y();
            tpdX = dX;
            tpdY = dY;
        } else {
            tpx = tp.pos().y();
            tpy = (this->width() - tp.pos().x());
            tpdX = dY;
            tpdY = dX;
        }
        switch (tp.state()) {
            case Qt::TouchPointReleased:
                for(int i = 0; i < BUTTON_COUNT; i++) {
                    if(
                      (tpx > TouchKeys[i]->Geometry.x()*tpdX && tpx < (TouchKeys[i]->Geometry.x() + TouchKeys[i]->Geometry.width())*tpdX)&&
                      (tpy > TouchKeys[i]->Geometry.y()*tpdY && tpy < (TouchKeys[i]->Geometry.y() + TouchKeys[i]->Geometry.height())*tpdY)
                      ) {
                            TouchKeys[i]->Value = false;
                    }
                }
            break;
            case Qt::TouchPointStationary:
            case Qt::TouchPointPressed:
            case Qt::TouchPointMoved:
                for(int i = 0; i < BUTTON_COUNT; i++) {
                    if(
                      (tpx > TouchKeys[i]->Geometry.x()*tpdX && tpx < (TouchKeys[i]->Geometry.x() + TouchKeys[i]->Geometry.width())*tpdX)&&
                      (tpy > TouchKeys[i]->Geometry.y()*tpdY && tpy < (TouchKeys[i]->Geometry.y() + TouchKeys[i]->Geometry.height())*tpdY)
                      ) {
                            TouchKeys[i]->Value = true;
                    }
                }
            break;
        }
    }
    for(int i = 0; i < BUTTON_COUNT; i++) {
        if(TouchKeys[i]->oldValue != TouchKeys[i]->Value)    {
            OnKeys(TouchKeys[i]->Code,TouchKeys[i]->Value);
            TouchKeys[i]->oldValue = TouchKeys[i]->Value;
        }
    }
}

void Widget::OnDrawText(int x, int y, int size, QString text_eng, QString text_rus, QPainter *pnt)  {
    QString text;
    if(Settings.bEnglish == true)   {
        text = text_eng;
    } else {
        text = text_rus;
    }
    switch(size)    {
        case FONT_SIZE_MICRO:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_micro[i].width(),y,imgSymbol_micro[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_SMALL:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_small[i].width(),y,imgSymbol_small[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_MEDIUM:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_medium[i].width(),y,imgSymbol_medium[i]);
                    }
                }
            }
        break;
        case FONT_SIZE_LARGE:
            for(int cnt = 0; cnt < text.size(); cnt++)    {
                for(int i = 0; i < sSymbols.size(); i++)    {
                    if(text.at(cnt) == sSymbols.at(i))    {
                        pnt->drawImage(x + (cnt)*imgSymbol_big[i].width(),y,imgSymbol_big[i]);
                    }
                }
            }
        break;
    }
}

void Widget::setColor(QColor brush, QColor pen, Qt::BrushStyle style, QPainter *qScr) {
    qScr->setBrush(QBrush(brush, style));
    qScr->setPen(QPen(pen));
}

void Widget::keyPressEvent(QKeyEvent *event)    {
    if(!event->isAutoRepeat())
        OnKeys(event->key(),true);
    QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent *event)  {
    if(!event->isAutoRepeat())
        OnKeys(event->key(),false);
    QWidget::keyReleaseEvent(event);
}


