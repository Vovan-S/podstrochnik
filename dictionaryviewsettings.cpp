#include "dictionaryviewsettings.h"

DictionaryViewSettings::DictionaryViewSettings() : m_wordtypes(Word::typeCount, QString()), m_languages(Dictionary::languageCount, QString())
{
    m_wordtypes[Word::Verb]     = "Гл.";
    m_wordtypes[Word::Noun]     = "Сущ.";
    m_wordtypes[Word::Adjective]= "Прил.";
    m_wordtypes[Word::Adverb]   = "Нар.";
    m_wordtypes[Word::Pronoun]  = "Мест.";
    m_wordtypes[Word::Particle] = "Част.";
    m_wordtypes[Word::Idiom]    = "Уст.";

    m_languages[Dictionary::English]    = "English";
    m_languages[Dictionary::Latin]      = "Latin";
    m_languages[Dictionary::Deutsch]    = "Deutsch";

    m_wordfont  = QFont("Arial", 16, QFont::Bold);
    m_trfont    = QFont("Arial", 12);
    m_typefont    = QFont("Arial", 12, -1, true);

    m_linelenght = 60;
    m_betweenwords = 8;
    m_betweentranslations = 3;
}

const QVector<QString> &DictionaryViewSettings::GetWordTypes() const
{
    return m_wordtypes;
}

const QString &DictionaryViewSettings::GetWordType(Word::WordType t) const
{
    return m_wordtypes.at(t);
}

const QVector<QString> &DictionaryViewSettings::GetLanguages() const
{
    return m_languages;
}

const QString &DictionaryViewSettings::GetLanguage(Dictionary::Language l) const
{
    return m_languages.at(l);
}

void DictionaryViewSettings::SetWordTypes(const QVector<QString> &v)
{
    m_wordtypes = v;
}

void DictionaryViewSettings::SetWordType(Word::WordType t, const QString &s)
{
    m_wordtypes[t] = s;
}

const QFont &DictionaryViewSettings::GetWordFont() const
{
    return m_wordfont;
}

void DictionaryViewSettings::SetWordFont(const QFont &f)
{
    m_wordfont = f;
}

const QFont &DictionaryViewSettings::GetTranslationFont() const
{
    return m_trfont;
}

void DictionaryViewSettings::SetTranslationFont(const QFont &f)
{
    m_trfont = f;
}

const QFont &DictionaryViewSettings::GetTypeFont() const
{
    return m_typefont;
}

void DictionaryViewSettings::SetTypeFont(const QFont &f)
{
    m_typefont = f;
}

DictionaryViewSettings::ListType DictionaryViewSettings::GetListType() const
{
    return m_listtype;
}

void DictionaryViewSettings::SetListType(DictionaryViewSettings::ListType l)
{
    m_listtype = l;
}

int DictionaryViewSettings::GetLineLenght() const
{
    return m_linelenght;
}

void DictionaryViewSettings::SetLineLenght(int n)
{
    m_linelenght = n;
}

int DictionaryViewSettings::GetBetweenTranslations() const
{
    return m_betweenwords;
}

void DictionaryViewSettings::SetBetweenTranslations(int n)
{
    m_betweentranslations = n;
}

int DictionaryViewSettings::GetBetweenWords() const
{
    return m_betweenwords;
}

void DictionaryViewSettings::SetBetweenWords(int n)
{
    m_betweenwords = n;
}
