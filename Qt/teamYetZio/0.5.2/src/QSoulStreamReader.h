#ifndef QSOULSTREAMREADER_H
#define QSOULSTREAMREADER_H

#include "QSoulParser.h"
#include <QMap>
#include <QFile>

/// @brief Standard Qt-based soul configuration file reader.
class QSoulStreamReader : QFile{
public:
    explicit QSoulStreamReader();
    explicit QSoulStreamReader(const QString &filename);

    enum SoulMaps{
        INFOMAP,
        VARMAP
    };
    
    /// @brief Function that runs on constructing or reading a soul configuration file.
    void startStream();

    /// @brief Returns a QSoulParser::TypeIdentifier int value for a value identified by its key.
    int typeOf(QString key, SoulMaps map);

    /// @brief Sets the current Soul File Document name.
    void setFileName(QString filename);

    /// @brief Returns the current Soul File Document name.
    QString getFileName();

    /// @brief Returns the Soul File Document version.
    QString soulVersion() const;

    /// @brief Returns the value in variable map identified by its key. The key is the variable.
    QString getVarValue(QString key);

    /// @brief Returns the group definition identified by its key. The key is the group name.
    QStringList groupDef(QString key);

    /// @brief Returns the group parameters identified by its key at index. The key is the group name.
    QString groupParams(QString key, int index);

    /// @brief Returns the Soul File Document Pre-Processed Information map.
    QMap<QString, QString> infoMap();

    /// @brief Returns the Soul File Document value or variable map.
    QMap<QString, QString> valueMap();

    /// @brief Returns the Soul File Document group map.
    QMap<QString, QStringList> grpMap();

    /// @brief Returns the Soul File Document group Variable map.
    QMap<QStringList, QString> grpVarMap();

    /// @brief Returns the Soul File Document group values.
    QString getGrpValue(QString group, QString param);
private:
    QString m_filename;
    QSoulParser *m_SoulParser;

    QMap<QString, QString> preprocessed_info;
    QMap<QString, QStringList> groupMap;
    QMap<QStringList, QString> groupVarMap;
    QMap<QString, QString> varMap;
};
#endif // QSOULSTREAMREADER_H
