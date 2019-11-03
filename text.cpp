#include <QDebug>

#include "text.h"
#include "Exceptions.h"


int Text::GetLength() const
{
  return lineLength;
}


void Text::SetLength( int linelength )
{
  lineLength = linelength;
}


void Text::SetFileName( const QString& filename )
{
  fileName = filename;
}


const QString& Text::GetFileName() const
{
  return fileName;
}


// formats the given text (list of strings) according to the set `lineLength`
void Text::SetPlainText( const QStringList& stringlist )
{
  QStringList text;
  QString string{};
  QRegExp re( R"(\w*[\.?!,]\n+)" ); // "word + any_of(.,?!) + \n{1, }"
  int amount;
  bool flag, flag1;

  for (int i = 0; i < stringlist.size(); i++)
  {
    text = stringlist[i].split( QRegExp( "\\t+| +" ) ); // splits by tabs and spaces to keep the dialogues seem like dialogues
    for (auto it = text.begin(); it != text.end(); it++)
    {
      amount = 0;

      if (it->trimmed().length() >= lineLength)
        throw LengthExc();

      if (flag = re.exactMatch( *it ))
        amount = it->count( '\n' );

      if (flag1 = it->length() - amount + string.length() >= lineLength)
        it--;
      else
      {
        *it = it->trimmed().remove('\n');
        string += ' ' + *it;
      }

      if (flag || flag1 || it == text.end() - 1)
      {
        string += '\n';
        plainText.append( string.remove( 0, 1 ) );
        string.clear();
      }

      for (int i = 0; i < amount - 1; i++)
        plainText.append( "\n" );
    }
  }

  //qDebug() << "After:";
  //for (auto it = plainText.begin(); it != plainText.end(); it++)
  //{
  //  qDebug() << *it;
  //}
}


void Text::DeleteString( int line )
{
  if (plainText.size() < line)
    throw OutOfBoundaries();
  plainText.removeAt( line );
}


void Text::WriteFile() const
{
  QFile file( fileName );

  if (!file.open( QIODevice::WriteOnly | QIODevice::Text ))
    throw FileNotOpened();

  QTextStream in( &file );

  for (auto& it = plainText.begin(); it != plainText.end(); it++)
    in << *it;

  file.close();
}


void Text::ReadFile()
{
  QFile file( fileName );

  if (!file.open( QIODevice::ReadOnly | QIODevice::Text ))
    throw FileNotOpened();

  QTextStream out( &file );

  while (!out.atEnd())
    plainText.append( out.readLine() );

  file.close();
}


bool Text::operator==( const Text& text ) const
{
  return this->plainText == text.plainText;
}


void Text::operator=( const Text& text )
{
  this->plainText = text.plainText;
}


const QString& Text::GetLine( int line ) const
{
  if (plainText.size() < line)
    throw OutOfBoundaries();
  return plainText.at( line );
}


void Text::SetLine( int line, const QString& string )
{
  if (plainText.size() < line)
    throw OutOfBoundaries();
  plainText.replace( line, string );
}


QString& Text::rLine( int line )
{
  if (plainText.size() < line)
    throw OutOfBoundaries();
  return plainText[line];
}