/* Nysse
 *
 * Class: MainWindow
 * ----------
 *
 * Program author
 * Name: Julia Harttunen and Krista Mätäsniemi
 * Student number: 291740 and 292064
 * UserID: mqjuha and sgkrma
 * E-Mail: julia.harttunen@tuni.fi and krista.matasniemi@tuni.fi
 *
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QGraphicsScene>
#include <QTimer>
#include <iostream>
#include <memory>
#include <QVector>
#include <map>
#include "startwindow.hh"
#include "interfaces/icity.hh"
#include "actor.hh"
#include "actoritem.hh"
#include "statistics.hh"


namespace Ui {
class MainWindow;
}

namespace GameSide {
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief  Default constructor
     * @param QWidget object
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
      * @brief Default destructor
      */
    ~MainWindow();

    /**
     * @brief Makes actoritem objects
     * @param item's x-coordinate
     * @param item's y-coordinate
     * @param type of item
     */
    void addActor(int locX, int locY, int type = 0);

    /**
     * @brief Makes stopitem objects
     * @param item's x-coordinate
     * @param item's y-coordinate
     * @param type of item
     */
    void addStop(int locX, int locY, int type = 0);

    /**
     * @brief Shows time in gamewindow
     * @param current time
     */
    void setClock(QString time);

    /**
     * @brief Sets the map to gamewindow
     * @param map image
     */
    void setPicture(QImage &img);

    /**
     * @brief saves used city object
     * @param shared pointer to used city object
     */
    void takeCity(std::shared_ptr<Interface::ICity> c);

    /**
     * @brief Shows player's name in gamewindow and saves it
     * @param player's name
     */
    void getPlayersName(QString name);

    /**
     * @brief Saves pointer to actor's item
     * @param shared pointer to the actor
     */
    void setActor(std::shared_ptr<Interface::IActor> a);

    /**
     * @brief Set actor's item to the new coordinates
     * @param shared pointer to the actor
     */
    void moveActor(std::shared_ptr<Interface::IActor> a);

    /**
     * @brief Saves pointer to stop's item
     * @param shared pointer to the stop
     */
    void setStops(std::shared_ptr<Interface::IStop> s);

    /**
     * @brief Saves player's item
     */
    void setPlayer();

    /**
     * @brief Set player's item to the new coordinates
     * @param shared pointer to the player
     */
    void movePlayer(std::shared_ptr<Actor> player);

    /**
     * @brief Hides actor's item
     * @param shared pointer to the actor
     */
    void removeActor(std::shared_ptr<Interface::IActor> a);

    /**
     * @brief Hides stop's item
     * @param shared pointer to the stop
     */
    void removeStop(std::shared_ptr<Interface::IStop> stop);

    /**
     * @brief Checks that coordinates are in the game area
     * @param x-coordinate
     * @param y-coordinate
     * @return true if coordinates are in area otherwise false
     */
    bool isInScreen(int new_x, int new_y);

    /**
     * @brief Move and eat action from keyboard
     * @param Pressed key
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Sets won text to screen
     */
    void playerWon();

    /**
     * @brief Sets points to the screen
     * @param points
     */
    void setPoints(int points);

    /**
     * @brief Saves used statistics object
     * @param shared pointer to the statistcs
     */
    void takeStatistics(std::shared_ptr<Statistics> s);

    /**
     * @brief Hides player's item
     */
    void removePlyer();

    /**
     * @brief Sets game time to the screen
     * @param game time
     */
    void setGameClock(QString time);

    /**
     * @brief Releases x2 button when the player has eaten airplane
     */
    void releaseX2Button();

    /**
     * @brief Sets player's name and game time to the statistics
     * @param game time
     */
    void finaleNameAndTime(int time);

    /**
     * @brief Writes top list to the screen
     * @param top list's names and times
     * @param top list's times
     */
    void writeTopList(std::map<int, std::string> info, std::list<int> times);


private:
    // Gamewindow
    Ui::MainWindow *ui;
    // Map
    QGraphicsScene *map;
    // Game timer
    QTimer *timer;
    // Actors and actor items
    std::map< std::shared_ptr<Interface::IActor>, GameSide::ActorItem*> items_ ={};
    // Stop items and stops
    std::map < std::shared_ptr<Interface::IStop>, GameSide::ActorItem*> stopitems_ = {};
    // Actor items
    QVector<QGraphicsItem*> actors_;
    // Actor item what has added the lastest
    GameSide::ActorItem* last_;
    // Shared pointer to used city
    std::shared_ptr<Interface::ICity> city_;
    // Pointer to player's item
    GameSide::ActorItem* player_item_;
    // Shared pointer to used statistics
    std::shared_ptr<Statistics> statistics_;
    // Player's name
    std::string players_name_;

    int width_= 500;
    int height_ = 500;
    int tick_ = 500;

signals:

    /**
     * @brief Starts game
     */
    void gameStarted();

    /**
     * @brief Notify if actor was eaten
     */
    void ate();

private slots:

    /**
     * @brief Starts game
     */
    void on_startPushButton_clicked();

    /**
     * @brief moves player up
     */
    void on_upPushButton_clicked();

    /**
     * @brief Moves player down
     */
    void on_downPushButton_clicked();

    /**
     * @brief Moves player to the left
     */
    void on_leftPushButton_clicked();

    /**
     * @brief Moves player to the right
     */
    void on_rightPushButton_clicked();

    /**
     * @brief Player eats
     */
    void on_eatPushButton_clicked();

    /**
     * @brief Doubles points
     */
    void on_x2PushButton_clicked();
};
}
#endif // MAINWINDOW_HH



