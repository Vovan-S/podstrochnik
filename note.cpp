#include "note.h"

Note::Note()
{

}

Note::Note(const QString &word, const QString &id, Word::WordType type, const QString &best, const QString &notes) :
    m_word(word), m_id(id),m_type(type), m_best(best), m_notes(notes)
{

}

void Note::SetWord(const QString &s)
{
    m_word = s;
}

QString Note::GetWord() const
{
    return m_word;
}

void Note::SetId(const QString &s)
{
    m_id = s;
}

QString Note::GetId() const
{
    return m_id;
}

void Note::SetType(Word::WordType t)
{
    m_type = t;
}

Word::WordType Note::GetType() const
{
    return m_type;
}

void Note::SetBest(const QString &s)
{
    m_best = s;
}

QString Note::GetBest() const
{
    return m_best;
}

void Note::SetNotes(const QString &s)
{
    m_notes = s;
}

QString Note::GetNotes() const
{
    return m_notes;
}

bool Note::operator==(const Note &n) const
{
    if (m_type == n.m_type && m_word == n.m_word && m_id == n.m_id && m_best == n.m_best && m_notes == n.m_best)
        return true;
    else
        return false;
}

bool Note::CompareWord(const Note &n) const
{
    if (m_word == n.m_word && m_id == n.m_id)
        return true;
    else
        return false;
}
