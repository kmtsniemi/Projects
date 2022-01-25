/* Nysse
 *
 * Class: City
 * ----------
 *
 * Program author
 * Name: Julia Harttunen and Krista Mätäsniemi
 * Student number: 291740 and 292064
 * UserID: mqjuha and sgkrma
 * E-Mail: julia.harttunen@tuni.fi and krista.matasniemi@tuni.fi
 *
 * */

#ifndef CITY_HH
#define CITY_HH
#include "interfaces/icity.hh"
#include "interfaces/iactor.hh"
#include "actors/stop.hh"
#include "core/location.hh"
#include "core/logic.hh"
#include <QMainWindow>
#include "mainwindow.hh"
#include "actor.hh"
#include "actors/nysse.hh"
#include <memory>
#include "airplane.hh"

class City:public QObject, public Interface::ICity
{
    Q_OBJECT

public:
    
    /**
     * @brief Default constructor
     */
    City();
    
    /**
      * @brief Default destructor
      */
    ~City() = default;
    
    /**
     * @brief setBackground sets the bitmap picture of the game area.
     * @param basicbackground  Normal sized picture used as the game area. Bottom left position of the picture in pixelcoordinates can be found out using the offset()-method.
     * @param bigbackground  Background of the game that is bigger than normal. Used only if doing Scrolling map-expansion. Bottom left position of the picture in pixelcoordinates can be found out using the offset()-method.
     * @pre City is in init state.
     * @post Picture for the game area is set. Exception guarantee: basic.
     * @exception InitError Setting the picture was unsuccesful or the picture was invalid.
     */
    void setBackground(QImage& basicbackground, QImage& bigbackground);
    
    /**
     * @brief setClock sets the time of the game clock.
     * @param clock Game clock time at the function call.
     * @pre `clock.isValid() == true`.
     * @post Time is set. Exception guarantee: nothrow.
     */
    void setClock(QTime clock);
    
    /**
     * @brief addStop adds a stop to the city.
     * @param stop pointer to a stop object.
     * @pre City is in init state.
     * @post Stop is added to the city. Exception guarantee: basic
     * @exception InitError Stops position is not valid.
     */
    void addStop(std::shared_ptr<Interface::IStop> stop);
    
    /**
     * @brief startGame shofts city from init state to the gamestate.
     * @pre City is in init state. setBackground() and setClock() have been called.
     * @post City is in gamestate. Exception guarantee: nothrow.
     */
    void startGame() ;
    
    /**
     * @brief addActor adds a new actor to the city.
     * @param newactor actor to be added to the city that fulfills ActorIF.
     * @pre -
     * @post Actor is added to the city. Exception guarantee: basic.
     * @exception GameError Actor is already in the city.
     */
    void addActor(std::shared_ptr<Interface::IActor> newactor);
    /**
     * @brief removeActor removes the actor from the city.
     * @param actor Actor to be removed.
     * @pre City is in gamestate.
     * @post Actor is removed from the city. Exception guarantee: strong.
     * @exception GameError Actor not found in the city
     */
    void removeActor(std::shared_ptr<Interface::IActor> actor) ;
    
    /**
     * @brief actorRemoved tells the city that actor is removed ingame.
     * @param actor Actor that is set removed ingame.
     * @pre City is in gamestate. Given actor is found in the city. Actor has `actor.isRemoved() == true`.
     * @post Exception guarantee: strong.
     */
    void actorRemoved(std::shared_ptr<Interface::IActor> actor) ;
    
    /**
     * @brief findActor checks if the given actor is in the city.
     * @param actor Actor that that is looked for in the city.
     * @pre -
     * @return Boolean that tells wether the actor is in the city.
     * @post Exception guarantee: nothrow.
     */
    bool findActor(std::shared_ptr<Interface::IActor> actor) const;
    
    /**
     * @brief actorMoved is an operation that is used to tell wether certain actor has moved.
     * @param actor Actor that has moved.
     * @pre City is in gamestate. Given actor is found in the city.
     * @post Exception guarantee: basic.
     */
    void actorMoved(std::shared_ptr<Interface::IActor> actor) ;
    
    /**
     * @brief getNearbyActors returns actors that are close to given position.
     * @param loc Location for getting the actors close to it.
     * @pre City is in gamestate.
     * @return Vector containing actors close to the location, that pass `getLocation().isClose(loc) == true`.
     * @post Exception guarantee: strong.
     */
    std::vector<std::shared_ptr<Interface::IActor>> getNearbyActors(Interface::Location loc) const ;
    
    /**
     * @brief isGameOver tells wether the game is overor not.
     * @pre City is in gamestate.
     * @return `true`, if game is over, else ´false´
     * @post Exception guarantee: nothrow.
     */
    bool isGameOver() const ;

    /**
     * @brief addPlayer sets the actor to map and sets start coordinates
     */
    void addPlayer();

    /**
     * @brief addAirplane sets the airplane to the map and sets start coordinates
     */
    void addAirplane();

    /**
     * @brief playerMoved moves the player on the map
     * @param step_x tells how much player moves to the right or the left
     * @param step_y tells how much player moves up or down
     */
    void playerMoved(int step_x, int step_y);

    /**
     * @brief getGameWindow returns pointer
     * @return shared pointer to the gamewindow
     */
    std::shared_ptr<GameSide::MainWindow> getGameWindow();

    /**
     * @brief playerAteVehicle checks if the actor is on the same coordinates than a vehicle and if it is, it removes the vehicle
     */
    void playerAteVehicle();

    /**
     * @brief setLogic saves pointer to the used logic object
     * @param shared pointer to the used logic object
     */
    void setLogic(std::shared_ptr<CourseSide::Logic> log);

    /**
     * @brief gameOver stops timers and removes all items
     */
    void gameOver();

    /**
     * @brief setStatistics saves pointer to the used statistics object
     * @param shared pointer to the used statictics object
     */
    void setStatistics(std::shared_ptr<Statistics> s);

    /**
     * @brief gametime updates time on screen and checks when airplane is moving
     */
    void gametime();

    /**
     * @brief moveAirplane moves airplane
     */
    void moveAirplane();

    /**
     * @brief startGameTime starts time when game starts
     */
    void startGameTime();

    /**
     * @brief finaleTime saves the time what is spent to playing
     */
    void finaleTime();

private:
    // Used main window
    std::shared_ptr<GameSide::MainWindow> gameWindow_;
    // Palyer object
    std::shared_ptr<Actor> player_ = std::make_shared<Actor>();
    // Airplane object
    std::shared_ptr<Airplane> airplane_ = std::make_shared<Airplane>();
    // Bus, player and airplane objects
    QVector<std::shared_ptr<Interface::IActor>> actors_;
    // Stop objects
    QVector<std::shared_ptr<Interface::IStop>> stops_;
    // Used logic object
    std::shared_ptr<CourseSide::Logic> logic_;
    // Tells if game is over
    bool gameover_ = false;
    // Used statistics object
    std::shared_ptr<Statistics> statistics_;
    // Current time
    QTime time_ = QTime(0,0);
    // Timer that increments time_ every second
    QTimer timer_;
    // Tells if airplane is in map
    bool airplane_added_ = false;
};

#endif // CITY_HH
