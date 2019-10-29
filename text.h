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
        Paragraph, //default section
        SubChapter,
        Chapter,
        Part
    };
    //мб замени, как будет удобнее, было бы логично первым элементом листа иметь заголовок,
    //если section type не paragraph
    typedef QList<QPair<SectionType,QStringList>>  TextContainer;
private:
    QString fileName;
    TextContainer text;
public:
    Text();

    //max lenght of one line in characters
    int GetLenght() const;
    void SetLenght(int);

    //saves in file or reads from file
    void SetFileName(const QString&);
    const QString& GetFileName() const;
    void WriteFile() const;
    void ReadFile();

    //text without any sections
    void SetPlainText(const QStringList&);

    //accessors
    void SetText(const TextContainer&);
    const TextContainer& GetText() const;
    const QString& GetLine(int) const;
    void SetLine(int, const QString&);
    QString& rLine(int);
    //caption of paragraph is its number
    QStringList& rSection(SectionType, const QString& caption);
    const QStringList& GetSection(SectionType, const QString& caption) const;
    //replacing text of one section
    void SetSection(SectionType,const QString& caption,const QStringList& text);
    void SetCaption(SectionType, const QString& old_caption, const QString& new_caption);
    //caption of n-th section
    const QString& GetCaption(int) const;
    //caption of n-th section of this type
    const QString& GetCaption(SectionType, int) const;

    //dividing text in sections: breaks line with number "line" at "pos" char
    //starts new section of given type with given caption with rest of line
    void Divide(int line, int pos, SectionType, const QString& caption);

    void LineBreak(int line, int pos);

    void DeleteSection(SectionType, const QString& caprion);
    void DeleteString(int);
 };

#endif // TEXT_H
