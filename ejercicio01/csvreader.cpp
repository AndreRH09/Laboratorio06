#include "CsvReader.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <iostream>

void CsvReader::readCSV(const QString& filename, BinaryTree& tree) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error al abrir el archivo CSV.\n";
        return;
    }

    QTextStream in(&file);
    in.readLine(); // Ignorar la primera línea de encabezado

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split('\t');

        int customerId = fields[0].toInt();
        QString itemPurchased = fields[3];
        double reviewRating = fields[10].toDouble();

        tree.insert(customerId, itemPurchased, reviewRating);
    }
    file.close();
}
