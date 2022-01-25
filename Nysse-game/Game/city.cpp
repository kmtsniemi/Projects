#include "city.hh"
#include <QTime>
#include <QPainter>
#include <QtDebug>
#include "actor.hh"
#include <typeinfo>
#include "errors/initerror.hh"
#include "errors/gameerror.hh"

int MAP_SIZE = 500;

City::City(): gameWindow_(std::make_shared<GameSide::MainWindow>())
{
    connect(gameWindow_.get(), &GameSide::MainWindow::gameStarted, this, &City::startGameTime);
    connect(gameWindow_.get(), &GameSide::MainWindow::ate, this, &City::playerAteVehicle);
    connect(&timer_, &QTimer::timeout, this, &City::gametime);

    //Makes startwindow
    StartWindow* startWindow = new StartWindow();
    startWindow->setWindowTitle("Start Window");
    connect(startWindow, &StartWindow::wantstoPlay, gameWindow_.get(), &GameSide::MainWindow::getPlayersName);
    int result = startWindow->exec();

    //Closing the program from the x button on the startwindow
    if (result == QDialog::Rejected){
        throw Interface::InitError("Startwindow closed.");
    }
    gameWindow_->show();
    gameWindow_->setWindowTitle("Main Window");
}

void City::setBackground(QImage &basicbackground, QImage &bigbackground)
{
    gameWindow_->setPicture(basicbackground);
    qDebug() << "Picture for the game area is set.";
}

void City::setClock(QTime clock)
{
    if (clock.isValid())
    {
        gameWindow_->setClock(clock.toString("hh:mm"));
        qDebug() << "Time is set.";
    }
}

void City::addStop(std::shared_ptr<Interface::IStop> stop)
{
    int x = stop->getLocation().giveX();
    int y = MAP_SIZE - stop->getLocation().giveY();

    stops_.push_back(stop);
    gameWindow_->addActor(x, y, 2);
    gameWindow_->setStops(stop);
    qDebug() << "Stop is added to the city.";
}

void City::startGame()
{
    qDebug() << "City is in gamestate.";
    statistics_->readOldTopList();
}

void City::addActor(std::shared_ptr<Interface::IActor> newactor)
{
    actors_.push_back(newactor);
    int x = newactor->giveLocation().giveX();
    int y = MAP_SIZE - newactor->giveLocation().giveY();

    //Busses
    if ( dynamic_cast<CourseSide::Nysse*>(newactor.get()) )
    {
        gameWindow_->addActor(x, y, 1);
    }
    //Passengers
    else if ( dynamic_cast<CourseSide::Passenger*>(newactor.get()) )
    {
        gameWindow_->addActor(x, y);
    }
    //Airplane
    else if (dynamic_cast<Airplane*>(newactor.get()) ){
        gameWindow_->addActor(x, y, 4);
    }
    //Player
    else
    {
        gameWindow_->addActor(x, y, 3);
        gameWindow_->setPlayer();
    }
    gameWindow_->setActor(newactor);
    qDebug() << "Actor is added to the city.";
}

void City::removeActor(std::shared_ptr<Interface::IActor> actor)
{
    gameWindow_->removeActor(actor);
    actorRemoved(actor);
    qDebug() << "Actor is removed from the city.";
}

void City::actorRemoved(std::shared_ptr<Interface::IActor> actor)
{
    for (std::shared_ptr<Interface::IActor> a : actors_)
    {
        if (a == actor)
        {
            a->remove();
        }
    }
}

bool City::findActor(std::shared_ptr<Interface::IActor> actor) const
{
    for (std::shared_ptr<Interface::IActor> a : actors_)
    {
        if (a == actor)
        {
            return true;
        }
    }
    return false;
}

void City::actorMoved(std::shared_ptr<Interface::IActor> actor)
{
    gameWindow_->moveActor(actor);
}

std::vector<std::shared_ptr<Interface::IActor> > City::getNearbyActors(Interface::Location loc) const
{
    std::vector<std::shared_ptr<Interface::IActor>> actors;

    for (std::shared_ptr<Interface::IActor> actor : actors_)
    {
        if (actor->giveLocation().isClose(loc))
        {
            actors.push_back(actor);
        }
    }
    return actors;
}

bool City::isGameOver() const
{
    return gameover_;
}

void City::addPlayer()
{
    int x = 250;
    int y = 250;

    player_->setLocation(x, y);
    addActor(player_);
}

void City::addAirplane()
{
    int x = 1;
    int y = 1;

    airplane_->setLocation(x, y);
    addActor(airplane_);
    airplane_added_ = true;
}

void City::playerMoved(int step_x, int step_y)
{ 
    int x = player_->giveLocation().giveX()+step_x;
    int y = player_->giveLocation().giveY()+step_y;

    if (gameWindow_->isInScreen(x, y))
    {
    player_->setLocation(x, y);
    gameWindow_->movePlayer(player_);
    }
}

std::shared_ptr<GameSide::MainWindow> City::getGameWindow()
{
    return gameWindow_;
}

void City::playerAteVehicle()
{
    int passengers = 0;
    std::vector<std::shared_ptr<Interface::IActor> > actors = getNearbyActors(player_->giveLocation());

    for (std::shared_ptr<Interface::IActor> actor : actors)
    {
        if ( dynamic_cast<CourseSide::Nysse*>(actor.get()) )
        {
            std::shared_ptr<CourseSide::Nysse> bus = std::dynamic_pointer_cast<CourseSide::Nysse>(actor);
            passengers += bus->getPassengers().size();

            for ( std::shared_ptr<Interface::IPassenger> passenger : bus->getPassengers())
            {
                passenger->remove();
                gameWindow_->removeActor(passenger);
            }
            actorRemoved(bus);
            bus->remove();
            gameWindow_->removeActor(bus);
        }
        if ( dynamic_cast<Airplane*>(actor.get()) )
        {
            actorRemoved(airplane_);
            airplane_->remove();
            gameWindow_->removeActor(airplane_);
            gameWindow_->releaseX2Button();
        }
    }
    statistics_->passengerDied(passengers);
}

void City::setLogic(std::shared_ptr<CourseSide::Logic> log)
{
    logic_ = log;
}

void City::gameOver()
{
    gameover_ = true;
    logic_->increaseTime();
    timer_.stop();

    for (std::shared_ptr<Interface::IActor> actor : actors_ )
    {
        gameWindow_->removeActor(actor);
    }
    for (std::shared_ptr<Interface::IStop> stop : stops_ )
    {
        gameWindow_->removeStop(stop);
    }
    gameWindow_->removePlyer();
    gameWindow_->playerWon();
    finaleTime();
    statistics_->updateTopList();
}

void City::setStatistics(std::shared_ptr<Statistics> s)
{
    statistics_ = s;
    connect(statistics_.get(), &Statistics::won, this, &City::gameOver);
}

void City::gametime()
{ 
    time_ = time_.addSecs(1);
    gameWindow_->setGameClock(time_.toString("mm:ss"));

    if(time_.toString("mm:ss") == "00:30")
    {
        addAirplane();
    }
    if(airplane_added_==true)
    {
        moveAirplane();
    }
}

void City::moveAirplane()
{
    Interface::Location newloc;
    int x = airplane_->giveLocation().giveX() + 10;
    int y = airplane_->giveLocation().giveY() + 10;

    if (gameWindow_->isInScreen(x, y))
    {
        newloc.setXY(x,y);
        airplane_->move(newloc);
        actorMoved(airplane_);
    }
    else
    {
        airplane_added_ = false;
        removeActor(airplane_);
    }
}

void City::startGameTime()
{
    timer_.start(1000);
    addPlayer();
}

void City::finaleTime()
{
    int sec = time_.second();
    int min = time_.minute();
    int time = 60 * min + sec;

    gameWindow_->finaleNameAndTime(time);
}
