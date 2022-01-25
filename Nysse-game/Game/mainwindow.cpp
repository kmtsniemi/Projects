#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>
#include "city.hh"
#include <QKeyEvent>

int COLUMN = 500;
int LINE = 10;
int STEP = 10;

namespace GameSide {

GameSide::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gameView->setFixedSize(width_, height_);
    ui->centralwidget->setFixedSize(width_ + ui->startPushButton->width() + COLUMN, height_ + LINE);

    //The buttons are locked first
    ui->x2PushButton->setDisabled(true);
    ui->upPushButton->setDisabled(true);
    ui->downPushButton->setDisabled(true);
    ui->leftPushButton->setDisabled(true);
    ui->rightPushButton->setDisabled(true);
    ui->eatPushButton->setDisabled(true);

    //Buttons
    ui->x2PushButton->move(600, 39*LINE);
    ui->x2PushButton->setFixedSize(200, 5*LINE);
    ui->startPushButton->move(600, 45*LINE);
    ui->startPushButton->setFixedSize(200, 5*LINE);
    ui->leftPushButton->move(550, 26*LINE);
    ui->leftPushButton->setFixedSize(100, 5*LINE);
    ui->rightPushButton->move(750, 26*LINE);
    ui->rightPushButton->setFixedSize(100, 5*LINE);
    ui->upPushButton->move(650, 20*LINE);
    ui->upPushButton->setFixedSize(100, 5*LINE);
    ui->downPushButton->move(650, 32*LINE);
    ui->downPushButton->setFixedSize(100, 5*LINE);
    ui->eatPushButton->move(650, 25*LINE);
    ui->eatPushButton->setFixedSize(100, 7*LINE);
    ui->eatPushButton->setStyleSheet("background-color: red");

    //Info boxes
    ui->topTextBrowser->move(870, LINE);
    ui->topTextBrowser->setFixedSize(200, 23*LINE);
    ui->infoTextBrowser->setFixedSize(200, 24*LINE);
    ui->infoTextBrowser->move(870, 26*LINE);

    //Points
    ui->pointsLabel->move(620, 15*LINE);
    ui->pointsLcdNumber->move(680, 14*LINE);
    ui->pointsLcdNumber->setFixedSize(80, 4*LINE);
    ui->pointsLcdNumber->setStyleSheet("background-color: yellow");

    //Clocks
    ui->busTimeLcdNumber->move(580, LINE);
    ui->busTimeLcdNumber->setFixedSize(100, 5*LINE);
    ui->busTimeLcdNumber->setStyleSheet("background-color: lightcyan");
    ui->playingTimeLcdNumber->move(720, LINE);
    ui->playingTimeLcdNumber->setFixedSize(100, 5*LINE);
    ui->playingTimeLcdNumber->setStyleSheet("background-color: lightgreen");

    map = new QGraphicsScene(this);
    ui->gameView->setScene(map);
    map->setSceneRect(0,0,width_,height_);

    resize(minimumSizeHint());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, map, &QGraphicsScene::advance);
    timer->start(tick_);

}

GameSide::MainWindow::~MainWindow()
{
    delete ui;
}

void GameSide::MainWindow::addActor(int locX, int locY, int type)
{
    GameSide::ActorItem* nActor = new GameSide::ActorItem(locX, locY, type);
    actors_.push_back(nActor);
    map->addItem(nActor);
    last_ = nActor;
}

void MainWindow::addStop(int locX, int locY, int type)
{
    GameSide::ActorItem* nStop = new GameSide::ActorItem(locX, locY, type);
    map->addItem(nStop);
    last_ = nStop;
}

void GameSide::MainWindow::setClock(QString time)
{
    ui->busTimeLcdNumber->display(time);
}

void GameSide::MainWindow::setPicture(QImage &img)
{
    map->setBackgroundBrush(img);
}

void GameSide::MainWindow::takeCity(std::shared_ptr<Interface::ICity> c)
{
    city_ = c;
}

void GameSide::MainWindow::getPlayersName(QString name)
{
    ui->nameTextBrowser->setFontPointSize(16);
    ui->nameTextBrowser->setText(name);
    ui->nameTextBrowser->move(680, 8*LINE);
    ui->nameLabel->move(620, 9*LINE);

    players_name_ = name.toStdString();
}

void GameSide::MainWindow::setActor(std::shared_ptr<Interface::IActor> a)
{
    items_.insert({a, last_});
}

void GameSide::MainWindow::moveActor(std::shared_ptr<Interface::IActor> a)
{
    items_.at(a)->setCoord(a->giveLocation().giveX(), COLUMN - a->giveLocation().giveY());
}

void MainWindow::setStops(std::shared_ptr<Interface::IStop> s)
{
    stopitems_.insert({s, last_});
}

void GameSide::MainWindow::setPlayer()
{
    player_item_ = last_;
}

void GameSide::MainWindow::movePlayer(std::shared_ptr<Actor> player)
{
    player_item_->setCoord(player->giveLocation().giveX(), COLUMN - player->giveLocation().giveY());
}

void GameSide::MainWindow::removeActor(std::shared_ptr<Interface::IActor> a)
{
    items_.find(a)->second->hide();
}

void MainWindow::removeStop(std::shared_ptr<Interface::IStop> stop)
{
    stopitems_.find(stop)->second->hide();
}

bool MainWindow::isInScreen(int new_x, int new_y)
{
    if ( not ui->gameView->sceneRect().contains(new_x, new_y) )
    {
        return false;
    }
    else
    {
        return true;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    std::shared_ptr<City> c=std::dynamic_pointer_cast<City>(city_);

    if ( event->key() == Qt::Key_A )
    {
        c->playerMoved(-STEP, 0);
    }
    if ( event->key() == Qt::Key_D )
    {
        c->playerMoved(STEP, 0);
    }
    if ( event->key() == Qt::Key_W )
    {
        c->playerMoved(0, STEP);
    }
    if ( event->key() == Qt::Key_S )
    {
        c->playerMoved(0, -STEP);
    }
    if ( event->key() == Qt::Key_B)
    {
        emit ate();
    }
}

void MainWindow::playerWon()
{
    QPainterPath path;
    QFont font;
    font.setPixelSize(25);
    font.setBold(false);
    font.setFamily("Calibri");
    path.addText(50, 250, font,  "YOU WONNN !!! CONGRATULATIONS! :D");
    map->addPath(path, QPen(QBrush(Qt::red), 1), QBrush(Qt::red));

    ui->x2PushButton->setDisabled(true);
}
void MainWindow::setPoints(int points)
{
    ui->pointsLcdNumber->display(points);
}

void MainWindow::takeStatistics(std::shared_ptr<Statistics> s)
{
    statistics_ = s;
    connect(statistics_.get(), &Statistics::addPoints, this, &GameSide::MainWindow::setPoints);
    connect(statistics_.get(), &Statistics::topList, this, &GameSide::MainWindow::writeTopList);
}

void MainWindow::removePlyer()
{
    player_item_->hide();
}

void MainWindow::setGameClock(QString time)
{
    ui->playingTimeLcdNumber->display(time);
}

void MainWindow::releaseX2Button()
{
    ui->x2PushButton->setDisabled(false);
}

void MainWindow::finaleNameAndTime(int time)
{
    statistics_->setNameAndTime(players_name_, time);
}

void MainWindow::writeTopList(std::map<int, std::string> info, std::list<int> times)
{
    int number;
    int ranking = 1;
    std::string text = "TOP 10 : \n";

    times.sort();

    if (times.size() > 10)
    {
        number = 10;
    }
    else
    {
        number = times.size();
    }

    for (int t : times)
    {
        if ( t > 0 )
        {
            if(number > 0)
            {
                std::string time = std::to_string(t);
                std::string name = info.at(t);
                int min = t / 60;
                int sec = t % 60;

                if (sec < 10)
                {
                    text += std::to_string(ranking) + ". " + std::to_string(min)
                            + ":0" + std::to_string(sec) + " " + name + "\n";
                }
                else
                {
                    text += std::to_string(ranking) + ". " + std::to_string(min)
                            + ":" + std::to_string(sec) + " " + name + "\n";
                }
            }
            --number;
            ++ranking;
        }
    }
    QString finaleText = QString::fromStdString(text);
    ui->topTextBrowser->setText(finaleText);
}

void GameSide::MainWindow::on_startPushButton_clicked()
{
    qDebug() << "Start clicked";
    emit gameStarted();
    ui->startPushButton->setDisabled(true);
    ui->upPushButton->setDisabled(false);
    ui->downPushButton->setDisabled(false);
    ui->leftPushButton->setDisabled(false);
    ui->rightPushButton->setDisabled(false);
    ui->eatPushButton->setDisabled(true);
}

void GameSide::MainWindow::on_upPushButton_clicked()
{ 
    std::shared_ptr<City> c=std::dynamic_pointer_cast<City>(city_);
    c->playerMoved(0, STEP);
}

void GameSide::MainWindow::on_downPushButton_clicked()
{
    std::shared_ptr<City> c=std::dynamic_pointer_cast<City>(city_);
    c->playerMoved(0, -STEP);
}

void GameSide::MainWindow::on_leftPushButton_clicked()
{
    std::shared_ptr<City> c=std::dynamic_pointer_cast<City>(city_);
    c->playerMoved(-STEP, 0);
}

void GameSide::MainWindow::on_rightPushButton_clicked()
{
    std::shared_ptr<City> c=std::dynamic_pointer_cast<City>(city_);
    c->playerMoved(STEP, 0);
}

void GameSide::MainWindow::on_eatPushButton_clicked()
{
    qDebug() << "Eat clicked";
    emit ate();
}

void GameSide::MainWindow::on_x2PushButton_clicked()
{
    statistics_->douplePoints();
    ui->x2PushButton->setDisabled(true);
}
}
