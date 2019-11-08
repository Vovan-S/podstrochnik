#ifndef DICTIONARYVIEWSETTINGS_H
#define DICTIONARYVIEWSETTINGS_H

#include <QString>
#include <QVector>
#include <QFont>
#include <QDebug>

#include "dictionary.h"

//contains setting for DictionaryView
class DictionaryViewSettings
{
public:
    //which list type will be used to display translations
    enum ListType
    {
        Ordered,
        Unordered
    };
private:
    //names of parts of speech in dictionary
    QVector<QString> m_wordtypes;
    QVector<QString> m_languages;

    //font for the main word
    QFont m_wordfont;
    //font for translations
    QFont m_trfont;
    //font for parts of speech
    QFont m_typefont;

    //list type
    ListType m_listtype;

    //metrics
    //in characters
    int m_linelenght;
    //in pixs
    int m_betweentranslations;
    int m_betweenwords;

public:
    DictionaryViewSettings();

    //accessors
    const QVector<QString>& GetWordTypes() const;
    const QString& GetWordType(Word::WordType) const;
    const QVector<QString>& GetLanguages() const;
    const QString& GetLanguage(Dictionary::Language) const;
    void SetWordTypes(const QVector<QString>&);
    void SetWordType(Word::WordType, const QString&);
    const QFont& GetWordFont() const;
    void SetWordFont(const QFont&);
    const QFont& GetTranslationFont() const;
    void SetTranslationFont(const QFont&);
    const QFont& GetTypeFont() const;
    void SetTypeFont(const QFont&);
    ListType GetListType() const;
    void SetListType(ListType);
    int GetLineLenght() const;
    void SetLineLenght(int);
    int GetBetweenTranslations() const;
    void SetBetweenTranslations(int);
    int GetBetweenWords() const;
    void SetBetweenWords(int);

};

#endif // DICTIONARYVIEWSETTINGS_H
