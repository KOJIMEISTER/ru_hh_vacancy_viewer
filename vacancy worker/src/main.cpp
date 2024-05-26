#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include "backend.h"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	Backend backend;
	QQmlApplicationEngine engine;
	QQuickStyle::setStyle("Material");
	const QUrl url(u"qrc:/ITD_AI_DATASET/main.qml"_qs);
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
			 &app, [url](QObject *obj, const QUrl &objUrl) {
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1);
	}, Qt::QueuedConnection);
	engine.rootContext()->setContextProperty("backend", &backend);
	engine.load(url);
	return app.exec();
}
