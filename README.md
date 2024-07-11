# Project-1-Playlist-Manager
create a command line tool to maintain a music library and associated playlists. 
1.1 File Structure
You will be working with two files: one file will be to store the music library, and one file will be to store all
of your playlists.
Music library files are in a standard format called TSV format (Tab Separated Values). TSV files have
all elements of a table separated with a tab character, which is written as \t. Music libraries will have one
1
song per line. Each line will have the artist name, a tab, the song title, a tab, the artist genre as stored in
Spotify, a tab, and finally, the spotify URL. It will look like this (yes, they can be so long they go off the
page):
Goodnight, Texas \t Hypothermic \t rebel blues \t https://open.spotify.com/track/0qjF1qVQ2L15hG2aZUAWeK
Robot Koch, Finnegan Tui \t In Between \t wonky \t https://open.spotify.com/track/5KJIJsnEaPdq2DeWlZOtSZMadalen Duke \t Love into a Weapon \t canadian electropop,dark r&b \t https://open.spotify.com/track/2KCHi´erophante \t Timeline \t future bass \t https://open.spotify.com/track/40jbcGQJ5oRgnczPYlfh8s
There are a couple music library files available on Canvas that you can use to test things. You can also
create your own music library files following the steps at the end of this document.
Playlist files will be easier to work with. Each playlist will begin with its title, followed by a series of
integers. These integers will be the internal song identification numbers or Song IDs. This is just which
number the song is in your library, where 0 is the first song in the library, 1 is the second song in the library,
so on and so forth. This will make it easy to use arrays. A single playlist might look like this:
Test Playlist
1
4
10
2
This playlist would be titled “Test Playlist” and would have the second song in your library, followed by
the fifth song in your library, followed by the eleventh, and ending with the third song.
Everything will be on its own line in the playlist file. Playlists will be listed one after each other, so they
will look like this:
Test Playlist
1
4
10
2
Second Playlist
3
89
2
Another Example
16
2
19
30
29
18
1.2 Important variables
Here is an overview of the significant variables you will need for this project. You are not limited to these
variables, but you will need them.
1.2.1 Global Variables
This project will introduce Global Variables. There are only two that you should use, and those are to
decide the maximum number of songs you can have as well as the maximum number of playlists. This is to
help with debugging as well as to give you flexibility to change the number of songs or playlists you allow in
the future. These global variables will be used to control your array size.
For this project, we will want the following two constant global variables:
2
const int MAX_SONGS = 1000;
const int MAX_PLAYLISTS = 20;
The maximum number of songs is both the maximum number of songs in your library and the maximum
number in any given playlist.
You will create functions to read and write these files, as well as make simple changes such as creating a
new empty playlist, adding or removing songs, and reordering songs. The bulk of the work will be in creating
the UI, which will be a menu for the user to choose which functions to perform on their music library and
playlists.
1.2.2 Counting Variables
You will need to keep track of the number of songs in your library as well as the number of playlists you
have. Both of these variables will be integers. You can call them whatever you like in your code, but for the
purposes of this write up and all other documentation on the project, these names are used:
int librarySize; //number of songs in the library
int numPlaylists; //number of playlists
1.2.3 Array Variables
All of the information about our music library and playlists will be read into array variables. There are four
arrays associated with the music library:
• string artists[MAX_SONGS] will be an array to store the artist names for all the songs in your library.
• string titles[MAX_SONGS] will be an array to store the titles of all the songs in your library.
• string genres[MAX_SONGS] will be an array to store the genres of all the songs in your library.
• string urls[MAX_SONGS] will be an array to store the spotify URLS for all the songs in your library.
Index values will correspond with a particular song in your library across all of these arrays. We will use
the index values to identify which song we are talking about in our code. For example, you could find the
artist for song 0 using artists[0] and the title using titles[0].
You will use the index values of the songs to organize your playlists as well. There are two arrays for
your playlists:
• string playlistNames[MAX_PLAYLISTS] is an array of all the playlist titles.
• int playlists[MAX_PLAYLISTS][MAX_SONGS] is a 2D array of the songs in each playlist. The rows
correspond to one playlist each, while the columns are the songs in each position in the playlist. Any
empty positions in your playlist array will store -1.
The index values used to get the playlist name will correspond to the row in the playlists[][] array.
The first index value will correspond with which playlist you are looking at, and the second index value will
correspond with which song in the playlist you are looking at. For example, you could get the name of the
first playlist using playlistNames[0] and the ID of the first song in that playlist using playlists[0][0],
or the second song using playlists[0][1].
The integer values stored in this array will be the index values of the song in your library. So, if the first
song of the first playlist playlists[0][0] stored the integer value 6, we could find the title of that song
using titles[6] or, equivalently, titles[playlists[0][0]].
3
2 Required Functions
Here is a list of all the function prototypes that you will need. These are all available in the starting cpp file
on Canvas:
//Include
#include <iostream>
#include <fstream>
using namespace std;
const int MAX_SONGS = 1000;
const int MAX_PLAYLISTS = 20;
//Functions
//Read in the music library, return num songs
int ReadLibrary(string inputFile, string artists[],
string titles[], string genres[], string urls[]);
//Read in the playlists, return num playlists
int ReadPlaylists(string inputFile, int playlists[][MAX_SONGS],
string playlistNames[]);
//Make a new playlist
int NewPlaylist(int numPlaylists, string playlistNames[],
string newName);
//Add a song to the library
int AddSongLibrary(int librarySize, string newArtist, string newTitle,
string newGenre, string newURL, string artists[],
string titles[], string genres[], string urls[]);
//Add a song to a playlist
bool AddSongPlaylist(int numPlaylists, int playlists[][MAX_SONGS],
int songID, int playlistID);
//Delete a song from a playlist
bool DeleteSongPlaylist(int numPlaylists, int playlists[][MAX_SONGS],
int songID, int playlistID);
//Delete a song from the library
int DeleteSongLibrary(int librarySize, string artists[], string titles[],
string genres[], string urls[], int songID, int playlists[][MAX_SONGS]);
//Swap two songs in a playlist
bool SwapSongs(int numPlaylists, int playlists[][MAX_SONGS],
int songID1, int songID2, int playlistID);
//Move a song to a new location in playlist
bool MoveSong(int numPlaylists, int playlists[][MAX_SONGS],
int songID, int playlistID, int newLocation);
//find song ID
int FindSongID(string artists[], string titles[], string genres[],
int librarySize);
4
//Print playlists in a spotify importable way
void PrintSpotify(int numPlaylists, int playlists[][MAX_SONGS],
int playlistID, string urls[]);
//Print the playlists just with artists+titles
void PrintPlaylist(int numPlaylists, int playlists[][MAX_SONGS],
int playlistID, string artists[], string titles[]);
//Close the program (save changes)
void SaveChanges(string libraryFile, string playlistFile, int librarySize,
int numPlaylists, string artists[], string titles[], string genres[],
string urls[], int playlists[][MAX_SONGS], string playlistNames[]);
A good first step would be to download the starting cpp file from Canvas. Check to make sure everything
compiles.
You are allowed to create and use any extra helper functions you would like. I used three helper functions:
the split function from your last homework, the integer validation function we made in class, and a search
for matching substrings function. These functions are not required, but you may find them useful.
2.1 ReadLibrary ⋆⋆
This function is to read in a library file and populate an array for the artists, titles, genres, and urls. The
function declaration should look like this:
int ReadLibrary(string inputFile, string artists[],
string titles[], string genres[], string urls[]);
Return: the number of songs in the library
You will need to open the provided input file, read in each line, split the line into four sections divided by
tabs (hint: the SPLIT function would be helpful here!), and store the song information in the appropriate
arrays.
You do not need to perform validation checks when you open the input file. We will do this in your main
function instead. If there is an error reading any line and splitting it into 4 elements, skip the line and state
”Error reading line. Song skipped.”
2.2 ReadPlaylists ⋆⋆
This function is to read in your playlist file and populate an array of the songs in each playlist, as well as
an array of the playlist titles. The function declaration should look like this:
int ReadPlaylists(string inputFile, int playlists[][MAX_SONGS],
string playlistNames[]);
Return: the number of playlists in the file.
This function will open the playlist file and read in the stored information. You should store each
playlist name in the array playlistNames, and all subsequently listed songs in the associated row of the
playlists[][] array. The playlists[][] array should have the first coordinate represent which playlist
we are accessing, and the second coordinate will represent which song is stored in that position.
Once properly populating these two arrays the function should return the number of playlists read in
from the file.
5
2.3 NewPlaylist⋆
This function will create a new empty playlist. The function declaration should look like this:
int NewPlaylist(int numPlaylists, string playlistNames[],
string newName);
Return: the new number of playlists.
Since the playlist will be empty when it is created, all we have to do is add the newName to our
playlistNames[] in the first unoccupied spot.
You will need to verify that there is space to do so (i.e., numPlaylists < MAX_PLAYLISTS).
2.4 AddSongLibrary ⋆
This function is to add a song to the end of our music library. The function declaration should look like this:
int AddSongLibrary(int librarySize, string newArtist, string newTitle,
string newGenre, string newURL, string artists[],
string titles[], string genres[], string urls[]);
Return: the new library size
We will need to add the new song information (artist, title, genre, and URL) to the music library arrays
in the first unoccupied position.
You will need to verify there is space to do so (i.e., librarySize < MAX_SONGS).
2.5 AddSongPlaylist ⋆⋆
This function is to add a song from the music library to our playlist. The function declaration should look
like this:
bool AddSongPlaylist(int numPlaylists, int playlists[][MAX_SONGS],
int songID, int playlistID);
Return: whether the song was successfully added or not.
This song will add a song to the end of the appropriate row of our playlists[][] array.
The songID is the index of the song in our library.
The playlistID is the index of the playlist we are adding the song to in our playlists[][] array.
You will need to find the first unoccupied space in this row of our array (i.e. the first place where the
value stored is -1) and add our song to that position in our playlist.
You will need to make sure there is space in that row to add the song. If there is not space, then you
should return false.
2.6 DeleteSongPlaylist ⋆⋆
