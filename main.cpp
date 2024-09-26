#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QPushButton>
#include <QHeaderView>
#include <QLineEdit>
#include <QScopedPointer>
#include <QScreen>
#include <QScroller>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

#include <customfilesystemmodel.h>

void setupCommandLineParser(QCommandLineParser &parser)
{
    parser.setApplicationDescription("Qt Dir View Example");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption dontUseCustomDirectoryIconsOption("c", "Set QFileSystemModel::DontUseCustomDirectoryIcons");
    parser.addOption(dontUseCustomDirectoryIconsOption);
    QCommandLineOption dontWatchOption("w", "Set QFileSystemModel::DontWatch");
    parser.addOption(dontWatchOption);
    parser.addPositionalArgument("directory", "The directory to start in.");
}

void setupFileSystemModel(CustomFileSystemModel &model)
{
    model.setRootPath("");
    model.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
}

void setupUI(QWidget &window, CustomFileSystemModel &model, QLineEdit &filterLineEdit, QTreeView &tree)
{
    QScopedPointer<QVBoxLayout> layout(new QVBoxLayout(&window));

    filterLineEdit.setPlaceholderText("Search");
    layout->addWidget(&filterLineEdit);

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

    window.setWindowTitle(QObject::tr("Dir View"));
    window.resize(800, 600);
}

void setupFiltering(QLineEdit &filterLineEdit, CustomFileSystemModel &model)
{
    QObject::connect(&filterLineEdit, &QLineEdit::textChanged, [&model](const QString &text) {
        model.setNameFilters(QStringList() << "*" + text + "*");
    });
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    setupCommandLineParser(parser);
    parser.process(app);

    CustomFileSystemModel model;
    setupFileSystemModel(model);

    QWidget window;
    QLineEdit filterLineEdit;
    QTreeView tree;
    setupUI(window, model, filterLineEdit, tree);

    setupFiltering(filterLineEdit, model);

    window.show();
    return app.exec();
}
