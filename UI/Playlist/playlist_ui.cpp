#include <QAction>
#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QSlider>
#include <QToolBar>
#include <QWidget>

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "playlist_ui.h"
#include "../../utilities/utilities.h"

using std::to_string;

PlaylistUI::PlaylistUI(QWidget *parent) : QWidget(parent)
{
	player = new PlayerUI;
	playlist_musics = new QMediaPlaylist;

	setupActions();
	setupUI();
	setupConnections();
}

void PlaylistUI::add_musics()
{
	QStringList music_list = QFileDialog::getOpenFileNames(this, tr("Select mp3 files"), "", tr("mp3 files (*.mp3)"));
	std::cout << table_musics->rowCount() << std::endl;
	int row = table_musics->rowCount();
	table_musics->setRowCount(table_musics->rowCount()+music_list.size());
	for(auto music : music_list)
	{
		std::string music_str = music.toStdString();
		music_str = music_str.substr(music_str.find_last_of("\\/")+1);

		SongInfo info = read_tags(music.toStdString());

		song_info[row] = info;
		fill_playlist_row(row);
		row++;
	}

	table_musics->resizeColumnsToContents();

	/*int cw1 = table_musics->columnWidth(0);
	int cw2 = table_musics->columnWidth(1);
	int cw3 = table_musics->columnWidth(2);
	while(cw1+cw2+cw3 < table_musics->width())
	{
		table_musics->setColumnWidth(0, cw1+1);
		table_musics->setColumnWidth(1, cw2+1);
		table_musics->setColumnWidth(2, cw3+1);

		cw1 = table_musics->columnWidth(0);
		cw2 = table_musics->columnWidth(1);
		cw3 = table_musics->columnWidth(2);
	}*/

	for(auto music : music_list)
		playlist_musics->addMedia(QUrl::fromLocalFile(music));
	player->set_playlist(playlist_musics);
}

void PlaylistUI::fill_playlist_row(int row)
{
	QTableWidgetItem *song_title = new QTableWidgetItem(QString::fromStdString(song_info[row].title));
	song_title->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	QTableWidgetItem *song_artist = new QTableWidgetItem(QString::fromStdString(song_info[row].artist));
	song_artist->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	QTableWidgetItem *song_album = new QTableWidgetItem(QString::fromStdString(song_info[row].album));
	song_album->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	table_musics->setItem(row, 0, song_title);
	table_musics->setItem(row, 1, song_artist);
	table_musics->setItem(row, 2, song_album);
}


SongInfo PlaylistUI::read_tags(const std::string &path_to_music)
{
	std::ifstream music(path_to_music, std::ios::binary|std::ios::ate);
	std::size_t music_size = music.tellg();
	music.seekg(music_size-128);

	std::stringstream music_info_ss;
	music_info_ss << music.rdbuf();
	std::string music_info = music_info_ss.str();

	//for ID3

	std::string music_title = music_info.substr(3, 30);
	std::string music_artist = music_info.substr(33, 30);
	std::string music_album = music_info.substr(63, 30);
	std::string album_year = music_info.substr(93, 4);
	std::string album_comment = music_info.substr(97, 30);
	std::string zero_byte = music_info.substr(127, 1);
	std::string track = music_info.substr(128, 1);
	//std::string genre = music_info.substr(129, 1);

	SongInfo info;
	info.path_to_song = path_to_music;

	const char *c = music_title.c_str();
	std::string m_title(c);
	if(m_title.empty())
	{
		m_title = get_file_name(path_to_music);
		m_title = m_title.substr(0, m_title.length()-4);
	}
	info.title = m_title;

	c = music_artist.c_str();
	std::string m_artist(c);
	info.artist = m_artist;

	c = music_album.c_str();
	std::string m_album(c);
	info.album = m_album;

	std::cout << music_title << std::endl
			  << music_artist << std::endl
			  << music_album << std::endl
			  << album_year << std::endl
			  << album_comment << std::endl
			  << zero_byte << std::endl
			  << track << std::endl
			  //<< genre << std::endl
			  << std::endl;

	return info;
}

void PlaylistUI::setupActions()
{
	act_add_musics = new QAction(QIcon("icons/plus.png"), tr("&Add"), this);
	act_clear_musics = new QAction(QIcon("icons/clear.png"), tr("&Clear"), this);
}

void PlaylistUI::setupConnections()
{
	connect(act_add_musics, SIGNAL(triggered()), this, SLOT(add_musics()));
	connect(act_clear_musics, SIGNAL(triggered()), this, SLOT(clear_playlist()));
	connect(volume_slider, SIGNAL(valueChanged(int)), this, SLOT(change_volume(int)));
	connect(player->player_musics, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(change_titlebar(QMediaPlayer::MediaStatus)));
	connect(table_musics, SIGNAL(cellClicked(int, int)), this, SLOT(set_current_index(int, int)));
	connect(table_musics, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(play_at(int, int)));
}

void PlaylistUI::setupUI()
{
	main_layout = new QGridLayout(this);

	tool_bar = new QToolBar();
	tool_bar->setOrientation(Qt::Orientation::Vertical);
	tool_bar->addAction(act_add_musics);
	tool_bar->addAction(act_clear_musics);


	table_musics = new QTableWidget(0, 3);
	table_musics->setSortingEnabled(false);
	set_playlist_table();

	volume_slider = new QSlider(Qt::Vertical);
	volume_slider->setRange(0,100);
	volume_slider->setValue(100);
	volume_slider->setTickInterval(1);

	main_layout->addWidget(tool_bar, 0, 1);
	main_layout->addWidget(table_musics, 0, 0, 3, 1);
	main_layout->setRowStretch(1, 1);
	main_layout->addWidget(volume_slider, 2, 1);
	main_layout->addWidget(player, 3, 0);

	setLayout(main_layout);
}

//dummy values, only so the SLOT() can be compatible with the signal
void PlaylistUI::set_current_index(int row, int dummy_col)
{
	std::cout << "PlaylistUI::set_current_index(): " << table_musics->currentRow() << std::endl;
	player->current_index = row;
}

void PlaylistUI::change_titlebar(QMediaPlayer::MediaStatus dummy_status)
{
	std::cout << playlist_musics->currentIndex();
	std::cout << ": ";
	std::cout << playlist_musics->currentMedia().canonicalUrl().toString().toStdString() << std::endl;
	QWidget::setWindowTitle(QString::fromStdString(to_string(playlist_musics->currentIndex()+1)) +
			QString(": ") + QString::fromStdString(song_info[playlist_musics->currentIndex()].artist) +
			QString(" - ") + QString::fromStdString(song_info[playlist_musics->currentIndex()].title));
}

void PlaylistUI::change_volume(int volume)
{
	player->set_volume(volume);
}

void PlaylistUI::clear_playlist()
{
	table_musics->clear();
	table_musics->setRowCount(0);
	playlist_musics->clear();
	set_playlist_table();
}

void PlaylistUI::set_playlist_table()
{
	//table_musics->horizontalHeader()->->setStretchLastSection(true);

	QStringList headers;
	headers 
			<< "Title"
			<< "Artist"
			<< "Album";
	table_musics->setHorizontalHeaderLabels(headers);
	table_musics->setSelectionBehavior(QAbstractItemView::SelectRows);
	auto header = table_musics->horizontalHeader();
	header->setStretchLastSection(true);
}

void PlaylistUI::play_at(int row, int col)
{
	player->player_musics->stop();
	set_current_index(row, col);
	player->play();
}

