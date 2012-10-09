#ifndef WORDMODEL_H
#define WORDMODEL_H

#include "FileParser.h"
#include <QStringList>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

class WordModel: public QAbstractTableModel
{
  Q_OBJECT

 public:
  WordModel(FileParser *fileParser, QObject *parent = 0);
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;

 private:
  QStringList modelheader;
  FileParser *parserModel;
  QSet<QString> *validKey;
  QMap<QString, QString> *keymap;
};

#endif // WORDMODEL_H
