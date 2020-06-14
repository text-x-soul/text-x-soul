#ifndef CUSTOM_H
#define CUSTOM_H
#include <QString>
#include <vector>

inline bool in_array(const QString &value, const std::vector<QString> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}
#endif
