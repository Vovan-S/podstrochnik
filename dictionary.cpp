#include "dictionary.h"

QString Dictionary::generateId(const Word &w) const
{
    QString id = QString::number(m_lng)+w.GetKey();
    for (auto i = w.GetTranslations().begin(); i!=w.GetTranslations().end();i++)
        id+=QString::number(i->second);
    return id;
}

Dictionary::Dictionary(Language lng) : m_map(), m_lng(lng)
{
    //выбираем реализованный для языка валидатор
    switch (lng) {
    default: m_val = new EmptyValidator();
    }
}

void Dictionary::SetLanguage(Dictionary::Language l)
{
    m_lng = l;
}

Dictionary::Language Dictionary::GetLanguage() const
{
    return m_lng;
}

void Dictionary::SetMap(const QMap<QString, Word> &m)
{
    m_map = m;
}

const QMap<QString, Word> &Dictionary::GetMap() const
{
    return m_map;
}

const Word Dictionary::GetWord(const QString &s, Dictionary::InputType t) const
{
    return m_map.value(GetId(s,t));
}

QString Dictionary::GetId(const QString &s, Dictionary::InputType t) const
{
    switch (t) {
    case Id: if (m_map.contains(s)) return s;
        else throw NoWord();
    case Key:
    {
        for (auto i = m_map.begin();i != m_map.end(); i++)
            if(i->GetKey()==s)
                return i.key();
        throw NoWord();
    }
    case FromText:
    {
        for (auto i = m_map.begin();i != m_map.end();i++)
            if (m_val->Validate(i->GetKey(),s))
                return i.key();
        throw NoWord();
    }
    }
}

AbstractWordValidator* Dictionary::GetValidator() const
{
    return m_val;
}

QString Dictionary::AddWord(const Word &w)
{
    for(auto it = m_map.begin(); it!=m_map.end(); it++)
        if (it->GetKey() == w.GetKey())
        {
            it->AddTranslations(w.GetTranslations());
            return it.key();
        }
    //прошли и не нашли
    QString id = generateId(w);
    m_map[id] = w;
    return id;
}

QString Dictionary::AddWord(const QString &word, const QString &key, const QStringList &translations, Word::WordType type)
{
    return AddWord(Word(word, key, QList<QPair<QStringList,Word::WordType>>() << QPair<QStringList,Word::WordType>(translations,type)));
}

Word &Dictionary::rWord(const QString &s, Dictionary::InputType t)
{
    return m_map[GetId(s,t)];
}

Word &Dictionary::rWord(const Word &w)
{
    for (auto i = m_map.begin();i!=m_map.end();i++)
        if (i.value()==w)
            return i.value();
    throw NoWord();
}

void Dictionary::DeleteWord(const QString &s, Dictionary::InputType t)
{
    QString id;
    try {
        id = GetId(s,t);
    } catch (NoWord) {
        return;
    }
    m_map.erase(m_map.find(id));
}

void Dictionary::DeleteWord(const Word &w)
{
    for (auto i = m_map.begin();i!=m_map.end();i++)
        if (i.value()==w)
            m_map.erase(i);
}

void Dictionary::Clear()
{
    m_map.clear();
}
