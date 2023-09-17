/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

UserIO Pin Data

***************************************************************************/
#ifndef PINDATA_H
#define PINDATA_H

class PinData
{
private:
    int  m_Function;
    bool m_ActiveStatus;
    bool m_CheckStatus;
public:
    int getFunction() const;
    void setFunction(int a_Function);
    bool getActiveStatus() const;
    void setActiveStatus(bool a_ActiveStatus);
    bool getCheckStatus() const;
    void setCheckStatus(bool a_ChechStatus);
};

#endif // PINDATA_H
