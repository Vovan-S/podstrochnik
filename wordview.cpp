#include "wordview.h"

void WordView::Save()
{
    Word edited = ptr_wordedit->getSavedWord();
    ptr_model->SetWord(edited.GetWord());
    ptr_model->SetTranslations(edited.GetTranslations());

    ptr_wordtext->setText(ptr_model->GetWord());
    fillTranslations();
    fillSmallWord();
    fillSmallTranslations();

    ptr_wordedit->close();
    delete ptr_wordedit;
    resizeFields();
    f_locked = false;
}

void WordView::Lock(bool f)
{
    f_locked = f;
}

void WordView::fillTranslations()
{
    if (ptr_translations == nullptr) return;
    //using html notation to set up the tranlations
    QString trText;
    //tags for part of speech
    QString typeFontHtml = "<font SIZE = \"" + QString::number(ptr_settings->GetTypeFont().pointSize()/5 + 1)
            + "\" face = \"" + ptr_settings->GetTypeFont().family() + "\">";
    QString typeFontClose;
    if (ptr_settings->GetTypeFont().italic())
    {
        typeFontHtml+="<i>";
        typeFontClose = "</i></font>";
    }
    else
        typeFontClose = "</font>";
    //list type
    QChar listType;
    if (ptr_settings->GetListType() == DictionaryViewSettings::Ordered)
        listType = 'o';
    else
        listType = 'u';
    for (auto i = ptr_model->GetTranslations().begin(); i != ptr_model->GetTranslations().end(); i++)
    {
        trText += typeFontHtml + ptr_settings->GetWordType(i->second) + typeFontClose + "\n";
        trText += '<' + listType + "l>\n";
        foreach (QString s, i->first) {
            trText += "<li>" + s + "</li>\n";
        }
        trText += "</" + QString(listType) + "l>\n";
    }
    ptr_translations->setHtml(trText);

}

void WordView::fillSmallTranslations()
{
    if(ptr_smalltranslation == nullptr) return;
    QString smallTr;
    int smallTrLen = ptr_settings->GetLineLenght() * 2 / 3;
    bool first = true;
    for (auto i = ptr_model->GetTranslations().begin(); i != ptr_model->GetTranslations().end() && (smallTrLen > 0); i++)
        foreach(QString s, i->first) {
            if (s.length() + 2 < smallTrLen)
            {
                smallTr += s + "; ";
                smallTrLen -= s.length() + 2;
                first = false;
            }
            else
            {
                if(first)
                {
                    smallTr+= s.left(smallTrLen - 5) + ". . .";
                    smallTrLen = 0;
                    break;
                }
            }
        }
    ptr_smalltranslation->setText(smallTr);

}

void WordView::fillSmallWord()
{
    if (ptr_smallword == nullptr) return;
    QString smallWordText;
    if (ptr_settings->GetLineLenght() * 2 / 5 < ptr_model->GetWord().length())
        smallWordText = ptr_model->GetWord().left(ptr_settings->GetLineLenght() * 2 / 5 - 3) + "...";
    else
        smallWordText = ptr_model->GetWord();
    ptr_smallword->setText(smallWordText);
}

int WordView::translationsHeight()
{
    int trHeight = ptr_model->GetTranslations().size() * (QFontMetrics(ptr_settings->GetTypeFont()).lineSpacing() + listCaption);
    int translationsLines = 0;
    for (auto i = ptr_model->GetTranslations().begin(); i != ptr_model->GetTranslations().end(); i++)
        foreach(QString s, i->first){
            translationsLines += s.length() / ptr_settings->GetLineLenght() + 1;
        }
    trHeight += translationsLines * (QFontMetrics(ptr_settings->GetTranslationFont()).lineSpacing()+listItem);
    return trHeight;
}

void WordView::resizeFields()
{
    int wordH  = (ptr_model->GetWord().length() / ptr_settings->GetLineLenght() + 1) * QFontMetrics(ptr_settings->GetWordFont()).lineSpacing() + 10;
    int trH = translationsHeight();
    full_height = wordH + trH + 2 * indent;
    if(m_mode == Full) resize(m_width,full_height);
    ptr_wordtext->setGeometry(indent,
                              indent,
                              m_width - 3 * indent - 3 * buttonSize,
                              wordH
                               );
    ptr_translations->setGeometry(indent,
                                  ptr_wordtext->height() + 2 * indent,
                                  m_width - 2 * indent,
                                  trH);
}

WordView::WordView(QWidget *parent) : QFrame(parent), ptr_settings(nullptr),
    ptr_model(nullptr), ptr_wordtext(nullptr), ptr_translations(nullptr),
    ptr_smallword(nullptr), ptr_smalltranslation(nullptr), m_mode(Full), f_locked(false)
{

}

WordView::WordView(Word *model, DictionaryViewSettings *setting, QWidget *parent) :
    QFrame(parent), ptr_settings(setting), ptr_model(model), m_mode(Full), f_locked(false)
{
    //(1) evaluating properties of window
    m_width = 4 * indent + QFontMetrics(ptr_settings->GetWordFont()).averageCharWidth() * ptr_settings->GetLineLenght()
            + 2 * buttonSize;

    //palette to make background transparent
    QPalette transparentBG;
    transparentBG.setColor(QPalette::Active, QPalette::Base, QColor(255,255,255,0));
    transparentBG.setColor(QPalette::Inactive, QPalette::Base, QColor(255,255,255,0));
    //(2) set up textedit for main word
    ptr_wordtext = new QTextEdit(this);
    ptr_wordtext->setFont(ptr_settings->GetWordFont());
    ptr_wordtext->setText(ptr_model->GetWord());
    ptr_wordtext->setReadOnly(true);
    ptr_wordtext->setFrameStyle(QFrame::NoFrame);
    ptr_wordtext->setPalette(transparentBG);
    //(3) edit button
    ptr_buttonedit = new QPushButton(this);
    ptr_buttonedit->setGeometry(m_width - 2 * indent - 2 * buttonSize, indent, buttonSize, buttonSize);
    ptr_buttonedit->setIcon(QIcon(":/images/edit.png"));
    //(4) expand minimize button
    ptr_buttonshow = new QPushButton(this);
    ptr_buttonshow->setGeometry(m_width - indent - buttonSize, indent, buttonSize, buttonSize);
    ptr_buttonshow->setIcon(QIcon(":/images/minimize.png"));
    //(5) text edit for translations
    ptr_translations = new QTextEdit(this);
    ptr_translations->setFont(ptr_settings->GetTranslationFont());
    ptr_translations->setReadOnly(true);
    ptr_translations->setFrameStyle(QFrame::NoFrame);
    ptr_translations->setPalette(transparentBG);
    ptr_translations->setMaximumWidth(m_width - 2 * indent);
    fillTranslations();

    resizeFields();
    //

    //(6)
    ptr_smallword = new QLineEdit(this);
    ptr_smallword->setFont(ptr_settings->GetWordFont());
    ptr_smallword->setFrame(false);
    ptr_smallword->setPalette(transparentBG);
    ptr_smallword->setReadOnly(true);
    fillSmallWord();
    ptr_smallword->setGeometry(indent + 1, indent + 1,
                               (m_width - 3 * indent - 2 * buttonSize) * 2 / 5 + 40,
                               QFontMetrics(ptr_settings->GetWordFont()).lineSpacing() + 5);
    ptr_smallword->setVisible(false);

    //(7)
    ptr_smalltranslation = new QLineEdit(this);
    ptr_smalltranslation->setFont(ptr_settings->GetTranslationFont());
    ptr_smalltranslation->setFrame(false);
    ptr_smalltranslation->setPalette(transparentBG);
    ptr_smalltranslation->setReadOnly(true);
    fillSmallTranslations();
    ptr_smalltranslation->setGeometry(ptr_smallword->x() + ptr_smallword->width() + indent, ptr_smallword->y(),
                               ptr_buttonedit->x() - 2 * indent - ptr_smallword->x() - ptr_smallword->width(),
                               ptr_smallword->height());
    ptr_smalltranslation->setVisible(false);

    //
    small_height = ptr_smallword->y() + ptr_smallword->height();

    //( )
    connect(ptr_buttonedit, SIGNAL(clicked()),      this,   SLOT(editButtonClicked()));
    connect(ptr_buttonshow, SIGNAL(clicked()),      this,   SLOT(showButtonClicked()));
}

void WordView::Unlock()
{
    f_locked = false;
}


void WordView::editButtonClicked()
{
    if(f_locked) return;
    SetMode(Edit);
}

void WordView::showButtonClicked()
{
    if(f_locked) return;
    if(m_mode == Full)
        SetMode(Small);
    else if(m_mode == Small)
        SetMode(Full);
}

void WordView::SetMode(WordView::Mode m)
{
    switch (m) {
    case Full:
    {
        ptr_wordtext->setVisible(true);
        ptr_translations->setVisible(true);
        ptr_buttonedit->setVisible(true);
        ptr_buttonshow->setVisible(true);
        ptr_smallword->setVisible(false);
        ptr_smalltranslation->setVisible(false);
        resize(m_width,full_height);
        ptr_buttonshow->setIcon(QIcon(":/images/minimize.png"));

        ptr_wordtext->setReadOnly(true);
        ptr_translations->setReadOnly(true);

        m_mode = Full;

        break;
    }
    case Small:
    {
        ptr_wordtext->setVisible(false);
        ptr_translations->setVisible(false);
        ptr_buttonedit->setVisible(true);
        ptr_buttonshow->setVisible(true);
        ptr_smallword->setVisible(true);
        ptr_smalltranslation->setVisible(true);
        resize(m_width,small_height);
        ptr_buttonshow->setIcon(QIcon(":/images/show.png"));

        m_mode = Small;

        break;
    }
    case Edit:
    {
        ptr_wordedit = new WordEdit(ptr_model,ptr_settings);
        ptr_wordedit->show();
        connect(ptr_wordedit, SIGNAL(Saved()), this, SLOT(Save()));
        connect(ptr_wordedit, SIGNAL(Closed()),this, SLOT(Unlock()));
        f_locked = true;
        break;
    }
    }
}

