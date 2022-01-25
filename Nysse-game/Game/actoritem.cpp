#include "actoritem.hh"

int BUS = 1;
int STOP = 2;
int PLAYER = 3;
int AIRPLANE = 4;

namespace GameSide {

ActorItem::ActorItem(int x, int y, int type):x_(x), y_(y), type_(type)
{
    setPos(mapToParent(x_, y_ ));
}

ActorItem::~ActorItem()
{

}

QRectF ActorItem::boundingRect() const
{
    return QRectF(0, 0, WIDTH, HEIGHT);
}

void ActorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QImage img;
    QRectF bounds = boundingRect();

    if ( type_ == BUS )
    {
        img = QImage(":/kuvat/kuvia/bussi_14x14.png");
    }
    else if ( type_ == STOP )
    {
        img = QImage(":/kuvat/kuvia/pysäkki_14x14.png");
    }
    else if ( type_ == PLAYER )
    {
        img = QImage(":/kuvat/kuvia/pelaaja_14x14.png");
    }
    else if ( type_ == AIRPLANE )
    {
        img = QImage(":/kuvat/kuvia/lentokone2_20x20.png");
    }
    else
    {
        img = QImage(":/kuvat/kuvia/matkustaja_14x14.png");
    }
    QBrush brush(img);
    painter->setBrush(brush);
    painter->drawImage(bounds, img);
}

void ActorItem::setCoord(int x, int y)
{
    setX( x );
    setY( y );
}
}
