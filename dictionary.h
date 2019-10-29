#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QMap>
#include <QList>
#include <QPair>

#include "word.h"
#include "abstractwordvalidator.h"

//dictonary contains translations for all texts in one language
//every word has assosiated id
class Dictionary
{
public:
    enum Language{
        English,
        Deutsch,
        Latin
    };
    //how word will be searched for
    enum InputType{
        Id,         //individual code
        Key,        //key from "word" class
        FromText    //form from text, validator will be used
    };

    //exception, thrown if absent word is required
    class NoWord {};
private:
    //map of pairs<id, word>
    QMap<QString,Word> m_map;
    Language m_lng;
    AbstractWordValidator *m_val;

    //generate for word individual id
    QString generateId(const Word&) const;
public:
    Dictionary(Language);
    ~Dictionary();

    //аксессоры
    void SetLanguage(Language);
    Language GetLanguage() const;
    void SetMap(const QMap<QString,Word>&);
    QMap<QString,Word> GetMap() const;

    //can throw NoWord
    const Word GetWord(const QString&, InputType = Id) const noexcept(false);

    //returns id of word, if there is one, else throws NoWord
    QString GetId(const QString&,InputType = Id) const noexcept(false);

    //returns validator for language of dictionary
    AbstractWordValidator* GetValidator() const;

    //modifiers
        //returns id of added word
    QString AddWord(const Word&);
    QString AddWord(const QString& word, const QString &key, const QStringList& translations, Word::WordType type);
        //can throw NoWord
    Word& rWord(const QString&, InputType = Id) noexcept(false);
    Word& rWord(const Word&) noexcept(false);
    void DeleteWord(const QString&, InputType = Id);
    void DeleteWord(const Word&);
    void Clear();

};

#endif // DICTIONARY_H
