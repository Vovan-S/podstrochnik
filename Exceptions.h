#pragma once

#include <QDebug>


class LengthExc
{
public:
  LengthExc() { qDebug() << "The length of the word exceeded the chosen length of the line"; };
};


class OutOfBoundaries
{
public:
  OutOfBoundaries() { qDebug() << "Seems you didn't estimated quite well: you are out of boundaries"; }
};


class FileNotOpened
{
public:
  FileNotOpened() { qDebug() << "File wasn't opened"; }
};