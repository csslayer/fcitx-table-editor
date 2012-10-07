#include "FileParser.h"
#include <QtCore>

void FileParser::openFile(QString fileName)
{
  intoData = false;
  intoRole = false;
  if (fileName.isEmpty())
      return;
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
  
  QTextStream in(&file);
  in.setCodec("UTF-8");
  QString line;
  while (!in.atEnd()) {
    line = in.readLine();
    if (line.isEmpty())
      continue;

    processLine(line);
  }
  file.close();
}

void FileParser::pfall()
{
  QTextStream out(stdout);
  out.setCodec("UTF-8");
  out << KeyCode << endl << Length;
}
void FileParser::processLine(const QString text)
{
  QStringList keyVal;
  if (intoData) {
    keyVal = readKeyVal(text);
    if (keyVal.size() <= 1)
      return;
    keymap.append(qMakePair(keyVal.at(0), keyVal.at(1)));
    return;
  }

  QString section = readSection(text);
  if (!section.isEmpty()) {
    if (section == QString::fromUtf8("数据") || section == "Data") {
      intoData = true;
      intoRole = false;
      return;
    }
    if (section == QString::fromUtf8("组词规则")) {
      intoRole = true;
    }
  }

  QStringList conf = readConf(text);
  if (!conf.isEmpty()) {
    if (intoRole == true) {
      QStringList tmp = conf.at(1).split('+');
      wordRole.append(qMakePair(conf.at(0), tmp));
    }
    if (conf.at(0) == QString::fromUtf8("键码") || conf.at(0) == "KeyCode") {
      KeyCode = conf.at(1);
      return;
    }
    if (conf.at(0) == QString::fromUtf8("码长") || conf.at(0) == "Length") {
      Length = conf.at(1);
      return;
    }
    if (conf.at(0) == QString::fromUtf8("拼音")) { 
      pinyinPrefix = conf.at(1);
      return;
    }
      if (conf.at(0) == QString::fromUtf8("拼音长度")) { 
      pinyinLens = conf.at(1);
      return;
    }
  }
}

QStringList FileParser::readKeyVal(const QString text)
{
  QStringList oneKeyVal;
  QString splitWhiteSpace = text;
  splitWhiteSpace.replace('\t', ' ');
  if (splitWhiteSpace.contains(' '))
    oneKeyVal = splitWhiteSpace.split(' ',QString::SkipEmptyParts);
  return oneKeyVal;
}

QStringList FileParser::readConf(const QString text)
{
  QStringList conf;
  if (text.contains('=')) {
    conf = text.split('='); // in this time, conf has 2 str at least
    conf.replaceInStrings(" ", "");
  }
  return conf;
}

QString FileParser::readSection(const QString text)
{
  QString sectionName;
  
  QString trimWhite = text;
  trimWhite.remove(QChar(' '));
  if ((trimWhite.at(0) == '[') &&
      (trimWhite.at(trimWhite.size() -1) == ']')) {
    sectionName = trimWhite.mid(1);
    sectionName = sectionName.left(sectionName.size() - 1);
  }
  return sectionName;
}
