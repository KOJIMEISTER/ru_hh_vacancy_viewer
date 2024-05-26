#include "main.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	Programm Prog;
	QQmlApplicationEngine engine;
	QQuickStyle::setStyle("Material");
	const QUrl url(u"qrc:/ITD_DESKTOP/main.qml"_qs);
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
			 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.rootContext()->setContextProperty("backend", Prog.GetBackend());
	engine.load(url);

	return app.exec();
}
