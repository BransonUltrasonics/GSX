#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <QObject>
#include <QQmlEngine>
#include <QDebug>
#include <QQmlContext>


class DataFilter : public QObject
{
    Q_OBJECT
public:
    explicit DataFilter(QObject *parent = nullptr);
    DataFilter(QString strFiltername,QStringList strlist);
    DataFilter(QString strFiltername, QStringList strlist,int index);
    DataFilter(QString strSubTitleName,int exportFilterName, int deleteFilterName);

    Q_PROPERTY(QString strFilterName READ getSelectedFilterName WRITE setSelectedFilterName NOTIFY selectedFilterNameChanged)
    Q_PROPERTY(QStringList strFilterList READ getSelectedFilterList WRITE setSelectedFilterList NOTIFY selectedFilterListChanged)
    Q_PROPERTY(int selected_index READ getSelectedIndex WRITE setSelectedIndex NOTIFY selectedFilterIndexChanged)
    Q_PROPERTY(QString strSubTitleName READ getSubTitleName WRITE setSubTitleName NOTIFY subTitleNameChanged)
    Q_PROPERTY(int exportFilterName READ getexportFilterName WRITE setExportFilterName NOTIFY exportFilterNameChanged)
    Q_PROPERTY(int deleteFilterName READ getdeleteFilterName WRITE setdeleteFilterName NOTIFY deleteFilterNameChanged)

    Q_INVOKABLE QString getSelectedFilterName();
    Q_INVOKABLE QStringList getSelectedFilterList();
    Q_INVOKABLE int getSelectedIndex();
    Q_INVOKABLE void setSelectedFilterList(QStringList SelectedFrequencyType);
    Q_INVOKABLE void setSelectedFilterName(QString SelectedReportType);
    Q_INVOKABLE void setSelectedIndex(int);
    int getexportFilterName();
    int getdeleteFilterName();
    void setExportFilterName(int exportName);
    void setSubTitleName(QString subTileNames);
    void setdeleteFilterName(int deleteName);
    QString getSubTitleName();

signals:
    void selectedFilterNameChanged();
    void selectedFilterListChanged();
    void selectedFilterIndexChanged();
    void subTitleNameChanged();
    void exportFilterNameChanged();
    void deleteFilterNameChanged();

private:
    QString m_strFilterName;
    QStringList m_Filterlist;
    QString m_subTileNames;
    int m_FilterIndex;
    int m_exportName;
    int m_deleteName;
};


#endif // DATAFILTER_H
