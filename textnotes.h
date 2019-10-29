#ifndef TEXTNOTES_H
#define TEXTNOTES_H

#include <QMap>
#include <QPair>
#include <QString>

#include "note.h"

//notes for text
class TextNotes
{
    //translation for word from text, associated with pair<number of line, number of word>
    QMap<QPair<int,int>,Note> m_words;
    //translation for one line
    QMap<int,QString> m_translation;
    //general notes
    QString m_notes;
public:
    TextNotes();

    //accessors
    void SetWord(const QString&);
};

#endif // TEXTNOTES_H
