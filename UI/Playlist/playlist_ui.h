#ifndef PLAYLIST_UI_H
#define PLAYLIST_UI_H

#include <QWidget>
#include <QTableWidget>

#include <map>
#include <string>

#include "../Player/player_ui.h"

class QAction;
class QGridLayout;
class QMediaPlaylist;
class QToolBar;

class SongInfo
{
public:
	std::string path_to_song;
	std::string title;
	std::string artist;
	std::string album;
	std::string year;
	std::string comment;
	std::string track;
	std::string genre;
};

class PlaylistUI : public QWidget
{
Q_OBJECT

public:
	PlaylistUI(QWidget *parent=0);

private:
	void set_playlist_table();
	void setupConnections();
	void setupActions();
	void setupUI();

private slots:
	void add_musics();
	void change_titlebar(QMediaPlayer::MediaStatus);
	void change_volume(int);
	void clear_playlist();
	SongInfo read_tags(const std::string &);
	void fill_playlist_row(int);
	void set_current_index(int, int);
	void play_at(int, int);


private:
	PlayerUI *player;
	QMediaPlaylist *playlist_musics;

	// <int: row in the table, struct SongInfo: info about the music>
	std::map<int, SongInfo> song_info;

	QAction *act_add_musics;
	QAction *act_clear_musics;

	QGridLayout *main_layout;

	QSlider *volume_slider;

	QTableWidget *table_musics;

	QToolBar *tool_bar;
};

#endif // PLAYLIST_UI_H
