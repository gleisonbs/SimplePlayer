#include <QApplication>

#include "UI/Playlist/playlist_ui.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("SimplePlayer");

	PlaylistUI playlist;
	playlist.setGeometry(300, 300, 600, 600);
	playlist.show();

	app.exec();
}
