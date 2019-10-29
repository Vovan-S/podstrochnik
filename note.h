#ifndef NOTE_H
#define NOTE_H

#include <QString>

#include "word.h"

//note for word in text
class Note
{
    //form of word from text
    QString m_word;
    //id of associated word in dictionary
    QString m_id;
    //part of speech in context
    Word::WordType m_type;
    //best translation in context
    QString m_best;
    //additional comments
    QString m_notes;
public:
    Note();
    Note(const QString& word, const QString& id, Word::WordType type,  const QString& best, const QString& notes);

    //accessor
    void SetWord(const QString&);
    const QString& GetWord() const;
    void SetId(const QString&);
    const QString& GetId() const;
    void SetType(Word::WordType);
    Word::WordType GetType() const;
    void SetBest(const QString&);
    const QString& GetBest() const;
    void SetNotes(const QString&);
    const QString& GetNotes() const;

    bool operator==(const Note&) const;
    //compares only "field" word and "id"
    bool CompareWord(const Note&) const;
};

#endif // NOTE_H
