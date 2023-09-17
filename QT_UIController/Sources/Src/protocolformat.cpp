/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2021

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.

 ---------------------------- MODULE DESCRIPTION ----------------------------

 Protocol Utility Module

 **********************************************************************************************************/

#include "Header/protocolformat.h"
/**************************************************************************//**
*
* \brief Constructor, Here we define all the function as the static
*       so the function can be run directly what is not owned by any object.
*       The class ProtocolFormat is the wrapper to limit the function can be used directly be the external.
*
* \param none
*
* \return ProtocolFormat object
*
******************************************************************************/
ProtocolFormat::ProtocolFormat()
{

}

/**************************************************************************//**
*
* \brief To build the request package following key and message
*
* \param key is the command id, message is what content HMI want to send to SC.
*        buff is the integrated package with crc32 checksum.
*
* \return result
*
******************************************************************************/
void ProtocolFormat::buildProtocolPackage(const int key, const QByteArray message, QByteArray &buff)
{
    const char* tmpBuffer;
    int offset = 0;
    buff.clear();
    offset = sizeof(int);
    tmpBuffer = reinterpret_cast<const char*>(&key);
    buff.append(tmpBuffer, offset);

    int dwLen = message.size();
    tmpBuffer = reinterpret_cast<const char*>(&dwLen);
    buff.append(tmpBuffer, offset);
    buff.append(message);
}

/**************************************************************************//**
*
* \brief To parse the response package that received from SC
*
* \param key is the command id, message is the data field of the response package.
*        buff is the received response package.
*
* \return If there is any issue during the parsing, it will return false.
*
******************************************************************************/
bool ProtocolFormat::parseProtocolPackage(int &key, QByteArray &message, const QByteArray buff)
{
    bool bResult = false;
    int offset = 0, dataLen = 0, position = 0;
    QByteArray tmpBuffer;
    key = 0;
    message.clear();

    offset = sizeof(unsigned int);
    if(position + offset > buff.size())
    {
        bResult = false;
        return bResult;
    }
    tmpBuffer = buff.left(offset);
    memcpy(&key, tmpBuffer.data(), offset);
    position += offset;

    offset = sizeof(unsigned int);
    if(position + offset > buff.size())
    {
        bResult = false;
        return bResult;
    }
    tmpBuffer = buff.mid(position, offset);
    memcpy(&dataLen, tmpBuffer.data(), offset);
    position += offset;

    offset = dataLen;
    if(position + offset > buff.size())
    {
        bResult = false;
        return bResult;
    }
    message = buff.mid(position, offset);
    return true;
}

/**************************************************************************//**
*
* \brief To check if there is a integrated package from the received data.
*
* \param sourceBuff is all received data from SC, onePackageBuff is the extracted integrated package.
*
* \return if there is a integrated package in the sourceBuff, it will return true, otherwise it will return false.
*
******************************************************************************/
bool ProtocolFormat::isFullProtocolPackage(const QByteArray sourceBuff, QByteArray &onePackageBuff)
{
    bool bResult = false;
    QByteArray tmpBuff;
    int position = 0, buffLen = sourceBuff.size();
    int offset = 0, command = 0, dataLen = 0;
    onePackageBuff.clear();

    // command id parsing
    offset = sizeof(command);
    if((position + offset) > buffLen)
        return false;
    tmpBuff = sourceBuff.left(offset);
    onePackageBuff.append(tmpBuff);
    memcpy(&command, tmpBuff.data(), offset);
    position += offset;

    // data length parsing
    offset = sizeof(dataLen);
    if((position + offset) > buffLen)
    {
        onePackageBuff.clear();
        return false;
    }
    tmpBuff = sourceBuff.mid(position, offset);
    onePackageBuff.append(tmpBuff);
    memcpy(&dataLen, tmpBuff.data(), offset);
    position += offset;

    offset = dataLen;
    if((position + offset) > buffLen)
    {
        onePackageBuff.clear();
        return false;
    }
    tmpBuff = sourceBuff.mid(position, offset);
    onePackageBuff.append(tmpBuff);
    bResult = true;
    return bResult;
}
