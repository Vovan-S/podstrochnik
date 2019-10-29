#include "word.h"

Word::Word()
{

}

Word::Word(const QString &word, const QString &key, const QList<QPair<QStringList, Word::WordType> > &translations):
    m_word(word), m_key(key), m_translations(translations)
{
}

void Word::SetWord(const QString &w)
{
    m_word = w;
}

QString Word::GetWord() const
{
    return m_word;
}

void Word::SetKey(const QString &k)
{
    m_key = k;
}

QString Word::GetKey() const
{
    return m_key;
}

void Word::SetTranslations(const QList<QPair<QStringList, Word::WordType> > &t)
{
    m_translations = t;
}

QList<QPair<QStringList, Word::WordType> > Word::GetTranslations() const
{
    return m_translations;
}

QStringList Word::GetTranslation(Word::WordType type) const
{
    for (auto i = m_translations.begin();i != m_translations.end();i++)
        if (i->second == type)
            return i->first;
    throw Word::NoTranslation();
}

void Word::AddTranslation(const QStringList &translation, Word::WordType type)
{
    for (auto i = m_translations.begin();i != m_translations.end();i++)
        if (i->second == type)
        {
            foreach (QString s, translation) {
               //если такого перевода еще нет
               if(!(i->first.contains(s)))
                    i->first << s;
            }
            return;
        }
    //такой части речи еще нет
    m_translations << QPair<QStringList, Word::WordType>(translation,type);
}

void Word::AddTranslations(const QList<QPair<QStringList, Word::WordType> > &t)
{
    for (auto i = t.begin(); i != t.end(); i++)
        AddTranslation(i->first,i->second);
}

void Word::DeleteTranslation(Word::WordType type)
{
    for (auto i = m_translations.begin();i != m_translations.end();i++)
        if (i->second == type)
        {
            m_translations.erase(i);
            return;
        }
}

void Word::DeleteTranslation(Word::WordType type, const QString &s)
{
    for (auto i = m_translations.begin();i != m_translations.end();i++)
        if (i->second == type)
        {
            i->first.removeOne(s);
            return;
        }
}

bool Word::operator==(const Word &w) const
{
    if (m_word != w.m_word || m_key != w.m_word || m_translations != w.m_translations)
        return false;
    else
        return true;
}

