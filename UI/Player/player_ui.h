#include <QMediaPlayer>
#include <QWidget>

class QAction;
class QLabel;
class QMediaPlaylist;
class QSlider;
class PlayerUI : public QWidget
{
Q_OBJECT

public:
	PlayerUI(QWidget *parent=0);
	void set_playlist(QMediaPlaylist *playlist);
	void set_volume(int);

private:
	void setupActions();
	void setupConnections();
	void setupUI();

public slots:
	void play();

private slots:
	void next();
	void previous();
	void tick(qint64 ms_time);
    void change_song_position(int);
	void media_status_changed(QMediaPlayer::MediaStatus);
	void player_state_changed(QMediaPlayer::State);

public:
	int current_index;
	QMediaPlayer *player_musics;

private:
	QAction *act_play_pause;
	QAction *act_stop;
	QAction *act_next;
	QAction *act_prev;

	QLabel *music_time;

	QMediaPlaylist *playlist_musics;

	QSlider *slider_position;
};
