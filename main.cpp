#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QHeaderView>
#include <QLineEdit>
#include <QScopedPointer>
#include <QScreen>
#include <QScroller>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

#include <customfilesystemmodel.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Qt Dir View Example");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption dontUseCustomDirectoryIconsOption("c", "Set QFileSystemModel::DontUseCustomDirectoryIcons");
    parser.addOption(dontUseCustomDirectoryIconsOption);
    QCommandLineOption dontWatchOption("w", "Set QFileSystemModel::DontWatch");
    parser.addOption(dontWatchOption);
    parser.addPositionalArgument("directory", "The directory to start in.");
    parser.process(app);

    CustomFileSystemModel model;
    model.setRootPath("");
    //hidden files and dir without self and parent link
    model.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

    if (parser.isSet(dontUseCustomDirectoryIconsOption)) {
        model.setOption(QFileSystemModel::DontUseCustomDirectoryIcons);
    }
    if (parser.isSet(dontWatchOption)) {
        model.setOption(QFileSystemModel::DontWatchForChanges);
    }

    QWidget window;
    QScopedPointer<QVBoxLayout> layout(new QVBoxLayout(&window));
    QLineEdit filterLineEdit;
    filterLineEdit.setPlaceholderText("Search");
    layout->addWidget(&filterLineEdit);

    QTreeView tree;
    tree.setModel(&model);
    layout->addWidget(&tree);

    const QString ROOT_PATH = QDir::home().path();
    if (!ROOT_PATH.isEmpty()) {
        const QModelIndex rootIndex = model.index(QDir::cleanPath(ROOT_PATH));
        if (rootIndex.isValid())
            tree.setRootIndex(rootIndex);
    }

    // Demonstrating look and feel features
    tree.setAnimated(false);
    tree.setIndentation(20);
    tree.setSortingEnabled(true);
    const QSize availableSize = tree.screen()->availableGeometry().size();
    tree.resize(availableSize / 2);
    tree.setColumnWidth(0, tree.width() / 3);

    // Make it flickable on touchscreens
    QScroller::grabGesture(&tree, QScroller::TouchGesture);

    QObject::connect(&filterLineEdit, &QLineEdit::textChanged, [&model](const QString &text) {
        model.setNameFilters(QStringList() << "*" + text + "*");
        //model.setNameFilterDisables(false);
    });

    window.setWindowTitle(QObject::tr("Dir View"));
    window.resize(800, 600);
    window.show();

    return app.exec();
}
