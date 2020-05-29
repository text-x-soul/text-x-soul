#include "QSoulStreamReader.h"
#include "QTextStream"
#include <QFileInfo>
#include <QDebug>

inline bool in_array(const QString &value, const std::vector<QString> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

QSoulStreamReader::QSoulStreamReader(){
    m_SoulParser = new QSoulParser;
}

QSoulStreamReader::QSoulStreamReader(const QString &filename){
    setFileName(filename);
}

/* If you are thinking about why Im removing comments so many times, this is to be ensured there is no occurence while reading.
* "Comments are removed at maximum while reading to not affect stream".
*/
void QSoulStreamReader::startStream(){
    m_SoulParser = new QSoulParser;
    QFile file(m_filename);
    QFile varFile(m_filename);
    QFile groupFile(m_filename);
    QFile groupVarFile(m_filename);

    QFileInfo fi(m_filename);
    QString ext = fi.completeSuffix();

    if(in_array(ext, soul_confextensions) != true){
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QFile::Text) or !varFile.open(QIODevice::ReadOnly | QFile::Text)
            or !groupFile.open(QIODevice::ReadOnly | QFile::Text) or !groupVarFile.open(QIODevice::ReadOnly | QFile::Text)){
        qDebug() << "SOUL: No Read Access";
    }
    else{
        QTextStream info_in(&file);
        QTextStream in(&varFile);
        QTextStream group_in(&groupFile);
        QTextStream group_varin(&groupVarFile);

        while(!info_in.atEnd()){
            QString line = info_in.readLine().replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "");
            QRegularExpressionMatch match = m_SoulParser->PrePro_List.match(line);
            if(match.hasMatch()){
                QRegularExpressionMatch infoMatch = m_SoulParser->varMatch.match(match.captured(1));
                if(infoMatch.hasMatch()){
                    if(infoMatch.captured(2).contains("Â")){
                        preprocessed_info.insert(infoMatch.captured(1), infoMatch.captured(2).replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "").remove("Â").trimmed());
                    }
                    else{
                        preprocessed_info.insert(infoMatch.captured(1), infoMatch.captured(2).trimmed());
                    }
                }
            }
        }
        file.close();

        while(!in.atEnd()){
            QString line = in.readLine().replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "");
            QRegularExpressionMatch infoMatch = m_SoulParser->PrePro_List.match(line);
            QRegularExpressionMatch groupMatchness = m_SoulParser->groupMatch.match(line);
            QRegularExpressionMatch groupVarMatchness = m_SoulParser->groupVarMatch.match(line);
            QRegularExpressionMatch match = m_SoulParser->varMatch.match(line);

            if(infoMatch.hasMatch()){
                continue;
            }
            else if(groupMatchness.hasMatch()){
                continue;
            }
            else if(groupVarMatchness.hasMatch()){
                continue;
            }
            else if(match.hasMatch()){
                varMap.insert(match.captured(1).trimmed(), match.captured(2).replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "").remove("Â").trimmed());
            }
        }
        varFile.close();

        while(!group_in.atEnd()){
            QString line = group_in.readLine().replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "");
            QRegularExpressionMatch groupMatchness = m_SoulParser->groupMatch.match(line);
            if(groupMatchness.hasMatch()){
                QString group_value = groupMatchness.captured(2).trimmed().remove("{").remove("}").replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "").remove("Â").trimmed();
                QStringList group_value_list = group_value.split(",");
                group_value_list.replaceInStrings(QRegExp("^\\s+|\\s+$"), "");
                group_value_list.replaceInStrings(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "");
                group_value_list.replaceInStrings("Â", "");
                group_value_list.replaceInStrings(QRegExp("^\\s+|\\s+$"), "");
                groupMap.insert(groupMatchness.captured(1).replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "").remove("Â").trimmed(), group_value_list);
                for(int i = 0; i < group_value_list.size(); i++){
                    QStringList groupIdentity;
                    groupIdentity.insert(0, group_value_list.value(i));
                    groupIdentity.insert(1, groupMatchness.captured(1).trimmed().remove("(").remove(")").trimmed());
                    groupVarMap.insert(groupIdentity, "NULL");
                }
            }
        }
        groupFile.close();

        while(!group_varin.atEnd()){
            QString line = group_varin.readLine().replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "");
            QRegularExpressionMatch groupvar_matchness = m_SoulParser->groupVarMatch.match(line);
            if(groupvar_matchness.hasMatch()){
                QString group_var = groupvar_matchness.captured(1).trimmed().remove(" ").remove(QRegularExpression("(@\(.*\))"));
                QString belongs_To = groupvar_matchness.captured(2).trimmed().remove(" ").remove("(").remove(")").replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "").remove("Â").trimmed();
                QString group_var_value = groupvar_matchness.captured(3).replace(QRegularExpression("(¿.*\?\\/)|(¿.*)"), "").remove("Â").trimmed();
                QStringList groupIdentity;
                groupIdentity.insert(0, group_var);
                groupIdentity.insert(1, belongs_To);
                groupVarMap.insert(groupIdentity, group_var_value);
            }
        }
        groupVarFile.close();
    }
}

void QSoulStreamReader::setFileName(QString filename){
    m_filename = filename;
    startStream();
}

QString QSoulStreamReader::getFileName(){
    return m_filename;
}

int QSoulStreamReader::typeOf(QString key, SoulMaps map){
    int result = 0;
    if(map == INFOMAP){
        QString val = preprocessed_info.value(key);
        if(m_SoulParser->isNumber.exactMatch(val)){
            result = m_SoulParser->NUMBER;
        }
        else{
            result = m_SoulParser->SOUL_STRING;
        }
    }
    else if(map == VARMAP){
        QString val = varMap.value(key);
        if(m_SoulParser->isNumber.exactMatch(val)){
            result = m_SoulParser->NUMBER;
        }
        else{
            result = m_SoulParser->SOUL_STRING;
        }
    }

    return result;
}

QString QSoulStreamReader::soulVersion() const{
    if(preprocessed_info.contains("SOUL_VERSION")){
        return preprocessed_info.value("SOUL_VERSION");
    }
    else{
        return QString("NULL");
    }
}

QString QSoulStreamReader::getVarValue(QString key){
    if(varMap.contains(key)){
        return varMap.value(key);
    }
    else{
        return QString("NULL");
    }
}

QStringList QSoulStreamReader::groupDef(QString key){
    if(groupMap.contains(key)){
        return groupMap.value(key);
    }
    else{
        return QStringList();
    }
}

QString QSoulStreamReader::groupParams(QString key, int index){
    if(groupMap.contains(key)){
        return groupMap.value(key).at(index);
    }
    else{
        return QString("NULL");
    }
}

QString QSoulStreamReader::getGrpValue(QString group, QString param){
    QStringList groupKey;
    groupKey.insert(0, param);
    groupKey.insert(1, group);

    if(groupVarMap.contains(groupKey)){
        return groupVarMap.value(groupKey);
    }
    else{
        return QString("NULL");
    }
}

QMap<QString, QString> QSoulStreamReader::infoMap(){
    return preprocessed_info;
}

QMap<QString, QString> QSoulStreamReader::valueMap(){
    return varMap;
}

QMap<QString, QStringList> QSoulStreamReader::grpMap(){
    return groupMap;
}

QMap<QStringList, QString> QSoulStreamReader::grpVarMap(){
    return groupVarMap;
}
