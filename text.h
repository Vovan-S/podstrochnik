#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>

//"text" contains lines of text, divided in diffenrent section
//all the lines should be limited to a number of characters
class Text
{
public:
  enum SectionType
  {
    Paragraph, // default section
    SubChapter, // *** text ***
    Chapter,
    Part // bunch of chapters
  };

  typedef QPair<SectionType, QList<QStringList>> TextContainer;

private:
  QStringList plainText;
  QString fileName;
  TextContainer text;
  int lineLength;
  bool sectioned = false; // whether the text is divided into sections

public:
  Text() {};

  //max lenght of one line in characters
  int GetLength() const;
  void SetLength( int );

  //saves in file or reads from file
  void SetFileName( const QString& );
  const QString& GetFileName() const;
  void WriteFile() const noexcept(false);
  void ReadFile() noexcept(false);

  //text without any sections
  void SetPlainText( const QStringList& ) noexcept(false); // LengthExc

  //accessors
  void SetText( const TextContainer& container );
  const TextContainer& GetText() const;
  const QString& GetLine( int line ) const noexcept(false); // OutOfBoundaries
  void SetLine( int line, const QString& ) noexcept(false); // OutOfBoundaries
  QString& rLine( int line ) noexcept(false);  // OutOfBoundaries

  //caption of paragraph is its number
  QStringList& rSection( SectionType, const QString& );
  const QStringList& GetSection( SectionType, const QString& ) const;

  //replacing text of one section
  void SetSection( SectionType, const QString&, const QStringList& );
  void SetCaption( SectionType, const QString&, const QString& );

  //caption of n-th section
  const QString& GetCaption( int ) const;

  //caption of n-th section of this type
  const QString& GetCaption( SectionType, int ) const;

  //dividing text in sections: breaks line with number "line" at "pos" char
  //starts new section of given type with given caption with rest of line
  void Divide( int line, int pos, SectionType, const QString& caption );
  void LineBreak( int line, int pos );
  void DeleteSection( SectionType section, const QString& caption );
  void DeleteString( int line ) noexcept(false); // OutOfBoundaries


  bool operator==( const Text& text ) const;
  void operator=( const Text& text );

};

#endif // TEXT_H
