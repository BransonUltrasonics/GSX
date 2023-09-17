/**************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018

     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

    Handels single report and continous report functionality

***************************************************************************/
#include "header/datafilter.h"

extern QQmlContext* context;

/**
 * @brief DataFilter::DataFilter : Constructor
 * @param parent
 */
DataFilter::DataFilter(QObject *parent) : QObject(parent)
{
    m_strFilterName.clear();
    m_Filterlist.clear();
}

/**
 * @brief DataFilter::DataFilter  :Parameterized constructor
 * @param strSubTitleName
 * @param exportFilterName
 * @param deleteFilterName
 */
DataFilter::DataFilter(QString strSubTitleName, int exportFilterName, int deleteFilterName)
{
    m_subTileNames = strSubTitleName;
    m_exportName= exportFilterName;
    m_deleteName = deleteFilterName;
}

/**
 * @brief DataFilter::DataFilter : Parmeterized constructor
 * @param strFiltername
 * @param strlist
 */
DataFilter::DataFilter(QString strFiltername,QStringList strlist)
{
    m_strFilterName = strFiltername;
    m_Filterlist = strlist;
}

/**
 * @brief DataFilter::DataFilter : Parmeterized constructor
 * @param strFiltername
 * @param strlist
 * @param index
 */
DataFilter::DataFilter(QString strFiltername,QStringList strlist,int index)
{
    m_strFilterName = strFiltername;
    m_Filterlist = strlist;
    m_FilterIndex = index;
}

/**
 * @brief DataFilter::getSelectedFilterList : Returns selected filter list
 * @return
 */
QStringList DataFilter::getSelectedFilterList()
{
    return m_Filterlist;
}

/**
 * @brief DataFilter::setSelectedFilterList : Sets selected filter list
 * @param strFilterList
 */
void DataFilter::setSelectedFilterList(QStringList strFilterList)
{
    m_Filterlist.clear();
    m_Filterlist = strFilterList;
    emit selectedFilterListChanged();
}

/**
 * @brief DataFilter::getSelectedFilterName : Returns selected filter name
 * @return
 */
QString DataFilter::getSelectedFilterName()
{
    return m_strFilterName;
}

/**
 * @brief DataFilter::setSelectedFilterName : Sets selected filter name
 * @param strFilterName
 */
void DataFilter::setSelectedFilterName(QString strFilterName)
{
    m_strFilterName = strFilterName;
    emit selectedFilterNameChanged();
}

/**
 * @brief DataFilter::setSelectedIndex : Sets selected filter index
 * @param selected_index
 */
void DataFilter::setSelectedIndex(int selected_index)
{
    m_FilterIndex  = selected_index;
    emit selectedFilterIndexChanged();
}

/**
 * @brief DataFilter::getexportFilterName : Returns exported filter name
 * @return
 */
int DataFilter::getexportFilterName()
{
    return m_exportName;
}

/**
 * @brief DataFilter::setExportFilterName : Sets exported filter name
 * @param exportName
 */
void DataFilter::setExportFilterName(int exportName)
{
    m_exportName = exportName;
}

/**
 * @brief DataFilter::getSubTitleName : Returns subtitle name
 * @return
 */
QString DataFilter::getSubTitleName()
{
    return m_subTileNames;
}

/**
 * @brief DataFilter::setSubTitleName : Sets subtitle name
 * @param subTileNames
 */
void DataFilter::setSubTitleName(QString subTileNames)
{
    m_subTileNames = subTileNames;
}

/**
 * @brief DataFilter::getdeleteFilterName : Returns delete filter name
 * @return
 */
int DataFilter::getdeleteFilterName()
{
    return m_deleteName;
}

/**
 * @brief DataFilter::setdeleteFilterName : Sets delete filter name
 * @param deleteName
 */
void DataFilter::setdeleteFilterName(int deleteName)
{
    m_deleteName = deleteName;
}

/**
 * @brief DataFilter::getSelectedIndex : Returns selected index
 * @return
 */
int DataFilter::getSelectedIndex()
{
    return m_FilterIndex  ;
}
