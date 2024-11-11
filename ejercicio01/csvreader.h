#ifndef CSVREADER_H
#define CSVREADER_H

#include "BinaryTree.h"
#include <QString>

class CsvReader {
public:
    static void readCSV(const QString& filename, BinaryTree& tree);
};

#endif // CSVREADER_H
