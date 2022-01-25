#include "statistics.hh"
#include <QDebug>

Statistics::Statistics() :
    points_()
{
    
}

Statistics::~Statistics()
{

}

void Statistics::passengerDied(int num)
{
    points_ += num;
    pointReview();
}

void Statistics::pointReview()
{
    if ( points_ > 50)
    {
        points_ = 20;
    }
    if ( points_ == 50 )
    {
        emit won();
    }
    emit addPoints(points_);
}

void Statistics::setNameAndTime(std::string name, int time)
{
    name_ = name;
    time_ = time;
}

void Statistics::readOldTopList()
{
    std::ifstream read_file_object(filename_);
    std::string row;

    while (getline(read_file_object, row, ';'))
    {
        std::string::size_type index = row.find(',');
        std::string first = row.substr(0,index);
        std::string name = row.substr(index +1);
        QString firstQString = QString::fromStdString(first);
        int time = firstQString.toInt();
        info_.insert({time, name});
        times_.push_back(time);
    }
    read_file_object.clear();
    read_file_object.close();

    emit topList(info_, times_);
}

void Statistics::updateTopList()
{
    std::ofstream write_file_object(filename_);
    info_.insert({time_, name_});
    times_.push_back(time_);
    times_.sort();
    int number;

    if (times_.size() > 10)
    {
        number = 10;
    }
    else
    {
        number = times_.size();
    }

    for (int t : times_)
    {
        if ( t > 0)
        {
            if(number > 0)
            {
                std::string time = std::to_string(t);
                std::string name = info_.at(t);
                write_file_object << time << ',' << name << ';' << std::endl;
            }
            --number;
        }
    }
    write_file_object.close();
}

void Statistics::douplePoints()
{
    points_ = 2*points_;
    pointReview();
}

void Statistics::setPoints(int num)
{
    points_ = num;
}

int Statistics::givePoints()
{
    return points_;
}

void Statistics::setFilename(std::string filename)
{
    filename_ = filename;
}

std::map<int, std::string> Statistics::giveInfo()
{
    return info_;
}

std::list<int> Statistics::giveTimes()
{
    return times_;
}

std::string Statistics::giveFilename()
{
    return filename_;
}

std::string Statistics::giveName()
{
    return name_;
}

int Statistics::giveTime()
{
    return time_;
}


