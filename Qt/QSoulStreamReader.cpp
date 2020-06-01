#include "QSoulStreamReader.h"
#include "utils/custom.h"
#include "QTextStream"
#include <sstream>
#include <QFileInfo>
#include <QDebug>

QSoulStreamReader::QSoulStreamReader(){
    m_SoulParser = new QSoulParser;
}

QSoulStreamReader::QSoulStreamReader(const QString &filename){
    setFileName(filename);
}

void QSoulStreamReader::startStream(){
    m_SoulParser = new QSoulParser;
    QFile file(m_filename);

    QFileInfo fi(m_filename);
    QString ext = fi.completeSuffix();

    if(in_array(ext, soul_confextensions) != true){
        return;
    }

    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        qDebug() << "SOUL: No Read Access";
    }
    else{
        QTextStream in(&file);

        QString Souldata = in.readAll();
        file.close();
        std::string souldata = Souldata.replace(m_SoulParser->commentMatch, "").remove("Â").toStdString();
        std::istringstream lines( souldata );
        std::istringstream varlines( souldata );
        std::istringstream grouplines( souldata );
        std::istringstream groupvarlines( souldata );

        std::string Line;
        QString line;
        while(getline(lines, Line)){
            line = QString::fromStdString(Line);
            QRegularExpressionMatch match = m_SoulParser->PrePro_List.match(line);
            if(match.hasMatch()){
                QRegularExpressionMatch infoMatch = m_SoulParser->varMatch.match(match.captured(1));
                if(infoMatch.hasMatch()){
                    preprocessed_info.insert(infoMatch.captured(1).trimmed(), infoMatch.captured(2).trimmed());
                }
            }
        }

        std::string varLine;
        QString varline;
        while(getline(varlines, varLine)){
            varline = QString::fromStdString(varLine);
            QRegularExpressionMatch infoMatch = m_SoulParser->PrePro_List.match(varline);
            QRegularExpressionMatch groupMatchness = m_SoulParser->groupMatch.match(varline);
            QRegularExpressionMatch groupVarMatchness = m_SoulParser->groupVarMatch.match(varline);
            QRegularExpressionMatch match = m_SoulParser->varMatch.match(varline);

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
                varMap.insert(match.captured(1).trimmed(), match.captured(2).trimmed());
            }
        }

        std::string grpLine;
        QString grpline;
        while(getline(grouplines, grpLine)){
            grpline = QString::fromStdString(grpLine);
            QRegularExpressionMatch groupMatchness = m_SoulParser->groupMatch.match(grpline);
            if(groupMatchness.hasMatch()){
                QString group_value = groupMatchness.captured(2).remove("{").remove("}").trimmed();
                QStringList group_value_list = group_value.split(",");
                group_value_list.replaceInStrings(QRegExp("^\\s+|\\s+$"), "");
                groupMap.insert(groupMatchness.captured(1).remove("(").remove(")").trimmed(), group_value_list);

                for(int i = 0; i < group_value_list.size(); i++){
                    QStringList groupIdentity;
                    groupIdentity.insert(0, group_value_list.value(i));
                    groupIdentity.insert(1, groupMatchness.captured(1).remove("(").remove(")").trimmed());
                    groupVarMap.insert(groupIdentity, "NULL");
                }
            }
        }

        std::string grpvarLine;
        QString grpvarline;
        while(getline(groupvarlines, grpvarLine)){
            grpvarline = QString::fromStdString(grpvarLine);
            QRegularExpressionMatch groupvar_matchness = m_SoulParser->groupVarMatch.match(grpvarline);
            if(groupvar_matchness.hasMatch()){
                QString group_var = groupvar_matchness.captured(1).remove(QRegularExpression("(@\\(.*\\))")).trimmed();
                QString belongs_To = groupvar_matchness.captured(2).remove("(").remove(")").trimmed();
                QString group_var_value = groupvar_matchness.captured(3).trimmed();
                QStringList groupIdentity;
                groupIdentity.insert(0, group_var);
                groupIdentity.insert(1, belongs_To);
                groupVarMap.insert(groupIdentity, group_var_value);
            }
        }
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
