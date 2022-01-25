/* Class: MainWindow
  * ----------
  * This class is the mainwindow of Tetris game.
  *
  * Name: Krista Mätäsniemi
  * E-Mail: krista.matasniemi@tuni.fi
  **/
#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    minutes_(0.00),
    seconds_(0.00)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates, but if you want
    // different placement, you can change their values.
    int left_margin = 100; // x coordinate
    int top_margin = 150; // y coordinate

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);

    // Setting random engine ready for the first real call.
    int seed = time(0); // You can change seed value for testing purposes
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
    distr(randomEng); // Wiping out the first random number (which is almost always 0)
    // After the above settings, you can use randomEng to draw the next falling
    // tetromino by calling: distr(randomEng) in a suitable method.

    // Add more initial settings and connect calls, when needed.

    timer_.setSingleShot(false);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::move_down);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::second_gone);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    if ( event->key() == Qt::Key_A ){
        move_side(MOVE_LEFT);
        return;
    }
    if ( event->key() == Qt::Key_D ){
        move_side(MOVE_RIGHT);
        return;
    }
    if ( event->key() == Qt::Key_S ){
        move_down();
        return;
    }
}

void MainWindow::new_block()
{
    game_over();
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);
    int shape = distr(randomEng) % 8;

    for (int i = 0; i < 4 ; i++) {
        int x = SHAPES.at(shape).at(i).at(0);
        int y = SHAPES.at(shape).at(i).at(1);
        QGraphicsRectItem* block = scene_->addRect(0, 0, 20, 20,blackPen, COLORS.at(shape));
        block->moveBy(SQUARE_SIDE*x, SQUARE_SIDE*y);
        tetromino_.push_back(block);
    }
}

void MainWindow::move_down()
{
    qreal deltaX, deltaY;
    deltaY = static_cast<qreal>(SQUARE_SIDE);
    deltaX = static_cast<qreal>(0);

    if ( not has_set(MOVE_DOWN) ){
        for ( QGraphicsRectItem* block : tetromino_ ){
            block->moveBy(deltaX, deltaY);
        }
        return;
    }
    for ( QGraphicsRectItem* block : tetromino_ ){
        int current_y = block->y()/SQUARE_SIDE;
        int current_x = block->x()/SQUARE_SIDE;
        grid_.at(current_y).at(current_x) = 1;
    }
    game_over();
    tetromino_.clear();
    add_points();
    new_block();
}

void MainWindow::move_side(std::string direction)
{
    if ( not has_set(direction) ){
        qreal deltaX;
        if ( direction == MOVE_RIGHT ){
            deltaX = static_cast<qreal>(SQUARE_SIDE);
        } else {
            deltaX = static_cast<qreal>(-SQUARE_SIDE);
        }
        for ( QGraphicsRectItem* block : tetromino_ ){
            block->moveBy(deltaX, 0);
        }
    }
}

bool MainWindow::has_set(std::string direction)
{
    for ( QGraphicsRectItem* tetromino_block : tetromino_ ){
        qreal current_x = tetromino_block->x();
        qreal current_y = tetromino_block->y();
        int x = tetromino_block->x()/SQUARE_SIDE;
        int y = tetromino_block->y()/SQUARE_SIDE;
        if ( direction == MOVE_RIGHT ){
            current_x += SQUARE_SIDE;
            x += 1;
        } else if ( direction == MOVE_LEFT ) {
            current_x -= SQUARE_SIDE;
            x -= 1;
        } else {
            current_y += SQUARE_SIDE;
            y += 1;
        }
        QRectF rect = scene_->sceneRect();
        if(not rect.contains(current_x, current_y)) {
            return true;
        }
        if ( grid_.at(y).at(x) == 1 ){
            return true;
        }
    }
    return false;
}

void MainWindow::second_gone()
{
    seconds_ += 0.01;

    if (seconds_ >= 0.60){
        ++minutes_;
        seconds_ = 0.00;
    }
    ui->lcdNumber->display(minutes_ + seconds_);
}

void MainWindow::create_grid()
{
    std::vector<int> line;
        for (int x = 0; x <= BORDER_RIGHT/SQUARE_SIDE; ++x) {
            line.push_back(0);
        }
        for (int y = 0; y <= BORDER_DOWN/SQUARE_SIDE; ++y) {
            grid_.push_back(line);
        }
}

void MainWindow::game_over()
{
    for ( int x:grid_.at(0) ){
        if ( x == 1 ){
            timer_.stop();
            ui->textBrowser->setFontPointSize(22);
            ui->textBrowser->setText("GAME OVER!");
        }
    }
}

void MainWindow::add_points()
{
    points_ = 0;
    for ( std::vector<int> row : grid_ ){
        int counter = 0;
        for ( int number : row ){
            if ( number == 1 ){
                ++counter;
            }
        }
        if ( counter == BORDER_RIGHT / SQUARE_SIDE ){
            points_ += ( BORDER_RIGHT / SQUARE_SIDE );
        }
    }
    ui->scoreLcdNumber->display(points_);
}

void MainWindow::on_startButton_clicked()
{
    on_newGameButton_clicked();
    ui->textBrowser->clear();
    create_grid();
    new_block();
    timer_.start(1000);
}

void MainWindow::on_leftButton_clicked()
{
    move_side(MOVE_LEFT);
}

void MainWindow::on_rightButton_clicked()
{
    move_side(MOVE_RIGHT);
}

void MainWindow::on_newGameButton_clicked()
{
    timer_.stop();
    scene_->clear();
    minutes_ = 0.00;
    seconds_ = 0.00;
    points_ = 0;
    ui->lcdNumber->display(minutes_ + seconds_);
    ui->scoreLcdNumber->display(points_);
    ui->textBrowser->setFontPointSize(20);
    ui->textBrowser->setText("START GAME!");
    tetromino_.clear();
    grid_.clear();
}

void MainWindow::on_helpButton_clicked()
{
    ui->textBrowser->setFontPointSize(8);
    ui->textBrowser->setText("The player can steer the falling block left or right with A and D keys. "
                             "By pressing the S key, the player can speed up the falling of the block. "
                             "You can also use bottoms on the screen. "
                             "A falling block stops when it hits the bottom of the playing area or a previously dropped block. "
                             "The player gets points for full rows. The game ends when the block stacks touch the top of the playing area.");
}

void MainWindow::on_pushButton_pressed()
{
    move_down();
}
