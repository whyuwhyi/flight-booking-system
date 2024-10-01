#include <QApplication>
#include <ui/WindowManager.h>
#include <ui/MainWindow.h>
#include <ui/ServerWindow.h>
#include <QIcon>
#include <cstring>


int main(int argc, char *argv[]) {
    if(argc>1 && !strcmp(argv[1], "--server")) {
        QApplication app(argc, argv);
        app.setWindowIcon(QIcon(":/icons/app.svg"));
        app.setApplicationName("flight system management");
        ServerWindow serverWindow;
        serverWindow.show();

        return app.exec();
    }

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app.svg"));
    app.setApplicationName("flight system");
    WindowManager window;
    window.initFrame();


    return app.exec();
}
