/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Custom validator

***************************************************************************/
#ifndef CUSTOMVALIDATOR_H
#define CUSTOMVALIDATOR_H

#include <QDoubleValidator>

class CustomValidator : public QDoubleValidator
{
public:
    CustomValidator (QObject * parent = 0) : QDoubleValidator(parent) {}
    CustomValidator (double bottom, double top, int decimals, QObject * parent) :
    QDoubleValidator(bottom, top, decimals, parent) {}

    QValidator::State validate(QString &s, int &pos) const
    {
        Q_UNUSED(pos);
        if (s.isEmpty() || s.startsWith("-"))
        {
            // allow empty field or minus sign
            return QValidator::Intermediate;
        }
        // check length of decimal places
        QChar point = locale().decimalPoint();
        if(s.indexOf(point) != -1)
        {
            int lengthDecimals = s.length() - s.indexOf(point) - 1;
            if (lengthDecimals > decimals()) {
                return QValidator::Invalid;
            }
        }
        // check range of value
        bool isNumber;
        double value = locale().toDouble(s, &isNumber);
        if (isNumber && bottom() <= value && value <= top())
        {
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }

};

#endif // CUSTOMVALIDATOR_H
