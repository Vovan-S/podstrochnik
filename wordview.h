#ifndef WORDVIEW_H
#define WORDVIEW_H

#include <QFrame>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QFontMetrics>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "word.h"
#include "dictionaryviewsettings.h"
#include "wordedit.h"


class WordView : public QFrame
{
    Q_OBJECT
public:
    enum Mode{
      Full,
      Small,
      Edit
    };
private:
    DictionaryViewSettings * ptr_settings;
    Word * ptr_model;

    QTextEdit * ptr_wordtext;
    QTextEdit * ptr_translations;
    QLineEdit * ptr_smallword;
    QLineEdit * ptr_smalltranslation;
    QPushButton * ptr_buttonedit;
    QPushButton * ptr_buttonshow;

    WordEdit * ptr_wordedit;

    Mode m_mode;

    int m_width;
    int full_height;
    int small_height;

    bool f_locked;

    static const int buttonSize = 25;
    static const int indent = 3;
    static const int listCaption = 20;
    static const int listItem = 5;


    //load string into edits
    void fillTranslations();
    void fillSmallTranslations();
    void fillSmallWord();
    //for resizing the edits
    int translationsHeight();

    void resizeFields();
public:
    explicit WordView(QWidget *parent = nullptr);
    WordView(Word * model, DictionaryViewSettings * setting, QWidget * parent = nullptr);



signals:
    void WordEdited();

private slots:
    void Unlock();

public slots:
    void editButtonClicked();
    void showButtonClicked();
    void SetMode(Mode);
    void Save();
    void Lock(bool);


};

#endif // WORDVIEW_H
