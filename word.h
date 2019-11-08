#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QTextStream>



//word contained in dictionary
class Word
{
public:
    //parts of speech
    enum WordType {
        Verb,
        Noun,
        Adjective,
        Adverb,
        Pronoun,
        Particle,
        Idiom
    };
    static const int typeCount = 7;
    //if a traslation for specific part of speech is required but
    //there is no such translation, this exception is thrown
    class NoTranslation{};

    typedef QList<QPair<QStringList,WordType>> container_t;
    typedef QPair<QStringList,WordType>        container_value_t;

    static const QString wordSeparator;
    static const QString typeSeparator;
    static const QString translationSeparator;

    static bool hasSeparators(const QString&);
private:
    //word for user to see
    QString m_word;
    //key for finding and comparing words
    QString m_key;
    //translations of word
    container_t m_translations;
public:
    Word();
    Word(const QString& word, const QString& key, const container_t& translations);

    //accessors

    void SetWord(const QString&);
    const QString& GetWord() const;
    void SetKey(const QString&);
    const QString& GetKey() const;
    void SetTranslations(const container_t&);
    const container_t& GetTranslations() const;
        //can throw NoTranslation
    const QStringList& GetTranslation(WordType) const noexcept(false);


    //modifiers

    void AddTranslation(const QStringList& translation, WordType type);
    void AddTranslations(const container_t&);
        //deletes all translations with this type
    void DeleteTranslation(WordType);
        //deletes a specific translation
    void DeleteTranslation(WordType, const QString&);

    bool operator==(const Word&) const;
};

//format: "<key>__W:<word>(__Ty:<Type>(__Tr:<translation>)+)+"
QTextStream & operator<<(QTextStream&,const Word&);
QTextStream & operator>>(QTextStream&,Word&);

#endif // WORD_H
