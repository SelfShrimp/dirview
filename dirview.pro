QT += widgets
requires(qtConfig(treeview))

SOURCES       = main.cpp \
    customfilesystemmodel.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/dirview
INSTALLS += target

HEADERS += \
    customfilesystemmodel.h
