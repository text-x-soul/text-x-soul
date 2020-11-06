#ifndef QSOULPARSER_H
#define QSOULPARSER_H

#include <vector>
#include <QObject>
#include <QString>
#include <QRegularExpression>

#define MEDIA_TYPE_SOUL = "text/soul"
#define SOUL_PARSER_VERSION = "0.5.2"
#define SOUL_UTI = "public.soul"

inline std::vector<QString> soul_confextensions {"soul", "SOUL", "sOUL", "SoUL", "SOuL", "SOUl"};

class QSoulParser: public QObject{
public:
    explicit QSoulParser(QObject *parent = nullptr);


    enum TypeIdentifiers{
        UNKNOWN = 0,
        SOUL_STRING = 1,
        NUMBER = 2,
        GROUP = 3,
        COMMENTS = 4,
        INFO = 5
    };

    QRegExp isNumber = QRegExp("\\d*");
    QRegularExpression PrePro_List = QRegularExpression("(?<=¡)(.*)(?=¡)");
    QRegularExpression varMatch = QRegularExpression("([^\\s]+)\\s*=\\s*(.*)");
    QRegularExpression groupMatch = QRegularExpression("([^\\s]+[()])\\s*=\\s*({.*})");
    QRegularExpression groupVarMatch = QRegularExpression("([^\\s]+@(.*))\\s*=\\s*(.*)");
    QRegularExpression commentMatch = QRegularExpression("¿([\\S\\s]*?)\\?");

    QString group_notation = "@";
    QString containers[2] = {"(", ")"};
    QString comment_containers[2] = {"¿", "?"};
    QString info_container = "¡";
};

#endif // QSOULPARSER_H
