#include <QApplication>
#include <ui/client/WindowManager.h>
#include <ui/client/MainWindow.h>
#include <ui/server/ServerWindow.h>
#include <QIcon>
#include <cstring>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/app.svg"));
    app.setApplicationName("flight system");

    if(argc>1 && !strcmp(argv[1], "--server")) {
        ServerWindow serverWindow;
        serverWindow.show();
        return app.exec();
    }

    WindowManager window;
    window.initFrame();


    return app.exec();
}
