#include "word.h"

const QString Word::wordSeparator        = "__W:";
const QString Word::typeSeparator        = "__Ty:";
const QString Word::translationSeparator = "__Tr:";


bool Word::hasSeparators(const QString &s)
{
    if (s.contains(wordSeparator) || s.contains(translationSeparator) || s.contains(typeSeparator))
        return true;
    else return false;
}

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

const QString &Word::GetWord() const
{
    return m_word;
}

void Word::SetKey(const QString &k)
{
    m_key = k;
}

const QString &Word::GetKey() const
{
    return m_key;
}

void Word::SetTranslations(const QList<QPair<QStringList, Word::WordType> > &t)
{
    m_translations = t;
}

const QList<QPair<QStringList, Word::WordType> >& Word::GetTranslations() const
{
    return m_translations;
}

const QStringList &Word::GetTranslation(Word::WordType type) const
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
               //there is such part of speech
               if(!(i->first.contains(s)))
                    i->first << s;
            }
            return;
        }
    //there is no such part of speech
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


QTextStream &operator<<(QTextStream & s, const Word & w)
{
    s << w.GetKey() << Word::wordSeparator << w.GetWord();
    for (auto i = w.GetTranslations().begin();i != w.GetTranslations().end();i++)
    {
        s << Word::typeSeparator << i->second << Word::translationSeparator;
        s << i->first.join(Word::translationSeparator);
    }
    s << "\n";
    return s;
}

QTextStream &operator>>(QTextStream &s, Word &w)
{
    QString str = s.readLine();
    //reads from lines chars from "from" to "end"
    int start = 0;
    int end = str.indexOf(Word::wordSeparator);
    w.SetKey(str.left(end));
    start = end + Word::wordSeparator.length();
    end = str.indexOf(Word::typeSeparator,start);
    w.SetWord(str.mid(start, end - start));
    Word::container_t list;
    while(end > 0)
    {
        start = end + Word::typeSeparator.length();
        end = str.indexOf(Word::translationSeparator,start);
        Word::WordType t = static_cast<Word::WordType>(str.mid(start,end-start).toInt());
        start = end + Word::translationSeparator.length();
        end = str.indexOf(Word::typeSeparator,start);
        if (end < 0)
            list << Word::container_value_t(str.mid(start).split(Word::translationSeparator),t);
        else
            list << Word::container_value_t(str.mid(start, end - start).split(Word::translationSeparator),t);
    }
    w.SetTranslations(list);
    return s;
}
