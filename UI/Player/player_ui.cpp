#include <QAction>
#include <QGridLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSlider>
#include <QTime>
#include <QToolBar>
#include <QWidget>
#include <QTimer>

#include <string>
#include <iostream>

#include "player_ui.h"

using std::to_string;

PlayerUI::PlayerUI(QWidget *parent)
{
	current_index = 0;
	setupActions();
	setupUI();
	setupConnections();
}

void PlayerUI::setupActions()
{
	act_play_pause = new QAction(tr("&Play"), this);
	act_stop = new QAction(tr("&Stop"), this);
	act_next = new QAction(tr("&Next"), this);
	act_prev = new QAction(tr("&Prev"), this);
}

void PlayerUI::setupConnections()
{
	connect(act_play_pause, SIGNAL(triggered()), this, SLOT(play()));
	connect(act_stop, SIGNAL(triggered()), player_musics, SLOT(stop()));
	connect(act_next, SIGNAL(triggered()), this, SLOT(next()));
	connect(act_prev, SIGNAL(triggered()), this, SLOT(previous()));
	connect(player_musics, SIGNAL(positionChanged(qint64)), this, SLOT(tick(qint64)));
	connect(player_musics, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(media_status_changed(QMediaPlayer::MediaStatus)));
	connect(player_musics, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(player_state_changed(QMediaPlayer::State)));
  connect(slider_position, SIGNAL(sliderMoved(int)), this, SLOT(change_song_position(int)));
}

void PlayerUI::setupUI()
{
	player_musics = new QMediaPlayer;

	QToolBar *tlb_actions = new QToolBar;
	tlb_actions->addAction(act_prev);
	tlb_actions->addAction(act_play_pause);
	tlb_actions->addAction(act_stop);
	tlb_actions->addAction(act_next);

	music_time = new QLabel("00:00:00");

	slider_position = new QSlider(Qt::Horizontal);
	slider_position->setTickInterval(1);

	QGridLayout *grid_layout = new QGridLayout;
	grid_layout->addWidget(tlb_actions, 0, 0);
	grid_layout->addWidget(music_time, 1, 1);
	grid_layout->addWidget(slider_position, 1, 0);

	setLayout(grid_layout);
}

void PlayerUI::change_song_position(int pos)
{
  player_musics->setPosition(pos*1000);
}

void PlayerUI::set_volume(int volume)
{
	player_musics->setVolume(volume);
}

void PlayerUI::next()
{
	playlist_musics->next();
	if(player_musics->state() == QMediaPlayer::PlayingState)
		player_musics->play();
}

void PlayerUI::play()
{
	playlist_musics->setCurrentIndex(current_index);
	std::cout << "PlayerUI::play(): " << playlist_musics->currentIndex() << std::endl;
	player_musics->play();
}

void PlayerUI::previous()
{
	playlist_musics->previous();
	if(player_musics->state() == QMediaPlayer::PlayingState)
		player_musics->play();
}

void PlayerUI::set_playlist(QMediaPlaylist *playlist)
{
	playlist_musics = playlist;
	player_musics->setPlaylist(playlist_musics);
}

void PlayerUI::tick(qint64 ms_time)
{
	QTime musicElapsedTime((ms_time/3600000)%3600, (ms_time/60000)%60, (ms_time/1000)%60);
	music_time->setText(musicElapsedTime.toString("hh:mm:ss"));

	slider_position->setValue(ms_time/1000);
}

void PlayerUI::media_status_changed(QMediaPlayer::MediaStatus status)
{
	int song_duration = player_musics->duration()/1000;
	if (status == QMediaPlayer::BufferedMedia)
		slider_position->setRange(0, song_duration);
}

void PlayerUI::player_state_changed(QMediaPlayer::State player_state)
{
	if(player_state == QMediaPlayer::PlayingState)
	{
		act_play_pause->setText("Pause");
		disconnect(act_play_pause, SIGNAL(triggered()), this, SLOT(play()));
		connect(act_play_pause, SIGNAL(triggered()), player_musics, SLOT(pause()));
	}
	else if(player_state == QMediaPlayer::PausedState || player_state == QMediaPlayer::StoppedState)
	{
		act_play_pause->setText("Play");
		disconnect(act_play_pause, SIGNAL(triggered()), player_musics, SLOT(pause()));
		connect(act_play_pause, SIGNAL(triggered()), this, SLOT(play()));
	}
}
