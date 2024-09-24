#ifndef CUSTOMFILESYSTEMMODEL_H
#define CUSTOMFILESYSTEMMODEL_H

#include <QFileSystemModel>

class CustomFileSystemModel : public QFileSystemModel
{
public:
    explicit CustomFileSystemModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    qint64 calculateFolderSize(const QString &folderPath) const;
};

#endif // CUSTOMFILESYSTEMMODEL_H
