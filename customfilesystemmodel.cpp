#include "customfilesystemmodel.h"

CustomFileSystemModel::CustomFileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
{
}

QVariant CustomFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole && index.column() == 1) {
        QFileInfo fileInfo = this->fileInfo(index);
        if (fileInfo.isDir()) {
            qint64 folderSize = calculateFolderSize(fileInfo.filePath());
            return QString("%1 KB").arg(folderSize / (1024));
        }
    }
    return QFileSystemModel::data(index, role);
}

qint64 CustomFileSystemModel::calculateFolderSize(const QString &folderPath) const {
    qint64 size = 0;
    QDir dir(folderPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);
    for (const QFileInfo &fileInfo : fileList) {
        if (!fileInfo.isSymLink()) {
            size += fileInfo.size();
        }
    }

    QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &subDir : dirList) {
        if (!subDir.isSymLink()) {
            size += calculateFolderSize(subDir.filePath());
        }
    }

    return size;
}

