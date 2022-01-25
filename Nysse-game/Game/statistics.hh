/* Nysse
 *
 * Class: Statistics
 * ----------
 *
 * Program author
 * Name: Julia Harttunen and Krista Mätäsniemi
 * Student number: 291740 and 292064
 * UserID: mqjuha and sgkrma
 * E-Mail: julia.harttunen@tuni.fi and krista.matasniemi@tuni.fi
 *
 * */

#ifndef STATISTICS_HH
#define STATISTICS_HH

#include "interfaces/istatistics.hh"
#include <QObject>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>


class Statistics: public QObject
{
    Q_OBJECT

public:
    
    /**
     * @brief Default constructor
     */
    Statistics();
    
    /**
      * @brief Default destructor
      */
    ~Statistics();
    
    /**
     * @brief passengerDied check how many passengers died
     * @param Number of passengers that the player has eaten
     */
    void passengerDied(int num);

    /**
     * @brief pointReview check the points
     */
    void pointReview();

    /**
     * @brief setNameAndTime saves name nad gametime
     * @param Players name
     * @param How long time the game took
     */
    void setNameAndTime(std::string name, int time);

    /**
     * @brief readOldTopList reads top-list file
     */
    void readOldTopList();

    /**
     * @brief updateTopList writes top-list and add the changes
     */
    void updateTopList();

    /**
     * @brief douplePoints doubles the points when the palyer wants
     */
    void douplePoints();

    /**
     * @brief setPoints saves points
     * @param Points
     */
    void setPoints(int num);

    /**
     * @brief givePoints returns the points.
     * @return Points.
     */
    int givePoints();

    /**
     * @brief setFilename saves filename
     * @param Filename
     */
    void setFilename(std::string filename);

    /**
     * @brief giveInfo returns the info
     *        (times and names).
     * @return Info.
     */
    std::map<int, std::string> giveInfo();

    /**
     * @brief giveTimes returns the gametimes.
     * @return Times.
     */
    std::list<int> giveTimes();

    /**
     * @brief giveFilename returns the filename.
     * @return Filename.
     */
    std::string giveFilename();

    /**
     * @brief giveName returns player's name.
     * @return Name.
     */
    std::string giveName();

    /**
     * @brief giveTime returns palyer's gametime.
     * @return Time.
     */
    int giveTime();

private:
    // Points
    int points_ = 0;
    // Players name
    std::string name_;
    // Game time
    int time_;
    // Information of top-list
    std::map<int, std::string> info_;
    // Top-lists times
    std::list<int> times_;
    // Name of top list file
    std::string filename_ = "top10list.txt";

signals:
    /**
     * @brief won tells if game is over
     */
    void won();

    /**
     * @brief addPoints send current points
     * @param points
     */
    void addPoints(int points);

    /**
     * @brief topList send to top list information
     * @param old game times and player names from top list
     * @param old game times from the top list
     */
    void topList(std::map<int, std::string> info, std::list<int> times);
};

#endif // STATISTICS_HH
