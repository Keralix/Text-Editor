#include <QtWidgets>
#include "Editor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.resize(320, 240);
    window.setWindowTitle(
        QApplication::translate("texted", "TextEd")
    );

    QVBoxLayout *layout = new QVBoxLayout(&window);
    Editor *editor = new Editor(&window);
    layout->addWidget(editor);

    window.show();
    return app.exec();
}