#ifndef WORDEDIT_H
#define WORDEDIT_H

#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QList>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>

#include "word.h"
#include "dictionaryviewsettings.h"

class WordEdit : public QWidget
{
    Q_OBJECT

    DictionaryViewSettings m_settings;

    QVBoxLayout * ptr_layout;
    QHBoxLayout * ptr_buttonlayout;

    QLineEdit * ptr_word;
    QPushButton * ptr_save;
    QPushButton * ptr_cancel;
    QPushButton * ptr_add;

    bool f_changed;

    static const int indent = 3;
    static const int defaultWidth = 400;

    static const int buttonStretch = 15;
    static const int buttonSpacing = 20;

    static const int buttonW = 30;
    static const int buttonH = 30;

    //adds new group or line to their layout
    // !!! should be used after all buttons under groups or lines are added and
    //layout is set
    void newGroup(Word::WordType, const QStringList&);
    void newLine(QWidget *, const QString&);

    void renameGroups(int startWith);
    void renameLines(QWidget *, int startWith);

    bool isEmpty();
    bool isEmpty(QWidget *);
    bool isEmpty(QWidget *, int line);
    bool checkStrings();
private slots:
    void newEmptyGroup();
    void newEmptyLine();
    void deleteLine();
    void deleteGroup();
    void changed();
    void save();

protected:
    void closeEvent(QCloseEvent * event) override;

public:
    explicit WordEdit(QWidget * parent = nullptr);
    WordEdit(const Word * w, const DictionaryViewSettings * s, QWidget * parent = nullptr);

    Word getSavedWord() const;

signals:
    void Saved();
    void Closed();

};

#endif // WORDEDIT_H
