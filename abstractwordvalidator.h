#ifndef ABSTRACTWORDVALIDATOR_H
#define ABSTRACTWORDVALIDATOR_H

#include <QString>

//validator checks matching of word and key
//for different languages realisation should be different
class AbstractWordValidator
{
public:
    AbstractWordValidator();
    virtual ~AbstractWordValidator();
    virtual bool Validate(QString key, QString word) = 0;
};

//simplest validator
class EmptyValidator: public AbstractWordValidator
{
public:
    EmptyValidator();
    bool Validate(QString key, QString word) override
    {
        return word.contains(key);
    }
};

#endif // ABSTRACTWORDVALIDATOR_H
