#include "wordedit.h"

//use in form <ptrname>-><macrosname>
#define GroupBox(n)             itemAt(n + 1)->widget() //<ptrname> = ptr_layout
#define ComboBox()              layout()->itemAt(0)->layout()->itemAt(0)->widget()  //object - groupbox
#define DeleteAllButton()       layout()->itemAt(0)->layout()->itemAt(1)->widget()  //object - groupbox
#define LineEdit(n)             layout()->itemAt((n)+1)->layout()->itemAt(0)->widget()  //object - groupbox
#define DeleteLineButton(n)     layout()->itemAt((n)+1)->layout()->itemAt(1)->widget()  //object - groupbox

//use in form <macrosname>(<ptrname>)
#define GetNewLineButton(ptr)   (ptr)->layout()->itemAt((ptr)->layout()->count()-1)->widget() //object - groupbox
#define GetNewGroupButton(ptr)  (ptr)->itemAt((ptr)->layout()->count()-2)->widget()           //ptr - ptr_layout


void WordEdit::newGroup(Word::WordType t, const QStringList &l)
{
    int n = ptr_layout->count() - 3;    //number of groupbox

    QGroupBox * group = new QGroupBox;
    group->setObjectName(QString::number(n));

    QVBoxLayout * vlayout = new QVBoxLayout(group);

    QHBoxLayout * combolayout = new QHBoxLayout;

    QComboBox * comb = new QComboBox;
    comb->addItems(m_settings.GetWordTypes().toList());
    comb->setCurrentIndex(t);
    combolayout->addWidget(comb, buttonStretch);
    combolayout->addSpacing(buttonSpacing);

    //TODO: иконки для удаления и добавления
    QPushButton * deleteAll = new QPushButton;
    deleteAll->setObjectName(group->objectName());
    combolayout->addWidget(deleteAll);

    vlayout->addLayout(combolayout);

    QPushButton * addLine = new QPushButton;
    addLine->setObjectName(group->objectName());
    vlayout->addWidget(addLine);


    if (l.empty())  //this is a new group
        newLine(group,  QString()); //empty line
    else
        foreach(QString s, l)
            newLine(group, s);

    connect(deleteAll,  SIGNAL(released()),this,    SLOT(deleteGroup()));
    connect(addLine,    SIGNAL(released()),this,    SLOT(newEmptyLine()));
    connect(comb,       SIGNAL(currentIndexChanged(int)), this, SLOT(changed()));

    ptr_layout->insertWidget(ptr_layout->count()-2, group);
    qDebug() << "group added: " << group->objectName();
}

void WordEdit::newLine(QWidget *g, const QString &s)
{
    int n = g->layout()->count() - 2;       //number of line in group
    QHBoxLayout * layout = new QHBoxLayout;

    QLineEdit * line = new QLineEdit;
    line->setText(s);
    layout->addWidget(line);
    layout->addSpacing(buttonSpacing);

    //TODO: иконка удаления
    QPushButton * del = new QPushButton;
    del->setObjectName(g->objectName() + " " + QString::number(n));
    layout->addWidget(del);

    connect(del, SIGNAL(released()),this,SLOT(deleteLine()));
    connect(line, SIGNAL(textChanged(const QString&)),this,SLOT(changed()));

    dynamic_cast<QVBoxLayout *>(g->layout())->insertLayout(n+1,layout);
}

void WordEdit::renameGroups(int startWith)
{
    if (ptr_layout->count() - 3 > startWith)   //there are groups below chosen group
    {
        QWidget * group = ptr_layout->GroupBox(startWith);
        qDebug() << "group renamed: " << group->objectName() << " -> " << startWith;
        //group name
        group->setObjectName(QString::number(startWith));
        //"group delete" button name
        group->DeleteAllButton()->setObjectName(QString::number(startWith));
        //"new line" button name
        GetNewLineButton(group)->setObjectName(QString::number(startWith));
        //lines and buttons names
        renameLines(group,0);
        //rename other groups
        renameGroups(startWith+1);
    }
}

void WordEdit::renameLines(QWidget *g, int startWith)
{
    if(g->layout()->count() - 2 > startWith)
    {
        //"delete line" button name
        g->DeleteLineButton(startWith)->setObjectName(g->objectName()+ " " + QString::number(startWith));
        //other lines
        renameLines(g, startWith+1);
    }
}

bool WordEdit::isEmpty()
{
    if (!ptr_word->text().isEmpty())
        return false;
    for (int i = 0; i < ptr_layout->count()-3; i++)
        if (!isEmpty(ptr_layout->GroupBox(i)))
            return false;
    return true;
}

bool WordEdit::isEmpty(QWidget * g)
{
    //lines are empty
    for (int i = 0; i < g->layout()->count() - 2; i++)
        if(!isEmpty(g,i))
            return false;
    return true;
}

bool WordEdit::isEmpty(QWidget *g, int line)
{
    auto le = dynamic_cast<QLineEdit *>(g->LineEdit(line));
    qDebug() << le;
    if (le != nullptr && !le->text().isEmpty())
    {
        qDebug() << le->text() << le->text().length();
        return false;
    }
    else return true;
}

bool WordEdit::checkStrings()
{
    QString str = ptr_word->text();
    bool bad = false;
    if (Word::hasSeparators(str)) bad = true;
    else
    {
        for(int i = 0;  !bad && i < ptr_layout->count() - 3; i++)
            for (int j = 0; j < ptr_layout->GroupBox(i)->layout()->count() - 2; j++)
            {
                QLineEdit * le = dynamic_cast<QLineEdit *>(ptr_layout->GroupBox(i)->LineEdit(j));
                if (le != nullptr && Word::hasSeparators(le->text()))
                {
                    str = le->text();
                    bad = true;
                    break;
                }
            }
    }
    if (bad)
        QMessageBox::critical(this,
                              tr("Ошибка сохранения!"),
                              tr("Введенная строка содержит запрещенный символ:\n") + str);
    return bad;
}


void WordEdit::newEmptyGroup()
{
    newGroup(static_cast<Word::WordType>(0),QStringList());
}

void WordEdit::newEmptyLine()
{
    int group = sender()->objectName().toInt() + 1;
    newLine(ptr_layout->itemAt(group)->widget(),QString());
}

void WordEdit::deleteLine()
{
    int space = sender()->objectName().indexOf(' ');
    int group = sender()->objectName().left(space).toInt();
    int line  = sender()->objectName().right(sender()->objectName().length() - space - 1).toInt();
    qDebug() << "group: " << group << "line: " << line;
    if (isEmpty(ptr_layout->GroupBox(group),line) || QMessageBox::Yes ==
                                                     QMessageBox::warning(this,
                                                     tr("Предупреждение"),
                                                     tr("Вы уверены, что хотите удалить этот перевод?"),
                                                     QMessageBox::Yes | QMessageBox::No,
                                                     QMessageBox::No)

       )
    {
        QLayoutItem * child = ptr_layout->GroupBox(group)->layout()->itemAt(line + 1)->layout()->takeAt(0);
        qDebug() << child->widget()->objectName();
        delete child->widget();
        child = ptr_layout->GroupBox(group)->layout()->itemAt(line + 1)->layout()->takeAt(1);
        qDebug() << child->widget();
        delete child->widget();
        delete ptr_layout->GroupBox(group)->layout()->takeAt(line + 1);
        if (ptr_layout->GroupBox(group)->layout()->count() == 2)
            newLine(ptr_layout->GroupBox(group),QString());
        else
            renameLines(ptr_layout->GroupBox(group),line);
        resize(width(),minimumHeight());
        f_changed = true;
    }
}

void WordEdit::deleteGroup()
{
    int group = sender()->objectName().toInt();
    if (isEmpty(ptr_layout->GroupBox(group)) || QMessageBox::Yes ==
                                               QMessageBox::warning(this,
                                               tr("Предупреждение"),
                                               tr("Вы уверены, что хотите удалить эти переводы?"),
                                               QMessageBox::Yes | QMessageBox::No,
                                               QMessageBox::No)
       )
    {
            delete ptr_layout->takeAt(group + 1)->widget();
            if (ptr_layout->count() == 3)
                newEmptyGroup();
            else
                renameGroups(group);
            resize(width(), minimumHeight());
            f_changed = true;
    }
}

void WordEdit::changed()
{
    f_changed = true;
}

void WordEdit::save()
{
    if(checkStrings()) return;
    //let wordview to close window
    f_changed = false;
    emit Saved();
}

void WordEdit::closeEvent(QCloseEvent *event)
{
    if (f_changed && !isEmpty())
    {
        int res = QMessageBox::warning(this,
                                       tr("Предупреждение"),
                                       tr("Изменения не будут сохранены!\nХотите сохранить слово?"),
                                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                       QMessageBox::Save);
        qDebug() << res;
        if (res == QMessageBox::Save)
        {
            if(checkStrings())
            {
                event->ignore();
                return;
            }
            //signal sent to wordview, wordview will get the word and then close the window
            f_changed = false;
            emit Saved();
            event->ignore();
            return;
        }
        if (res == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }
    emit Closed();
    QWidget::closeEvent(event);
}

WordEdit::WordEdit(QWidget *parent) : QWidget(parent), m_settings(), f_changed(false)
{
    setMinimumWidth(defaultWidth);

    ptr_layout = new QVBoxLayout(this);

    ptr_word = new QLineEdit;
    ptr_word->setFont(m_settings.GetTranslationFont());
    ptr_layout->addWidget(ptr_word);

    ptr_add = new QPushButton(tr("Добавить"));
    ptr_layout->addWidget(ptr_add, 3);

    ptr_buttonlayout = new QHBoxLayout;

    ptr_save = new QPushButton(tr("Сохранить"));
    ptr_cancel = new QPushButton(tr("Отмена"));
    ptr_buttonlayout->addWidget(ptr_save);
    ptr_buttonlayout->addWidget(ptr_cancel);
    ptr_layout->addLayout(ptr_buttonlayout);

    newEmptyGroup();

    connect(ptr_save,SIGNAL(released()),this,SLOT(save()));
    connect(ptr_cancel,SIGNAL(released()),this,SLOT(close()));
    connect(ptr_add,SIGNAL(released()),this,SLOT(newEmptyGroup()));
    connect(ptr_word, SIGNAL(textChanged(const QString&)),this,SLOT(changed()));
}

WordEdit::WordEdit(const Word *w, const DictionaryViewSettings * s, QWidget *parent) : QWidget(parent), m_settings(*s), f_changed(false)
{
    setMinimumWidth(defaultWidth);

    ptr_layout = new QVBoxLayout(this);

    ptr_word = new QLineEdit;
    ptr_word->setText(w->GetWord());
    ptr_word->setFont(s->GetTranslationFont());
    ptr_layout->addWidget(ptr_word);

    ptr_add = new QPushButton(tr("Добавить"));
    ptr_layout->addWidget(ptr_add, 3);

    ptr_buttonlayout = new QHBoxLayout;

    ptr_save = new QPushButton(tr("Сохранить"));
    ptr_cancel = new QPushButton(tr("Отмена"));
    ptr_buttonlayout->addWidget(ptr_save);
    ptr_buttonlayout->addWidget(ptr_cancel);
    ptr_layout->addLayout(ptr_buttonlayout);

    for (auto i = w->GetTranslations().begin(); i != w->GetTranslations().end(); i++)
        newGroup(i->second,i->first);

    connect(ptr_save,SIGNAL(released()),this,SLOT(save()));
    connect(ptr_cancel,SIGNAL(released()),this,SLOT(close()));
    connect(ptr_add,SIGNAL(released()),this,SLOT(newEmptyGroup()));
    connect(ptr_word, SIGNAL(textChanged(const QString&)),this,SLOT(changed()));
}

Word WordEdit::getSavedWord() const
{
    Word res;
    res.SetWord(ptr_word->text());
    Word::container_t list;
    for (int i = 0; i < ptr_layout->count() - 3; i++)
    {
        auto group = ptr_layout->GroupBox(i);
        auto comb = dynamic_cast<QComboBox *>(group->ComboBox());
        if (comb == nullptr) continue;
        QStringList sl;
        for (int j = 0; j < group->layout()->count() - 2; j++)
        {
            auto line = dynamic_cast<QLineEdit *>(group->LineEdit(j));
            if (line != nullptr)
                sl << line->text();
        }
        list << Word::container_value_t(sl,static_cast<Word::WordType>(comb->currentIndex()));
    }
    res.SetTranslations(list);
    return res;
}
