#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>

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
    //if a traslation for specific part of speech is required but
    //there is no such translation, this exception is thrown
    class NoTranslation{};
private:
    //word for user to see
    QString m_word;
    //key for finding and comparing words
    QString m_key;
    //translations of word
    QList<QPair<QStringList,WordType>> m_translations;
public:
    Word();
    Word(const QString& word, const QString& key, const QList<QPair<QStringList,WordType>>& translations);

    //accessors

    void SetWord(const QString&);
    QString GetWord() const;
    void SetKey(const QString&);
    QString GetKey() const;
    void SetTranslations(const QList<QPair<QStringList,WordType>>&);
    QList<QPair<QStringList,WordType>> GetTranslations() const;
        //can throw NoTranslation
    QStringList GetTranslation(WordType) const noexcept(false);


    //modifiers

    void AddTranslation(const QStringList& translation, WordType type);
    void AddTranslations(const QList<QPair<QStringList,WordType>>&);
        //deletes all translations with this type
    void DeleteTranslation(WordType);
        //deletes a specific translation
    void DeleteTranslation(WordType, const QString&);

    bool operator==(const Word&) const;
};

#endif // WORD_H
