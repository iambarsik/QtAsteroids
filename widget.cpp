#include "widget.h"
#include "ui_widget.h"

/*
 *
 * Auto-stretched screen
 * Auto-rotated screen
 * Big font ENG+RUS
 * Medium font ENG+RUS
 * Small font eng+rus
 * Touch buttons
 *
 *
 */

Widget::Widget(QWidget *parent)
    : QGLWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setAttribute((Qt::WA_AcceptTouchEvents));

        // get available screens and scale widget
    QScreen* screen = QApplication::screens().at(0);
    QSize size = screen->availableSize();
    this->setGeometry(0,0,size.width(),size.height());

    GameField = QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE);

    #ifdef Q_OS_WINDOWS
        this->setWindowFlags(Qt::FramelessWindowHint);
    #endif

    Resolution.setX(GameField.width());
    Resolution.setY(GameField.height());

        // rotation flag for mobile devices and calculating aspect ration
    bRotateScreen = this->width() > this->height() ? false : true;
    if(bRotateScreen == false)  {
        dX = (float)(size.width())/(float)Resolution.x();
        dY = (float)(size.height())/(float)Resolution.y();
    } else {
        dX = (float)size.width()/(float)Resolution.y();
        dY = (float)(size.height())/(float)Resolution.x();
    }

        // Определяем соотношение экрана, чтоб знать, относительно чего растягивать канву
    bStretchHorizontal = this->width()/this->height() < SCREEN_TILE_WIDTH/SCREEN_TILE_HEIGHT ? true : false;
    if(bRotateScreen == false)  {
        if(bStretchHorizontal == true) {
            fImageZoom = (float)this->width()/(float)Resolution.x();
            fImageX = 0;
            fImageY = (this->height() - ((float)Resolution.y()*fImageZoom))/(2*fImageZoom);
        } else {
            fImageZoom = (float)this->height()/(float)Resolution.y();
            fImageX = (this->width() - ((float)Resolution.x()*fImageZoom))/(2*fImageZoom);
            fImageY = 0;
        }
    } else {
        if(bStretchHorizontal == true) {
            fImageZoom = (float)this->width()/((float)Resolution.y());
            fImageX = 0;   // доработать
            fImageY = 0;   // доработать
        } else {
            fImageZoom = (float)this->height()/((float)Resolution.x());
            fImageX = 0;   // доработать
            fImageY = 0;   // доработать
        }
    }

        // init user fonts
    sSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,!:+-?* abcdefghijklmnopqrstuvwxyzАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя=";
    FONT_IMAGE.load(":/images/font_micro.png");
    for(int i = 0; i < 136; i++) {
        imgSymbol_micro[i] = FONT_IMAGE.copy(i*8,0,8,8);
    }
    FONT_IMAGE.load(":/images/font_small.png");
    for(int i = 0; i < 136; i++) {
        imgSymbol_small[i] = FONT_IMAGE.copy(i*15,0,15,16);
    }
    FONT_IMAGE.load(":/images/font_medium.png");
    for(int i = 0; i < 136; i++) {
        imgSymbol_medium[i] = FONT_IMAGE.copy(i*30,0,30,32);
    }
    FONT_IMAGE.load(":/images/font_big.png");
    for(int i = 0; i < 136; i++) {
        imgSymbol_big[i] = FONT_IMAGE.copy(i*45,0,45,48);
    }

        // init buttons
    QImage imON;
    QImage imOFF;
    imON.load(":/images/button_on.png");
    imOFF.load(":/images/button_off.png");
    for(int i = 0; i < BUTTON_COUNT; i++)  {
        TouchKeys[i] = new TouchButton(QRect(
                TouchButtonGeometry[i][0]*TILE_SIZE,
                TouchButtonGeometry[i][1]*TILE_SIZE,
                TouchButtonGeometry[i][2]*TILE_SIZE,
                TouchButtonGeometry[i][3]*TILE_SIZE
                ),
                TouchButtonGeometry[i][5]);
        TouchKeys[i]->Enable = TouchButtonGeometry[i][4];
        //TouchKeys[i]->SetImage(imON,imOFF);
        myKey[i] = false;
    }

    imON.load(":/images/buttons/up_p.png");
    imOFF.load(":/images/buttons/up.png");
    TouchKeys[BUTTON_UP]->SetImage(imON,imOFF);
    imON.load(":/images/buttons/right_p.png");
    imOFF.load(":/images/buttons/right.png");
    TouchKeys[BUTTON_RIGHT]->SetImage(imON,imOFF);
    imON.load(":/images/buttons/down_p.png");
    imOFF.load(":/images/buttons/down.png");
    TouchKeys[BUTTON_DOWN]->SetImage(imON,imOFF);
    imON.load(":/images/buttons/left_p.png");
    imOFF.load(":/images/buttons/left.png");
    TouchKeys[BUTTON_LEFT]->SetImage(imON,imOFF);
    imON.load(":/images/buttons/ok_p.png");
    imOFF.load(":/images/buttons/ok.png");
    TouchKeys[BUTTON_ACT2]->SetImage(imON,imOFF);
    imON.load(":/images/buttons/shoot_p.png");
    imOFF.load(":/images/buttons/shoot.png");
    TouchKeys[BUTTON_ACT1]->SetImage(imON,imOFF);
    imON.load(":/images/buttons/options_p.png");
    imOFF.load(":/images/buttons/options.png");
    TouchKeys[BUTTON_MENU]->SetImage(imON,imOFF);

    SetStartValues();

    iForGame = 0;
    iForAnimation = 0;
    GameLoop = new QTimer;
    GameLoop->setTimerType(Qt::PreciseTimer);
    connect(GameLoop, SIGNAL(timeout()), SLOT(GamePlayFunction()));
    GameLoop->setInterval(0);
    GameLoop->start();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::GamePlayFunction() {
    QApplication::processEvents();
    QTime currentTime= QTime::currentTime();
    int msCurrent = currentTime.msecsSinceStartOfDay();

    if(msCurrent - iForGame > ON_GAME_PERIOD) {
        OnGame();
        iForGame = msCurrent;
    }
    if(msCurrent - iForAnimation > ON_ANIMATION_PERIOD) {
        OnAnimation();
        iForAnimation = msCurrent;
    }
}

void Widget::paintEvent(QPaintEvent *event) {
        // init buffer image
    QImage img(Resolution.x(),Resolution.y(),QImage::Format_RGB32);
    QPainter qScreen;
    qScreen.begin(&img);
    qScreen.setRenderHint(QPainter::Antialiasing,false);

    setColor(Qt::black,Qt::black, Qt::SolidPattern, &qScreen);
    qScreen.drawRect(0,0,Resolution.x(),Resolution.y());

        // ====================================== start user drawing ===============================
        // before zooming

    // draw something

        // zooming
    qScreen.save();
    //qScreen.scale(Camera.Zoom,Camera.Zoom);

    // draw something

    if(game_flash)  {
        setColor(Qt::white,Qt::white, Qt::SolidPattern, &qScreen);
        qScreen.drawRect(0,0,Resolution.x(),Resolution.y());
    }

    switch(game_screen) {
        case Logo:  {
            OnDrawText(400, 300, Fonts::FONT_SIZE_LARGE, QString("BARSIK GAME STUDIOS"),QString("BARSIK ГЕЙМ СТУДИОС"),&qScreen);
        } break;
        case Language:  {
            OnDrawText(450, 200, Fonts::FONT_SIZE_LARGE, QString("Language/Язык"),QString("Language/Язык"),&qScreen);
            for(int i = 0; i < game_language_menu.Options.size(); i++)  {
                OnDrawText(300, 300 + i*75, Fonts::FONT_SIZE_LARGE, game_language_menu.getPositionName(i),game_language_menu.getPositionName(i),&qScreen);
            }
        } break;
        case StartMenu: {
            for(int i = 0; i < game_main_menu.Options.size(); i++)  {
                OnDrawText(100, 200 + i*75, Fonts::FONT_SIZE_LARGE, game_main_menu.getPositionName(i),game_main_menu.getPositionName(i),&qScreen);
            }
        } break;
        case PauseMenu: {
            for(int i = 0; i < game_pause_menu.Options.size(); i++)  {
                OnDrawText(100, 200 + i*75, Fonts::FONT_SIZE_LARGE, game_pause_menu.getPositionName(i),game_pause_menu.getPositionName(i),&qScreen);
            }
        } break;
        case DeadMenu:  {
            OnDrawText(100, 150, Fonts::FONT_SIZE_MEDIUM, QString("Your score: %1").arg(game_score),
                QString("Ваш результат: %1").arg(game_score),&qScreen);
            switch(game_spawn_timer)    {
                case SPAWN_TIMER_EASY:
                    OnDrawText(50, 200, Fonts::FONT_SIZE_MEDIUM, QString("Best score on easy level: %1").arg(game_results[0]),
                        QString("Лучший результат на легком уровне: %1").arg(game_results[0]),&qScreen);
                break;
                case SPAWN_TIMER_MEDIUM:
                    OnDrawText(50, 200, Fonts::FONT_SIZE_MEDIUM, QString("Best score on medium level: %1").arg(game_results[1]),
                        QString("Лучший результат на среднем уровне: %1").arg(game_results[1]),&qScreen);
                break;
                case SPAWN_TIMER_HARD:
                    OnDrawText(50, 200, Fonts::FONT_SIZE_MEDIUM, QString("Best score on hard level: %1").arg(game_results[2]),
                        QString("Лучший результат на сложном уровне: %1").arg(game_results[2]),&qScreen);
                break;
                case SPAWN_TIMER_VERY:
                    OnDrawText(50, 200, Fonts::FONT_SIZE_MEDIUM, QString("Best score on very level: %1").arg(game_results[3]),
                        QString("Лучший результат на очень сложном уровне: %1").arg(game_results[3]),&qScreen);
                break;
                case SPAWN_TIMER_SUICIDE:
                    OnDrawText(50, 200, Fonts::FONT_SIZE_MEDIUM, QString("Best score on suicide level: %1").arg(game_results[4]),
                        QString("Лучший результат на самоубийственном уровне: %1").arg(game_results[4]),&qScreen);
                break;
            }
            for(int i = 0; i < game_dead_menu.Options.size(); i++)  {
                OnDrawText(100, 300 + i*75, Fonts::FONT_SIZE_LARGE, game_dead_menu.getPositionName(i),game_dead_menu.getPositionName(i),&qScreen);
            }
        } break;
        case Game:
            for(Convex c: game_object) {
                c.Draw(&qScreen);
            }
            qScreen.setPen(QPen(Qt::white,5,Qt::SolidLine));
            for(myObject b: game_bullet) {
                qScreen.drawPoint(QPoint(b.point()));
            }
            OnDrawText(50,  100, Fonts::FONT_SIZE_MEDIUM,QString("Targets = %1").arg(game_object.size()),QString("Цели = %1").arg(game_object.size()),&qScreen);
            OnDrawText(50, 150, Fonts::FONT_SIZE_MEDIUM,QString("Scores = %1").arg(game_score),QString("Очки = %1").arg(game_score),&qScreen);

                // draw HP status
            setColor(Qt::green,Qt::red,Qt::NoBrush,&qScreen);
            qScreen.drawRect(TILE_SIZE*2,TILE_SIZE*2,MAX_HP*2,TILE_SIZE/2);

            setColor(Qt::green,Qt::red,Qt::SolidPattern,&qScreen);
            qScreen.drawRect(TILE_SIZE*2,TILE_SIZE*2,game_hp*2,TILE_SIZE/2);
        break;
    }

        // after zooming
    qScreen.restore();
        // after zooming

        // ====================================== end user drawing =================================

        // Рисуем сенсорные кнопки, если экран перевёрнут
    if(bRotateScreen)   {
        for(int i = 0; i < BUTTON_COUNT; i++) {
            if(TouchKeys[i]->Enable) TouchKeys[i]->Draw(&qScreen);
        }
    }

    qScreen.end();

        // transposing buffer image to main widget
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing,false);
    setColor(Qt::black,Qt::black, Qt::SolidPattern, &p);
    p.drawRect(0,0,this->width(),this->height());
    if(bRotateScreen == false)   {
        p.scale(fImageZoom,fImageZoom);
        p.drawImage(fImageX,fImageY,img);
    } else {
        p.save();
        p.translate(this->width(),0);
        p.rotate(90);
        p.scale(fImageZoom,fImageZoom);
        p.drawImage(0,0,img);
        p.restore();
    }
    p.end();

    QWidget::paintEvent(event);
}
