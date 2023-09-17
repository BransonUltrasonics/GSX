/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

Trends Graph

***************************************************************************/
#include "Header/trendsgraph.h"
#include "Header/trendsgraphparametername.h"
#include <QQuickView>
#include <QQmlContext>
#include <algorithm>

using namespace std ;
QT_CHARTS_USE_NAMESPACE
extern CommunicationInterface CIObj;
extern QQmlContext*context;

/**
 * @brief TrendsGraph :constructor
 * @param QObject *ptr
 */
TrendsGraph::TrendsGraph(QObject *parent) : QObject(parent)
{}

/**
 * @brief TrendsGraph::getGraphValueByIndex : Get a Graph Value according to index
 * @param gIndex
 * @param typeIndex
 * @return
 */
QPointF TrendsGraph::getGraphValueByIndex(int gIndex,int typeIndex)
{
    switch (typeIndex)
    {
    case 0:
        return weldForceList.at(gIndex);
    case 1:
        return holdForceList.at(gIndex);
    case 2:
        return totalAbsoluteList.at(gIndex);
    case 3:
        return partContactPositionList.at(gIndex);
    case 4:
        return weldTimeList.at(gIndex);
    case 5:
        return weldEnergyList.at(gIndex);
    case 6:
        return weldPeakPowerList.at(gIndex);
    case 7:
        return startFrequencyList.at(gIndex);
    case 8:
        return frequencyChangeList.at(gIndex);
    default:
        break;
    }
    return QPointF(0,0);
}

/**
 * @brief TrendsGraph::trendsGraphData : Clear the list values
 * @param recipeNumber
 */
void TrendsGraph::trendsGraphData(QString recipeNumber)
{
    clearModel();
    if(weldForceList.size()>=0 && weldForceListYValue.size()>=0)
    {
        weldForceList.clear();
        holdForceList.clear();
        totalAbsoluteList.clear();
        partContactPositionList.clear();
        weldTimeList.clear();
        weldEnergyList.clear();
        weldPeakPowerList.clear();
        startFrequencyList.clear();
        frequencyChangeList.clear();
        weldForceListYValue.clear();
        holdForceListYValue.clear();
        totalAbsoluteListYValue.clear();
        partContactPositionListYValue.clear();
        weldTimeListYValue.clear();
        weldEnergyListYValue.clear();
        weldPeakPowerListYValue.clear();
        startFrequencyListYValue.clear();
        frequencyChangeListYValue.clear();
        cycleCounterList.clear();
        alarmFlagList.clear();
    }

    int i=getTrendsGraphSignatureData(recipeNumber);
    if(i==0)
    {
        getSuspectRejectParamLimits(recipeNumber);
        getCycleConterValues();
    }
    else{
        setXCount(0);
        setTickCountY(0);
        // countValueX = 0;
    }
    getGraphParametersName();

}

/**
 * @brief getTrendsGraphSignatureDatagetSignatureData : Get graph points from Database
 * @param id of data in database
 */
int TrendsGraph :: getTrendsGraphSignatureData(QString a_historyId)
{
    /* Retreive parameters of SignatureData and retrieve the points */
    int ind=0;
    int totalSampleCountFromDb = 0 ;
    QString word ;
    QString signatureMsg ;
    QStringList a_SignatureData;
    int weldForceY = 0 ;
    int holdForceY = 0 ;
    int totalAbsoluteY = 0 ;
    int partContactPositionY = 0 ;
    int weldTimeY = 0 ;
    int weldEnergyY = 0 ;
    int weldPeakPowerY = 0 ;
    int startFrequencyY = 0 ;
    int frequencyChangeY = 0 ;
    int cycleCounter=0;
    int alarmFlag=0;

    int retVal = OPERATION_SUCCESS ;

    if(weldForceList.size()>=0 && weldForceListYValue.size()>=0)
    {
        weldForceList.clear();
        holdForceList.clear();
        totalAbsoluteList.clear();
        partContactPositionList.clear();
        weldTimeList.clear();
        weldEnergyList.clear();
        weldPeakPowerList.clear();
        startFrequencyList.clear();
        frequencyChangeList.clear();

        weldForceListYValue.clear();
        holdForceListYValue.clear();
        totalAbsoluteListYValue.clear();
        partContactPositionListYValue.clear();
        weldTimeListYValue.clear();
        weldEnergyListYValue.clear();
        weldPeakPowerListYValue.clear();
        startFrequencyListYValue.clear();
        frequencyChangeListYValue.clear();
        cycleCounterList.clear();
        alarmFlagList.clear();
    }
    /* Send request message for the  signature data */
    CIObj.sendMessage(REQ_GET_TRENDSGRAPH_SIGNATURE_DATA, RES_GET_TRENDSGRAPH_SIGNATURE_DATA , a_historyId);


    /* Receive response message of the  signature data */
    CIObj.recivedMessage(RES_GET_TRENDSGRAPH_SIGNATURE_DATA , signatureMsg);

    a_SignatureData = signatureMsg.split(',');

    totalSampleCountFromDb  = a_SignatureData.count();

    if(1>=totalSampleCountFromDb)
    {
        retVal = OPERATION_FAILURE ;
    }
    else{

        int minWeldForceValue=0;
        int minHoldForceValue=0;
        int minTotalAbsoluteValue=0;
        int minPartContactPositionValue=0;
        int minWeldTimeValue=0;
        int minWeldEnergyValue=0;
        int minWeldPeakPowerValue=0;
        int minStartFrequencyValue=0;
        int minFrequencyChangeValue=0;

        int maxWeldForceValue=0;
        int maxHoldForceValue=0;
        int maxTotalAbsoluteValue=0;
        int maxPartContactPositionValue=0;
        int maxWeldTimeValue=0;
        int maxWeldEnergyValue=0;
        int maxWeldPeakPowerValue=0;
        int maxStartFrequencyValue=0;
        int maxFrequencyChangeValue=0;

        for(ind = 0 ; ind < totalSampleCountFromDb;)
        {

            word = a_SignatureData.at(ind );
            weldForceY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 1);
            holdForceY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 2);
            totalAbsoluteY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 3);
            partContactPositionY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 4);
            weldTimeY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 5);
            weldEnergyY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 6);
            weldPeakPowerY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 7);
            startFrequencyY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 8);
            frequencyChangeY = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 9);
            cycleCounter = std::stoi(word.toStdString()) ;

            word = a_SignatureData.at(ind + 10);
            alarmFlag = std::stoi(word.toStdString()) ;

            ind = ind +11;

            weldForceList.push_back(QPointF(cycleCounter , weldForceY));
            holdForceList.push_back(QPointF(cycleCounter , holdForceY));
            totalAbsoluteList.push_back(QPointF(cycleCounter, totalAbsoluteY));
            partContactPositionList.push_back(QPointF(cycleCounter , partContactPositionY));
            weldTimeList.push_back(QPointF(cycleCounter, weldTimeY));
            weldEnergyList.push_back(QPointF(cycleCounter , weldEnergyY));
            weldPeakPowerList.push_back(QPointF(cycleCounter, weldPeakPowerY));
            startFrequencyList.push_back(QPointF(cycleCounter , startFrequencyY));
            frequencyChangeList.push_back(QPointF(cycleCounter, frequencyChangeY));

            weldForceListYValue.push_back(weldForceY);
            holdForceListYValue.push_back(holdForceY);
            totalAbsoluteListYValue.push_back(totalAbsoluteY);
            partContactPositionListYValue.push_back(partContactPositionY);
            weldTimeListYValue.push_back(weldTimeY);
            weldEnergyListYValue.push_back(weldEnergyY);
            weldPeakPowerListYValue.push_back(weldPeakPowerY);
            startFrequencyListYValue.push_back(startFrequencyY);
            frequencyChangeListYValue.push_back(frequencyChangeY);
            cycleCounterList.push_back(cycleCounter);
            alarmFlagList.push_back(alarmFlag);
        }

        minWeldForceValue = *std::min_element(weldForceListYValue.begin(), weldForceListYValue.end());
        maxWeldForceValue = *std::max_element(weldForceListYValue.begin(), weldForceListYValue.end());

        minHoldForceValue = *std::min_element(holdForceListYValue.begin(), holdForceListYValue.end());
        maxHoldForceValue = *std::max_element(holdForceListYValue.begin(), holdForceListYValue.end());

        minTotalAbsoluteValue = *std::min_element(totalAbsoluteListYValue.begin(), totalAbsoluteListYValue.end());
        maxTotalAbsoluteValue = *std::max_element(totalAbsoluteListYValue.begin(), totalAbsoluteListYValue.end());

        minPartContactPositionValue = *std::min_element(partContactPositionListYValue.begin(), partContactPositionListYValue.end());
        maxPartContactPositionValue = *std::max_element(partContactPositionListYValue.begin(), partContactPositionListYValue.end());

        minWeldTimeValue = *std::min_element(weldTimeListYValue.begin(), weldTimeListYValue.end());
        maxWeldTimeValue = *std::max_element(weldTimeListYValue.begin(), weldTimeListYValue.end());

        minWeldEnergyValue = *std::min_element(weldEnergyListYValue.begin(), weldEnergyListYValue.end());
        maxWeldEnergyValue = *std::max_element(weldEnergyListYValue.begin(), weldEnergyListYValue.end());

        minWeldPeakPowerValue = *std::min_element(weldPeakPowerListYValue.begin(), weldPeakPowerListYValue.end());
        maxWeldPeakPowerValue = *std::max_element(weldPeakPowerListYValue.begin(), weldPeakPowerListYValue.end());

        minStartFrequencyValue = *std::min_element(startFrequencyListYValue.begin(), startFrequencyListYValue.end());
        maxStartFrequencyValue = *std::max_element(startFrequencyListYValue.begin(), startFrequencyListYValue.end());

        minFrequencyChangeValue = *std::min_element(frequencyChangeListYValue.begin(), frequencyChangeListYValue.end());
        maxFrequencyChangeValue = *std::max_element(frequencyChangeListYValue.begin(), frequencyChangeListYValue.end());

        //adjust  min and max for all parameters
        // 1 WeldForce
        if(minWeldForceValue==maxWeldForceValue)
        {
            if(maxWeldForceValue==0){
                maxWeldForceValue=minWeldForceValue+1;
            }
            else if(maxWeldForceValue==1  || maxWeldForceValue<12){
                minWeldForceValue=0;
                maxWeldForceValue=maxWeldForceValue+1;
            }
            else {
                minWeldForceValue=0;
                maxWeldForceValue=maxWeldForceValue + (maxWeldForceValue/6)/2;
            }
        }
        else{
            if(maxWeldForceValue==1  || maxWeldForceValue<12)
                maxWeldForceValue=maxWeldForceValue+1;
            else
                maxWeldForceValue=maxWeldForceValue + (maxWeldForceValue/6)/2;
        }
        // 2 HoldForce
        if(minHoldForceValue==maxHoldForceValue)
        {
            if(maxHoldForceValue==0)
                maxHoldForceValue=minHoldForceValue+1;
            else if(maxHoldForceValue==1  || maxHoldForceValue<12){
                minHoldForceValue=0;
                maxHoldForceValue=maxHoldForceValue+1;
            }
            else {
                minHoldForceValue=0;
                maxHoldForceValue=maxHoldForceValue + (maxHoldForceValue/6)/2;
            }
        }
        else{
            if(maxHoldForceValue==1  || maxHoldForceValue<12)
                maxHoldForceValue=maxHoldForceValue+1;
            else
                maxHoldForceValue=maxHoldForceValue + (maxHoldForceValue/6)/2;
        }

        // 3 Toatal absolute
        if(minTotalAbsoluteValue==maxTotalAbsoluteValue)
        {
            if(maxTotalAbsoluteValue==0)
                maxTotalAbsoluteValue=minTotalAbsoluteValue+1;
            else if(maxTotalAbsoluteValue==1  || maxTotalAbsoluteValue<12){
                minTotalAbsoluteValue=0;
                maxTotalAbsoluteValue=maxTotalAbsoluteValue+1;
            }
            else {
                minTotalAbsoluteValue=0;
                maxTotalAbsoluteValue=maxTotalAbsoluteValue + (maxTotalAbsoluteValue/6)/2;
            }
        }
        else{
            if(maxTotalAbsoluteValue==1  || maxTotalAbsoluteValue<12)
                maxTotalAbsoluteValue=maxTotalAbsoluteValue+1;
            else
                maxTotalAbsoluteValue=maxTotalAbsoluteValue + (maxTotalAbsoluteValue/6)/2;
        }
        // 4 Part Contact Distance
        if(minPartContactPositionValue==maxPartContactPositionValue)
        {
            if(maxPartContactPositionValue==0)
                maxPartContactPositionValue=minPartContactPositionValue+1;
            else if(maxPartContactPositionValue==1  || maxPartContactPositionValue<12){
                minPartContactPositionValue=0;
                maxPartContactPositionValue=maxPartContactPositionValue+1;
            }
            else {
                minPartContactPositionValue=0;
                maxPartContactPositionValue=maxPartContactPositionValue + (maxPartContactPositionValue/6)/2;
            }
        }
        else{
            if(maxPartContactPositionValue==1  || maxPartContactPositionValue<12)
                maxPartContactPositionValue=maxPartContactPositionValue+1;
            else
                maxPartContactPositionValue=maxPartContactPositionValue + (maxPartContactPositionValue/6)/2;
        }
        // 5 Weld Time
        if(minWeldTimeValue==maxWeldTimeValue)
        {
            if(maxWeldTimeValue==0)
                maxWeldTimeValue=minWeldTimeValue+1;
            else if(maxWeldTimeValue==1  || maxWeldTimeValue<12){
                minWeldTimeValue=0;
                maxWeldTimeValue=maxWeldTimeValue+1;
            }
            else {
                minWeldTimeValue=0;
                maxWeldTimeValue=maxWeldTimeValue + (maxWeldTimeValue/6)/2;
            }
        }
        else{
            if(maxWeldTimeValue==1  || maxWeldTimeValue<12)
                maxWeldTimeValue=maxWeldTimeValue+1;
            else
                maxWeldTimeValue=maxWeldTimeValue + (maxWeldTimeValue/6)/2;
        }
        // 6 Weld Energy
        if(minWeldEnergyValue==maxWeldEnergyValue)
        {
            if(maxWeldEnergyValue==0)
                maxWeldEnergyValue=minWeldEnergyValue+1;
            else if(maxWeldEnergyValue==1  || maxWeldEnergyValue<12){
                minWeldEnergyValue=0;
                maxWeldEnergyValue=maxWeldEnergyValue+1;
            }
            else {
                minWeldEnergyValue=0;
                maxWeldEnergyValue=maxWeldEnergyValue + (maxWeldEnergyValue/6)/2;
            }
        }
        else{
            if(maxWeldEnergyValue==1 || maxWeldEnergyValue<12)
                maxWeldEnergyValue=maxWeldEnergyValue+1;
            else if(maxTotalAbsoluteValue==1  || maxTotalAbsoluteValue<12){
                minTotalAbsoluteValue=0;
                maxTotalAbsoluteValue=maxTotalAbsoluteValue+1;
            }
            else
                maxWeldEnergyValue=maxWeldEnergyValue + (maxWeldEnergyValue/6)/2;
        }
        // 7 Weld Peak Power
        if(minWeldPeakPowerValue==maxWeldPeakPowerValue)
        {
            if(maxWeldPeakPowerValue==0)
                maxWeldPeakPowerValue=minWeldPeakPowerValue+1;
            else if(maxWeldPeakPowerValue==1  || maxWeldPeakPowerValue<12){
                minWeldPeakPowerValue=0;
                maxWeldPeakPowerValue=maxWeldPeakPowerValue+1;
            }
            else {
                minWeldPeakPowerValue=0;
                maxWeldPeakPowerValue=maxWeldPeakPowerValue + (maxWeldPeakPowerValue/6)/2;
            }
        }
        else{
            if(maxWeldPeakPowerValue==1 || maxWeldPeakPowerValue<12)
                maxWeldPeakPowerValue=maxWeldPeakPowerValue+1;
            else if(maxTotalAbsoluteValue==1  || maxTotalAbsoluteValue<12){
                minTotalAbsoluteValue=0;
                maxTotalAbsoluteValue=maxTotalAbsoluteValue+1;
            }
            else
                maxWeldPeakPowerValue=maxWeldPeakPowerValue + (maxWeldPeakPowerValue/6)/2;
        }
        // 8 Start Frequency
        if(minStartFrequencyValue==maxStartFrequencyValue)
        {
            if(maxStartFrequencyValue==0)
                maxStartFrequencyValue=minStartFrequencyValue+1;
            else if(maxStartFrequencyValue==1  || maxStartFrequencyValue<12){
                minStartFrequencyValue=0;
                maxStartFrequencyValue=maxStartFrequencyValue+1;
            }
            else {
                minStartFrequencyValue=0;
                maxStartFrequencyValue=maxStartFrequencyValue + (maxStartFrequencyValue/6)/2;
            }
        }
        else{
            if(maxStartFrequencyValue==1 || maxStartFrequencyValue<12)
                maxStartFrequencyValue=maxStartFrequencyValue+1;
            else
                maxStartFrequencyValue=maxStartFrequencyValue + (maxStartFrequencyValue/6)/2;
        }
        // 9 Frequency Change
        if(minFrequencyChangeValue==maxFrequencyChangeValue)
        {
            if(maxFrequencyChangeValue==0)
                maxFrequencyChangeValue=minFrequencyChangeValue+1;
            else if(maxFrequencyChangeValue==1  || maxFrequencyChangeValue<12){
                minFrequencyChangeValue=0;
                maxFrequencyChangeValue=maxFrequencyChangeValue+1;
            }
            else {
                minFrequencyChangeValue=0;
                maxFrequencyChangeValue=maxFrequencyChangeValue + (maxFrequencyChangeValue/6)/2;
            }
        }
        else{
            if(maxFrequencyChangeValue==1 || maxFrequencyChangeValue<12)
                maxFrequencyChangeValue=maxFrequencyChangeValue+1;
            else
                maxFrequencyChangeValue=maxFrequencyChangeValue + (maxFrequencyChangeValue/6)/2;
        }

        //set min and max for all parameters
        setMinWeldForceYValue(minWeldForceValue);
        setMinHoldForceYValue(minHoldForceValue);
        setMinTotalAbsoluteYValue(minTotalAbsoluteValue);
        setMinPartContactPosYValue(minPartContactPositionValue);
        setMinWeldTimeYValue(minWeldTimeValue);
        setMinWeldEnergyYValue(minWeldEnergyValue);
        setMinWeldPeakPowerYValue(minWeldPeakPowerValue);
        setMinStartFrequencyYValue(minStartFrequencyValue);
        setMinFrequencyChangeYValue(minFrequencyChangeValue);

        setMaxWeldForceYValue(maxWeldForceValue);
        setMaxHoldForceYValue(maxHoldForceValue);
        setMaxTotalAbsoluteYValue(maxTotalAbsoluteValue);
        setMaxPartContactPosYValue(maxPartContactPositionValue);
        setMaxWeldTimeYValue(maxWeldTimeValue);
        setMaxWeldEnergyYValue(maxWeldEnergyValue);
        setMaxWeldPeakPowerYValue(maxWeldPeakPowerValue);
        setMaxStartFrequencyYValue(maxStartFrequencyValue);
        setMaxFrequencyChangeYValue(maxFrequencyChangeValue);

        setTickCountY(6);
        setXCount(cycleCounterList.size());

    }
    return retVal;
}

/**
 * @brief TrendsGraph::getSuspectRejectParamLimits : Get parameter limits value based on index
 * @param a_historyId
 * @return
 */
int TrendsGraph::getSuspectRejectParamLimits(QString a_historyId)
{
    int totalLimitsCountFromDb = 0 ;
    QString wordLimits ;
    QString signatureMsgLimits ;
    QStringList a_SignatureDataLimits;

    QStringList a_SignatureDataLimitsTime;
    QStringList a_SignatureDataLimitsEnergy;
    QStringList a_SignatureDataLimitsPeakPower;
    QStringList a_SignatureDataLimitsAbsoluteDist;
    QStringList a_SignatureDataLimitsFrequency;
    QStringList a_SignatureDataLimitsMaxWeldForce;

    int timeSuspectLimitHigh=0;
    int timeRejectLimitHigh=0;
    int timeRejectLimitLow=0;
    int timeSuspectLimitLow=0;

    int energySuspectLimitHigh=0;
    int energyRejectLimitHigh=0;
    int energyRejectLimitLow=0;
    int energySuspectLimitLow=0;

    int peakPowerSuspectLimitHigh=0;
    int peakPowerRejectLimitHigh=0;
    int peakPowerRejectLimitLow=0;
    int peakPowerSuspectLimitLow=0;

    int absoluteDistSuspectLimitHigh=0;
    int absoluteDistRejectLimitHigh=0;
    int absoluteDistRejectLimitLow=0;
    int absoluteDistSuspectLimitLow=0;

    int frequencySuspectLimitHigh=0;
    int frequencyRejectLimitHigh=0;
    int frequencyRejectLimitLow=0;
    int frequencySuspectLimitLow=0;

    int maxWeldForceSuspectLimitHigh=0;
    int maxWeldForceRejectLimitHigh=0;
    int maxWeldForceRejectLimitLow=0;
    int maxWeldForceSuspectLimitLow=0;

    int retVal = OPERATION_SUCCESS ;
    /* Send request message for the  signature data */
    CIObj.sendMessage(REQ_GET_SUSPECT_REJECT_LIMITS_TRENDSGRAPH, RES_GET_SUSPECT_REJECT_LIMITS_TRENDSGRAPH , a_historyId);

    /* Receive response message of the  signature data */
    CIObj.recivedMessage(RES_GET_SUSPECT_REJECT_LIMITS_TRENDSGRAPH , signatureMsgLimits);

    //select Time,Energy,PeakPower,AbsoluteDistance,Frequency,EndWeldForce from SuspectRejectLimits;
    a_SignatureDataLimits = signatureMsgLimits.split(',');

    totalLimitsCountFromDb  = a_SignatureDataLimits.count();

    if(1>=totalLimitsCountFromDb)
    {
        retVal = OPERATION_FAILURE ;
    }
    else
    {
        a_SignatureDataLimitsTime=a_SignatureDataLimits.at(0).split(':');//time
        a_SignatureDataLimitsEnergy=a_SignatureDataLimits.at(1).split(':');//energy
        a_SignatureDataLimitsPeakPower=a_SignatureDataLimits.at(2).split(':');//power
        a_SignatureDataLimitsAbsoluteDist=a_SignatureDataLimits.at(3).split(':');//abs distance
        a_SignatureDataLimitsFrequency=a_SignatureDataLimits.at(4).split(':');//freq
        a_SignatureDataLimitsMaxWeldForce=a_SignatureDataLimits.at(5).split(':');//weldforce

        wordLimits=a_SignatureDataLimitsTime.at(3);
        timeSuspectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsTime.at(4);
        timeSuspectLimitHigh=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsTime.at(8);
        timeRejectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsTime.at(9);
        timeRejectLimitHigh=std::stoi(wordLimits.toStdString()) ;

        wordLimits=a_SignatureDataLimitsEnergy.at(3);
        energySuspectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsEnergy.at(4);
        energySuspectLimitHigh=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsEnergy.at(8);
        energyRejectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsEnergy.at(9);
        energyRejectLimitHigh=std::stoi(wordLimits.toStdString()) ;

        wordLimits=a_SignatureDataLimitsPeakPower.at(3);
        peakPowerSuspectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsPeakPower.at(4);
        peakPowerSuspectLimitHigh=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsPeakPower.at(8);
        peakPowerRejectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsPeakPower.at(9);
        peakPowerRejectLimitHigh=std::stoi(wordLimits.toStdString()) ;

        wordLimits=a_SignatureDataLimitsAbsoluteDist.at(3);
        absoluteDistSuspectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsAbsoluteDist.at(4);
        absoluteDistSuspectLimitHigh=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsAbsoluteDist.at(8);
        absoluteDistRejectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsAbsoluteDist.at(9);
        absoluteDistRejectLimitHigh=std::stoi(wordLimits.toStdString()) ;

        wordLimits=a_SignatureDataLimitsFrequency.at(3);
        frequencySuspectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsFrequency.at(4);
        frequencySuspectLimitHigh=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsFrequency.at(8);
        frequencyRejectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsFrequency.at(9);
        frequencyRejectLimitHigh=std::stoi(wordLimits.toStdString()) ;

        wordLimits=a_SignatureDataLimitsMaxWeldForce.at(3);
        maxWeldForceSuspectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsMaxWeldForce.at(4);
        maxWeldForceSuspectLimitHigh=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsMaxWeldForce.at(8);
        maxWeldForceRejectLimitLow=std::stoi(wordLimits.toStdString()) ;
        wordLimits=a_SignatureDataLimitsMaxWeldForce.at(9);
        maxWeldForceRejectLimitHigh=std::stoi(wordLimits.toStdString()) ;

        setTimeSuspectLimitLow(timeSuspectLimitLow);
        setTimeSuspectLimitHigh(timeSuspectLimitHigh);
        setTimeRejectLimitLow(timeRejectLimitLow);
        setTimeRejectLimitHigh(timeRejectLimitHigh);

        setEnergySuspectLimitLow(energySuspectLimitLow);
        setEnergySuspectLimitHigh(energySuspectLimitHigh);
        setEnergyRejectLimitLow(energyRejectLimitLow);
        setEnergyRejectLimitHigh(energyRejectLimitHigh);

        setPeakPowerSuspectLimitLow(peakPowerSuspectLimitLow);
        setPeakPowerSuspectLimitHigh(peakPowerSuspectLimitHigh);
        setPeakPowerRejectLimitLow(peakPowerRejectLimitLow);
        setPeakPowerRejectLimitHigh(peakPowerRejectLimitHigh);

        setAbsoluteDistSuspectLimitLow(absoluteDistSuspectLimitLow);
        setAbsoluteDistSuspectLimitHigh(absoluteDistSuspectLimitHigh);
        setAbsoluteDistRejectLimitLow(absoluteDistRejectLimitLow);
        setAbsoluteDistRejectLimitHigh(absoluteDistRejectLimitHigh);

        setFrequencySuspectLimitLow(frequencySuspectLimitLow);
        setFrequencySuspectLimitHigh(frequencySuspectLimitHigh);
        setFrequencyRejectLimitLow(frequencyRejectLimitLow);
        setFrequencyRejectLimitHigh(frequencyRejectLimitHigh);

        setMaxWeldForceSuspectLimitLow(maxWeldForceSuspectLimitLow);
        setMaxWeldForceSuspectLimitHigh(maxWeldForceSuspectLimitHigh);
        setMaxWeldForceRejectLimitLow(maxWeldForceRejectLimitLow);
        setMaxWeldForceRejectLimitHigh(maxWeldForceRejectLimitHigh);

    }
    return retVal;
}

/**
 * @brief TrendsGraph::getYAxisMaxValues : Get YAxis Values to plot the graph
 * @param index
 * @return
 */
int TrendsGraph::getYAxisMaxValues(int index)
{
    switch (index) {
    case 0:
        return m_maxWeldForceYValue;
    case 1:
        return m_maxHoldForceYValue;
    case 2:
        return m_maxTotalAbsoluteYValue;
    case 3:
        return m_maxPartContactPosYValue;
    case 4:
        return m_maxWeldTimeYValue;
    case 5:
        return m_maxWeldEnergyYValue;
    case 6:
        return m_maxWeldPeakPowerYValue;
    case 7:
        return m_maxStartFrequencyYValue;
    case 8:
        return m_maxFrequencyChangeYValue;

    default:
        break;
    }
    return (0);
}

/**
 * @brief TrendsGraph::getYAxisMinValues : Get YAxis value of minimum range
 * @param index
 * @return
 */
int TrendsGraph::getYAxisMinValues(int index)
{
    switch (index) {
    case 0:
        return m_minWeldForceYValue;
    case 1:
        return m_minHoldForceYValue;
    case 2:
        return m_minTotalAbsoluteYValue;
    case 3:
        return m_minPartContactPosYValue;
    case 4:
        return m_minWeldTimeYValue;
    case 5:
        return m_minWeldEnergyYValue;
    case 6:
        return m_minWeldPeakPowerYValue;
    case 7:
        return m_minStartFrequencyYValue;
    case 8:
        return m_minFrequencyChangeYValue;
    default:
        break;
    }
    return (0);
}

/**
 * @brief TrendsGraph::getGraphParametersName : Get the parameter value from List and save in model
 */
void TrendsGraph :: getGraphParametersName()
{
    context->setContextProperty("trendsModel", QVariant::fromValue(m_listdummy));
    for(int element=0; element <m_list.size() ; element++)
    {
        Trendsgraphparametername *ptrgraph = dynamic_cast<Trendsgraphparametername*>(m_list.at(element));
        delete ptrgraph;
        ptrgraph = NULL;
    }
    m_list.clear();
    QString parameterMsg ;
    QStringList a_parameterMsgList;

    parameterMsg="MaxWeldForce,MaxHoldForce,TotalAbsolute,PartContactPosition,WeldTime,WeldEnergy,WeldPeakPower,StartFrequency,FrequencyChange";
    a_parameterMsgList = parameterMsg.split(',');
    for(int i =0; i< a_parameterMsgList.size();i++)
    {
        Trendsgraphparametername *ptrgraph = new Trendsgraphparametername;
        ptrgraph->m_ParameterName = a_parameterMsgList.at(i);
        m_list.append(ptrgraph);
    }
    context->setContextProperty("trendsModel", QVariant::fromValue(m_list));
}

/**
 * @brief TrendsGraph::getCycleConterValues : Get cycle counter values
 */
void TrendsGraph :: getCycleConterValues()
{
    QString a_parameterMsg = "";
    QStringList a_parameterMsgList;

    context->setContextProperty("cycleCounterModel", QVariant::fromValue(m_listdummy));
    for(int element=0; element <m_list.size() ; element++)
    {
        Trendsgraphparametername *ptrgraph = dynamic_cast<Trendsgraphparametername*>(m_list.at(element));
        delete ptrgraph;
        ptrgraph = NULL;
    }
    m_list.clear();

    for(int i =0; i< cycleCounterList.size();i++)
    {
        a_parameterMsg.append(QString::number(cycleCounterList.at(i)));
        if(i<cycleCounterList.size()-1)
            a_parameterMsg.append(",");
    }

    a_parameterMsgList = a_parameterMsg.split(',');

    for(int i =0; i< (a_parameterMsgList.size());i++)
    {
        m_list.append(new TrendsGraphCycleCounterVal((a_parameterMsgList.at(i)),alarmFlagList.at(i)));
    }
    /* cyclecount model to update the UI */
    context->setContextProperty("cycleCounterModel", QVariant::fromValue(m_list));
}

/**
 * @brief TrendsGraph::getTopScaleModel : Top scale model set
 */
void TrendsGraph :: getTopScaleModel()
{
    QString a_parameterMsg = "1";
    m_list.clear();

    Trendsgraphparametername *ptrgraph = new Trendsgraphparametername;
    ptrgraph->m_ParameterName = a_parameterMsg;
    m_list.append(ptrgraph);

    context->setContextProperty("topScaleModel", QVariant::fromValue(m_list));
}

/**
 * @brief TrendsGraph::getBrushColorForAxis
 * @return
 */
QString TrendsGraph::getBrushColorForAxis()
{
    return "blue";//QBrush(Qt::blue);
}

/**
 * @brief TrendsGraph::getAlarmFlag
 * @param index
 * @return
 */
int TrendsGraph::getAlarmFlag(int index)
{
    return alarmFlagList.at(index);
}

/**
 * @brief TrendsGraph::getXCount
 * @return
 */
int TrendsGraph::getXCount()
{
    return m_countValueX;
}

/**
 * @brief TrendsGraph::setXCount
 * @param a
 */
void TrendsGraph::setXCount(int a)
{
    if (a != m_countValueX)
    {
        m_countValueX = a;
        emit valueXChanged();
    }
}

/**
 * @brief TrendsGraph::getMinWeldForceYValue
 * @return
 */
int TrendsGraph::getMinWeldForceYValue()
{
    return m_minWeldForceYValue;
}

/**
 * @brief TrendsGraph::setMinWeldForceYValue
 * @param a
 */
void TrendsGraph::setMinWeldForceYValue(int a)
{
    if (a != m_minWeldForceYValue)
    {
        m_minWeldForceYValue = a;
        emit minWeldForceYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxWeldForceYValue
 * @return
 */
int TrendsGraph::getMaxWeldForceYValue()
{
    return m_maxWeldForceYValue;
}

/**
 * @brief TrendsGraph::setMaxWeldForceYValue
 * @param a
 */
void TrendsGraph::setMaxWeldForceYValue(int a)
{
    if (a != m_maxWeldForceYValue)
    {
        m_maxWeldForceYValue = a;
        emit maxWeldForceYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMinHoldForceYValue
 * @return
 */
int TrendsGraph::getMinHoldForceYValue()
{
    return m_minHoldForceYValue;
}

/**
 * @brief TrendsGraph::setMinHoldForceYValue
 * @param a
 */
void TrendsGraph::setMinHoldForceYValue(int a)
{
    if (a != m_minHoldForceYValue)
    {
        m_minHoldForceYValue = a;
        emit minHoldForceYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxHoldForceYValue
 * @return
 */
int TrendsGraph::getMaxHoldForceYValue()
{
    return m_maxHoldForceYValue;
}

/**
 * @brief TrendsGraph::setMaxHoldForceYValue
 * @param a
 */
void TrendsGraph::setMaxHoldForceYValue(int a)
{
    if (a != m_maxHoldForceYValue)
    {
        m_maxHoldForceYValue = a;
        emit maxHoldForceYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMinTotalAbsoluteYValue
 * @return
 */
int TrendsGraph::getMinTotalAbsoluteYValue()
{
    return m_minTotalAbsoluteYValue;
}

/**
 * @brief TrendsGraph::setMinTotalAbsoluteYValue
 * @param a
 */
void TrendsGraph::setMinTotalAbsoluteYValue(int a)
{
    if (a != m_minTotalAbsoluteYValue)
    {
        m_minTotalAbsoluteYValue = a;
        emit minTotalAbsoluteYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxTotalAbsoluteYValue
 * @return
 */
int TrendsGraph::getMaxTotalAbsoluteYValue()
{
    return m_maxTotalAbsoluteYValue;
}

/**
 * @brief TrendsGraph::setMaxTotalAbsoluteYValue
 * @param a
 */
void TrendsGraph::setMaxTotalAbsoluteYValue(int a)
{
    if (a != m_maxTotalAbsoluteYValue)
    {
        m_maxTotalAbsoluteYValue = a;
        emit maxTotalAbsoluteYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMinPartContactPosYValue
 * @return
 */
int TrendsGraph::getMinPartContactPosYValue()
{
    return m_minPartContactPosYValue;
}

/**
 * @brief TrendsGraph::setMinPartContactPosYValue
 * @param a
 */
void TrendsGraph::setMinPartContactPosYValue(int a)
{
    if (a != m_minPartContactPosYValue)
    {
        m_minPartContactPosYValue = a;
        emit minPartContactPosYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxPartContactPosYValue
 * @return
 */
int TrendsGraph::getMaxPartContactPosYValue()
{
    return m_maxPartContactPosYValue;
}

/**
 * @brief TrendsGraph::setMaxPartContactPosYValue
 * @param a
 */
void TrendsGraph::setMaxPartContactPosYValue(int a)
{
    if (a != m_maxPartContactPosYValue)
    {
        m_maxPartContactPosYValue = a;
        emit maxPartContactPosYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMinWeldTimeYValue
 * @return
 */
int TrendsGraph::getMinWeldTimeYValue()
{
    return m_minWeldTimeYValue;
}

/**
 * @brief TrendsGraph::setMinWeldTimeYValue
 * @param a
 */
void TrendsGraph::setMinWeldTimeYValue(int a)
{
    if (a != m_minWeldTimeYValue)
    {
        m_minWeldTimeYValue = a;
        emit minWeldTimeYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxWeldTimeYValue
 * @return
 */
int TrendsGraph::getMaxWeldTimeYValue()
{
    return m_maxWeldTimeYValue;
}

/**
 * @brief TrendsGraph::setMaxWeldTimeYValue
 * @param a
 */
void TrendsGraph::setMaxWeldTimeYValue(int a)
{
    if (a != m_maxWeldTimeYValue)
    {
        m_maxWeldTimeYValue = a;
        emit maxWeldTimeYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMinWeldEnergyYValue
 * @return
 */
int TrendsGraph::getMinWeldEnergyYValue()
{
    return m_minWeldEnergyYValue;
}

/**
 * @brief TrendsGraph::setMinWeldEnergyYValue
 * @param a
 */
void TrendsGraph::setMinWeldEnergyYValue(int a)
{
    if (a != m_minWeldEnergyYValue)
    {
        m_minWeldEnergyYValue = a;
        emit minWeldEnergyYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxWeldEnergyYValue
 * @return
 */
int TrendsGraph::getMaxWeldEnergyYValue()
{
    return m_maxWeldEnergyYValue;
}

/**
 * @brief TrendsGraph::setMaxWeldEnergyYValue
 * @param a
 */
void TrendsGraph::setMaxWeldEnergyYValue(int a)
{
    if (a != m_maxWeldEnergyYValue)
    {
        m_maxWeldEnergyYValue = a;
        emit maxWeldEnergyYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMinWeldPeakPowerYValue
 * @return
 */
int TrendsGraph::getMinWeldPeakPowerYValue()
{
    return m_minWeldPeakPowerYValue;
}

/**
 * @brief TrendsGraph::setMinWeldPeakPowerYValue
 * @param a
 */
void TrendsGraph::setMinWeldPeakPowerYValue(int a)
{
    if (a != m_minWeldPeakPowerYValue)
    {
        m_minWeldPeakPowerYValue = a;
        emit minWeldPeakPowerYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxWeldPeakPowerYValue
 * @return
 */
int TrendsGraph::getMaxWeldPeakPowerYValue()
{
    return m_maxWeldPeakPowerYValue;
}

/**
 * @brief TrendsGraph::setMaxWeldPeakPowerYValue
 * @param a
 */
void TrendsGraph::setMaxWeldPeakPowerYValue(int a)
{
    if (a != m_maxWeldPeakPowerYValue)
    {
        m_maxWeldPeakPowerYValue = a;
        emit maxWeldPeakPowerYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMinStartFrequencyYValue
 * @return
 */
int TrendsGraph::getMinStartFrequencyYValue()
{
    return m_minStartFrequencyYValue;
}

/**
 * @brief TrendsGraph::setMinStartFrequencyYValue
 * @param a
 */
void TrendsGraph::setMinStartFrequencyYValue(int a)
{
    if (a != m_minStartFrequencyYValue)  {
        m_minStartFrequencyYValue = a;
        emit minStartFrequencyYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxStartFrequencyYValue
 * @return
 */
int TrendsGraph::getMaxStartFrequencyYValue()
{
    return m_maxStartFrequencyYValue;
}

/**
 * @brief TrendsGraph::setMaxStartFrequencyYValue
 * @param a
 */
void TrendsGraph::setMaxStartFrequencyYValue(int a)
{
    if (a != m_maxStartFrequencyYValue)  {
        m_maxStartFrequencyYValue = a;
        emit maxStartFrequencyYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMinFrequencyChangeYValue
 * @return
 */
int TrendsGraph::getMinFrequencyChangeYValue()
{
    return m_minFrequencyChangeYValue;
}

/**
 * @brief TrendsGraph::setMinFrequencyChangeYValue
 * @param a
 */
void TrendsGraph::setMinFrequencyChangeYValue(int a)
{
    if (a != m_minFrequencyChangeYValue)
    {
        m_minFrequencyChangeYValue = a;
        emit minFrequencyChangeYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxFrequencyChangeYValue
 * @return
 */
int TrendsGraph::getMaxFrequencyChangeYValue()
{
    return m_maxFrequencyChangeYValue;
}

/**
 * @brief TrendsGraph::setMaxFrequencyChangeYValue
 * @param a
 */
void TrendsGraph::setMaxFrequencyChangeYValue(int a)
{
    if (a != m_maxFrequencyChangeYValue)
    {
        m_maxFrequencyChangeYValue = a;
        emit maxFrequencyChangeYValueChanged();
    }
}

/**
 * @brief TrendsGraph::getTickCountY
 * @return
 */
int TrendsGraph::getTickCountY()
{
    return m_tickYCount;
}

/**
 * @brief TrendsGraph::setTickCountY
 * @param a
 */
void TrendsGraph::setTickCountY(int a)
{
    if (a != m_tickYCount)
    {
        m_tickYCount = a;
        emit tickCountYChanged();
    }
}

/**
 * @brief TrendsGraph::getSuspectLimitHigh
 * @return
 */
int TrendsGraph::getSuspectLimitHigh()
{
    return m_suspectLimitHigh;
}

/**
 * @brief TrendsGraph::setSuspectLimitHigh
 * @param a
 */
void TrendsGraph::setSuspectLimitHigh(int a)
{
    if (a != m_suspectLimitHigh)
    {
        m_suspectLimitHigh = a;
        emit suspectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getRejectLimitHigh
 * @return
 */
int TrendsGraph::getRejectLimitHigh()
{
    return m_rejectLimitHigh;
}

/**
 * @brief TrendsGraph::setRejectLimitHigh
 * @param a
 */
void TrendsGraph::setRejectLimitHigh(int a)
{
    if (a != m_rejectLimitHigh)
    {
        m_rejectLimitHigh = a;
        emit rejectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getRejectLimitLow
 * @return
 */
int TrendsGraph::getRejectLimitLow()
{
    return m_suspectLimitLow;
}

/**
 * @brief TrendsGraph::setRejectLimitLow
 * @param a
 */
void TrendsGraph::setRejectLimitLow(int a)
{
    if (a != m_suspectLimitLow)
    {
        m_suspectLimitLow = a;
        emit rejectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getSuspectLimitLow
 * @return
 */
int TrendsGraph::getSuspectLimitLow()
{
    return m_rejectLimitLow;
}

/**
 * @brief TrendsGraph::setSuspectLimitLow
 * @param a
 */
void TrendsGraph::setSuspectLimitLow(int a)
{
    if (a != m_rejectLimitLow)
    {
        m_rejectLimitLow = a;
        emit suspectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getTimeSuspectLimitHigh
 * @return
 */
int TrendsGraph::getTimeSuspectLimitHigh()
{
    return m_timeSuspectLimitHigh;
}

/**
 * @brief TrendsGraph::setTimeSuspectLimitHigh
 * @param a
 */
void TrendsGraph::setTimeSuspectLimitHigh(int a)
{
    if (a != m_timeSuspectLimitHigh)
    {
        m_timeSuspectLimitHigh = a;
        emit timeSuspectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getTimeRejectLimitHigh
 * @return
 */
int TrendsGraph::getTimeRejectLimitHigh()
{
    return m_timeRejectLimitHigh;
}

/**
 * @brief TrendsGraph::setTimeRejectLimitHigh
 * @param a
 */
void TrendsGraph::setTimeRejectLimitHigh(int a)
{
    if (a != m_timeRejectLimitHigh
            ) {
        m_timeRejectLimitHigh = a;
        emit timeRejectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getTimeRejectLimitLow
 * @return
 */
int TrendsGraph::getTimeRejectLimitLow()
{
    return m_timeRejectLimitLow;
}

/**
 * @brief TrendsGraph::setTimeRejectLimitLow
 * @param a
 */
void TrendsGraph::setTimeRejectLimitLow(int a)
{
    if (a != m_timeRejectLimitLow)
    {
        m_timeRejectLimitLow = a;
        emit timeRejectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getTimeSuspectLimitLow
 * @return
 */
int TrendsGraph::getTimeSuspectLimitLow()
{
    return m_timeSuspectLimitLow;
}

/**
 * @brief TrendsGraph::setTimeSuspectLimitLow
 * @param a
 */
void TrendsGraph::setTimeSuspectLimitLow(int a)
{
    if (a != m_timeSuspectLimitLow)
    {
        m_timeSuspectLimitLow = a;
        emit timeSuspectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getEnergySuspectLimitHigh
 * @return
 */
int TrendsGraph::getEnergySuspectLimitHigh()
{
    return m_energySuspectLimitHigh;
}

/**
 * @brief TrendsGraph::setEnergySuspectLimitHigh
 * @param a
 */
void TrendsGraph::setEnergySuspectLimitHigh(int a)
{
    if (a != m_energySuspectLimitHigh)
    {
        m_energySuspectLimitHigh = a;
        emit energySuspectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getEnergyRejectLimitHigh
 * @return
 */
int TrendsGraph::getEnergyRejectLimitHigh()
{
    return m_energyRejectLimitHigh;
}

/**
 * @brief TrendsGraph::setEnergyRejectLimitHigh
 * @param a
 */
void TrendsGraph::setEnergyRejectLimitHigh(int a)
{
    if (a != m_energyRejectLimitHigh)
    {
        m_energyRejectLimitHigh = a;
        emit energyRejectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getEnergyRejectLimitLow
 * @return
 */
int TrendsGraph::getEnergyRejectLimitLow()
{
    return m_energyRejectLimitLow;
}

/**
 * @brief TrendsGraph::setEnergyRejectLimitLow
 * @param a
 */
void TrendsGraph::setEnergyRejectLimitLow(int a)
{
    if (a != m_energyRejectLimitLow)
    {
        m_energyRejectLimitLow = a;
        emit energyRejectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getEnergySuspectLimitLow
 * @return
 */
int TrendsGraph::getEnergySuspectLimitLow()
{
    return m_energySuspectLimitLow;
}

/**
 * @brief TrendsGraph::setEnergySuspectLimitLow
 * @param a
 */
void TrendsGraph::setEnergySuspectLimitLow(int a)
{
    if (a != m_energySuspectLimitLow)
    {
        m_energySuspectLimitLow = a;
        emit energySuspectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getPeakPowerSuspectLimitHigh
 * @return
 */
int TrendsGraph::getPeakPowerSuspectLimitHigh()
{
    return m_peakPowerSuspectLimitHigh;
}

/**
 * @brief TrendsGraph::setPeakPowerSuspectLimitHigh
 * @param a
 */
void TrendsGraph::setPeakPowerSuspectLimitHigh(int a)
{
    if (a != m_peakPowerSuspectLimitHigh)
    {
        m_peakPowerSuspectLimitHigh = a;
        emit peakPowerSuspectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getPeakPowerRejectLimitHigh
 * @return
 */
int TrendsGraph::getPeakPowerRejectLimitHigh()
{
    return m_peakPowerRejectLimitHigh;
}

/**
 * @brief TrendsGraph::setPeakPowerRejectLimitHigh
 * @param a
 */
void TrendsGraph::setPeakPowerRejectLimitHigh(int a)
{
    if (a != m_peakPowerRejectLimitHigh)
    {
        m_peakPowerRejectLimitHigh = a;
        emit peakPowerRejectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getPeakPowerRejectLimitLow
 * @return
 */
int TrendsGraph::getPeakPowerRejectLimitLow()
{
    return m_peakPowerRejectLimitLow;
}

/**
 * @brief TrendsGraph::setPeakPowerRejectLimitLow
 * @param a
 */
void TrendsGraph::setPeakPowerRejectLimitLow(int a)
{
    if (a != m_peakPowerRejectLimitLow)
    {
        m_peakPowerRejectLimitLow = a;
        emit peakPowerRejectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getPeakPowerSuspectLimitLow
 * @return
 */
int TrendsGraph::getPeakPowerSuspectLimitLow()
{
    return m_peakPowerSuspectLimitLow;
}

/**
 * @brief TrendsGraph::setPeakPowerSuspectLimitLow
 * @param a
 */
void TrendsGraph::setPeakPowerSuspectLimitLow(int a)
{
    if (a != m_peakPowerSuspectLimitLow)
    {
        m_peakPowerSuspectLimitLow = a;
        emit peakPowerSuspectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getAbsoluteDistSuspectLimitHigh
 * @return
 */
int TrendsGraph::getAbsoluteDistSuspectLimitHigh()
{
    return m_absoluteDistSuspectLimitHigh;
}

/**
 * @brief TrendsGraph::setAbsoluteDistSuspectLimitHigh
 * @param a
 */
void TrendsGraph::setAbsoluteDistSuspectLimitHigh(int a)
{
    if (a != m_absoluteDistSuspectLimitHigh)
    {
        m_absoluteDistSuspectLimitHigh = a;
        emit absoluteDistSuspectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getAbsoluteDistRejectLimitHigh
 * @return
 */
int TrendsGraph::getAbsoluteDistRejectLimitHigh()
{
    return m_absoluteDistRejectLimitHigh;
}

/**
 * @brief TrendsGraph::setAbsoluteDistRejectLimitHigh
 * @param a
 */
void TrendsGraph::setAbsoluteDistRejectLimitHigh(int a)
{
    if (a != m_absoluteDistRejectLimitHigh)
    {
        m_absoluteDistRejectLimitHigh = a;
        emit absoluteDistRejectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getAbsoluteDistRejectLimitLow
 * @return
 */
int TrendsGraph::getAbsoluteDistRejectLimitLow()
{
    return m_absoluteDistRejectLimitLow;
}

/**
 * @brief TrendsGraph::setAbsoluteDistRejectLimitLow
 * @param a
 */
void TrendsGraph::setAbsoluteDistRejectLimitLow(int a)
{
    if (a != m_absoluteDistRejectLimitLow)
    {
        m_absoluteDistRejectLimitLow = a;
        emit absoluteDistRejectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getAbsoluteDistSuspectLimitLow
 * @return
 */
int TrendsGraph::getAbsoluteDistSuspectLimitLow()
{
    return m_absoluteDistSuspectLimitLow;
}

/**
 * @brief TrendsGraph::setAbsoluteDistSuspectLimitLow
 * @param a
 */
void TrendsGraph::setAbsoluteDistSuspectLimitLow(int a)
{
    if (a != m_absoluteDistSuspectLimitLow)
    {
        m_absoluteDistSuspectLimitLow = a;
        emit absoluteDistSuspectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getFrequencySuspectLimitHigh
 * @return
 */
int TrendsGraph::getFrequencySuspectLimitHigh()
{
    return m_frequencySuspectLimitHigh;
}

/**
 * @brief TrendsGraph::setFrequencySuspectLimitHigh
 * @param a
 */
void TrendsGraph::setFrequencySuspectLimitHigh(int a)
{
    if (a != m_frequencySuspectLimitHigh)
    {
        m_frequencySuspectLimitHigh = a;
        emit frequencySuspectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getFrequencyRejectLimitHigh
 * @return
 */
int TrendsGraph::getFrequencyRejectLimitHigh()
{
    return m_frequencyRejectLimitHigh;
}

/**
 * @brief TrendsGraph::setFrequencyRejectLimitHigh
 * @param a
 */
void TrendsGraph::setFrequencyRejectLimitHigh(int a)
{
    if (a != m_frequencyRejectLimitHigh)
    {
        m_frequencyRejectLimitHigh = a;
        emit frequencyRejectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getFrequencyRejectLimitLow
 * @return
 */
int TrendsGraph::getFrequencyRejectLimitLow()
{
    return m_frequencySuspectLimitLow;
}

/**
 * @brief TrendsGraph::setFrequencyRejectLimitLow
 * @param a
 */
void TrendsGraph::setFrequencyRejectLimitLow(int a)
{
    if (a != m_frequencyRejectLimitLow)
    {
        m_frequencyRejectLimitLow = a;
        emit frequencyRejectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getFrequencySuspectLimitLow
 * @return
 */
int TrendsGraph::getFrequencySuspectLimitLow()
{
    return m_frequencySuspectLimitLow;
}

/**
 * @brief TrendsGraph::setFrequencySuspectLimitLow
 * @param a
 */
void TrendsGraph::setFrequencySuspectLimitLow(int a)
{
    if (a != m_frequencySuspectLimitLow)
    {
        m_frequencySuspectLimitLow = a;
        emit frequencySuspectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxWeldForceSuspectLimitHigh
 * @return
 */
int TrendsGraph::getMaxWeldForceSuspectLimitHigh()
{
    return m_maxWeldForceSuspectLimitHigh;
}

/**
 * @brief TrendsGraph::setMaxWeldForceSuspectLimitHigh
 * @param a
 */
void TrendsGraph::setMaxWeldForceSuspectLimitHigh(int a)
{
    if (a != m_maxWeldForceSuspectLimitHigh)
    {
        m_maxWeldForceSuspectLimitHigh = a;
        emit maxWeldForceSuspectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxWeldForceRejectLimitHigh
 * @return
 */
int TrendsGraph::getMaxWeldForceRejectLimitHigh()
{
    return m_maxWeldForceRejectLimitHigh;
}

/**
 * @brief TrendsGraph::setMaxWeldForceRejectLimitHigh
 * @param a
 */
void TrendsGraph::setMaxWeldForceRejectLimitHigh(int a)
{
    if (a != m_maxWeldForceRejectLimitHigh)
    {
        m_maxWeldForceRejectLimitHigh = a;
        emit maxWeldForceRejectLimitHighChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxWeldForceRejectLimitLow
 * @return
 */
int TrendsGraph::getMaxWeldForceRejectLimitLow()
{
    return m_maxWeldForceRejectLimitLow;
}

/**
 * @brief TrendsGraph::setMaxWeldForceRejectLimitLow
 * @param a
 */
void TrendsGraph::setMaxWeldForceRejectLimitLow(int a)
{
    if (a != m_maxWeldForceRejectLimitLow)
    {
        m_maxWeldForceRejectLimitLow = a;
        emit maxWeldForceRejectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::getMaxWeldForceSuspectLimitLow
 * @return
 */
int TrendsGraph::getMaxWeldForceSuspectLimitLow()
{
    return m_maxWeldForceSuspectLimitLow;
}

/**
 * @brief TrendsGraph::setMaxWeldForceSuspectLimitLow
 * @param a
 */
void TrendsGraph::setMaxWeldForceSuspectLimitLow(int a)
{
    if (a != m_maxWeldForceSuspectLimitLow)
    {
        m_maxWeldForceSuspectLimitLow = a;
        emit maxWeldForceSuspectLimitLowChanged();
    }
}

/**
 * @brief TrendsGraph::clearModel
 */
void TrendsGraph::clearModel()
{
    if (!m_list.empty())
    {
        auto tempList = m_list;
        m_list.clear();
        context->setContextProperty("trendsModel", QVariant::fromValue(m_list));
        context->setContextProperty("cycleCounterModel", QVariant::fromValue(m_list));
        qDeleteAll(tempList);
        tempList.clear();
    }
}
