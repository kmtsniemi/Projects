/* Nysse
 *
 * Class: ActorItem
 * ----------
 *
 * Program author
 * Name: Julia Harttunen and Krista Mätäsniemi
 * Student number: 291740 and 292064
 * UserID: mqjuha and sgkrma
 * E-Mail: julia.harttunen@tuni.fi and krista.matasniemi@tuni.fi
 *
 * */

#ifndef ACTORITEM_HH
#define ACTORITEM_HH

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QImage>

namespace GameSide {

const int WIDTH = 15;
const int HEIGHT = 15;

class ActorItem : public QGraphicsItem
{

public:

    /**
     * @brief Default constructor
     * @param x-coordinate
     * @param y-coordinate
     * @param type chooses right image
     */
    ActorItem(int x, int y, int type = 0);

    /**
      * @brief Default destructor
      */
    ~ActorItem();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief setCoord sets new coordinates to the actor item
     * @param new x-coordinate
     * @param new y-coordinate
     */
    void setCoord(int x, int y);

private:
    // x-coordinate
    int x_;
    // y-coordinate
    int y_;
    // helps choose image
    int type_;
};

}
#endif // ACTORITEM_HH
