//IMPORTANT: ADD COMMENT HERE to state your name, the date, and any 
//important information to run your program.


#include<iostream>
#include<fstream>

using namespace std;

//global variables
const int MAX_SONGS = 1000;
const int MAX_PLAYLISTS = 20;


bool Split(const string &line, char delimiter, string &part1, string &part2, string &part3, string &part4) {
    size_t start = 0;
    size_t end = line.find(delimiter);

    //  first part
    if (end == string::npos) return false;  // Less than expected parts
    part1 = line.substr(start, end - start);

    //  second part
    start = end + 1;
    end = line.find(delimiter, start);
    if (end == string::npos) return false;  // Less than expected parts
    part2 = line.substr(start, end - start);

    //  third part
    start = end + 1;
    end = line.find(delimiter, start);
    if (end == string::npos) return false;  // Less than expected parts
    part3 = line.substr(start, end - start);

    // get fourth part
    start = end + 1;
    part4 = line.substr(start);

    return true;
}

int ReadLibrary(string inputFile, string artists[], string titles[], string genres[], string urls[]) {
    ifstream file(inputFile);

    //  if the file was successfully opened
    if (!file.is_open()) {
        cerr << "Error opening file: " << inputFile << endl;
        return 0;
    }

    string line;
    int count = 0;

    //  each line from the file, up to the maximum number of songs (MAX_SONGS)
    while (getline(file, line) && count < MAX_SONGS) {
        string artist, title, genre, url;

        // split the line into artist, title, genre, and url
        if (Split(line, '\t', artist, title, genre, url)) {
            // Store the split components into respective arrays
            artists[count] = artist;
            titles[count] = title;
            genres[count] = genre;
            urls[count] = url;
            count++;
        } else {
            cout << "Error reading line. Song skipped. " << endl;
        }
    }

    // Close the file after reading all data
    file.close();

    // Return the number of songs read
    return count;
}


int ReadPlaylists(string inputFile, int playlists[][MAX_SONGS], string playlistNames[]) {
    ifstream inFile(inputFile);

    if (!inFile) {
        cerr << "error: unable to open file " << inputFile << endl;
        return 0;
    }

    int numPlaylists = 0;
    string line;
    int currentPlaylist = 0;

    while (getline(inFile, line)) {
        if (line.empty()) continue; // skip empty lines

        // check if the line indicates a new playlist
        if (line.find("playlist") != string::npos) {
            playlistNames[currentPlaylist] = line;
            numPlaylists++;
        } else {
            // read song IDs for the current playlist
            playlists[currentPlaylist][0] = stoi(line);
            for (int i = 1; i < MAX_SONGS; i++) {
                if (!getline(inFile, line) || line.empty()) break;
                if (line.find("playlist") != string::npos) {
                    currentPlaylist++;
                    playlistNames[currentPlaylist] = line;
                    numPlaylists++;
                    break;
                } else {
                    playlists[currentPlaylist][i] = stoi(line);
                }
            }
        }

        if (currentPlaylist >= MAX_PLAYLISTS) break;
    }

    inFile.close();
    return numPlaylists;
}

// add new playlist name if space available
int NewPlaylist(int numPlaylists, string playlistNames[], string newName) {
    if (numPlaylists < MAX_PLAYLISTS) {
        playlistNames[numPlaylists] = newName;
        numPlaylists++;
    }

    return numPlaylists;
}


// add a new song to the library if space available
int AddSongLibrary(int librarySize, string newArtist, string newTitle, string newGenre, string newURL,
                   string artists[], string titles[], string genres[], string urls[]) {
    if (librarySize < MAX_SONGS) {
        artists[librarySize] = newArtist;
        titles[librarySize] = newTitle;
        genres[librarySize] = newGenre;
        urls[librarySize] = newURL;
        librarySize++;
    }

    return librarySize;
}


// add a song to a playlist if the playlist exists and has space
bool AddSongPlaylist(int numPlaylists, int playlists[][MAX_SONGS], int songID, int playlistID) {
    if (playlistID >= 0 && playlistID < numPlaylists) {
        for (int i = 0; i < MAX_SONGS; ++i) {
            if (playlists[playlistID][i] == -1) {
                playlists[playlistID][i] = songID;
                return true; // song added successfully
            }
        }
    }

    return false; // playlist not found or full
}


// print songs in a playlist if the playlist exists
void PrintPlaylist(int numPlaylists, int playlists[][MAX_SONGS], int playlistID, string artists[], string titles[]) {
    if (playlistID < 0 || playlistID >= numPlaylists) {
        cerr << "invalid playlistID" << endl;
        return;
    }

    for (int i = 0; i < MAX_SONGS; ++i) {
        int songID = playlists[playlistID][i];
        if (songID != -1) {
            cout << "\"" << titles[songID] << "\" by " << artists[songID] << endl;
        } else {
            break; // end of playlist reached
        }
    }
}


// delete a song from a playlist if the playlist exists
bool DeleteSongPlaylist(int numPlaylists, int playlists[][MAX_SONGS], int songID, int playlistID) {
    // check if playlistID is valid
    if (playlistID < 0 || playlistID >= numPlaylists) {
        cout << "invalid playlist ID." << endl;
        return false;
    }

    bool songFound = false;

    // find the songID in the playlist and remove it
    for (int j = 0; j < MAX_SONGS; j++) {
        if (playlists[playlistID][j] == songID) {
            // shift remaining songs to the left
            for (int k = j; k < MAX_SONGS - 1; k++) {
                playlists[playlistID][k] = playlists[playlistID][k + 1];
            }
            // mark the last position as empty
            playlists[playlistID][MAX_SONGS - 1] = -1;
            songFound = true;
            j--; // adjust index due to shift
        }
    }

    return songFound;
}

// delete a song from the library and all playlists it belongs to
int DeleteSongLibrary(int librarySize, string artists[], string titles[],
                      string genres[], string urls[], int songID, int playlists[][MAX_SONGS]) {
    // check if songID is valid
    if (songID < 0 || songID >= librarySize) {
        //cout << "invalid song ID." << endl;
        return librarySize; // return current library size if invalid songID
    }

    // shift remaining songs in the library arrays
    for (int i = songID; i < librarySize - 1; i++) {
        artists[i] = artists[i + 1];
        titles[i] = titles[i + 1];
        genres[i] = genres[i + 1];
        urls[i] = urls[i + 1];
    }

    // clear the last entry in the library arrays
    artists[librarySize - 1] = "";
    titles[librarySize - 1] = "";
    genres[librarySize - 1] = "";
    urls[librarySize - 1] = "";

    // delete the song from all playlists
    for (int i = 0; i < MAX_PLAYLISTS; i++) {
        DeleteSongPlaylist(MAX_PLAYLISTS, playlists, songID, i);
    }

    return librarySize - 1; // return updated library size
}

// swap two songs within a playlist if both songs exist in the playlist
bool SwapSongs(int numPlaylists, int playlists[][MAX_SONGS], int songID1, int songID2, int playlistID) {
    // check if playlistID is valid
    if (playlistID < 0 || playlistID >= numPlaylists) {
        return false; // invalid playlist ID
    }

    int index1 = -1;
    int index2 = -1;

    // find indices of songID1 and songID2 within the playlist
    for (int j = 0; j < MAX_SONGS; j++) {
        if (playlists[playlistID][j] == -1) break; // end of playlist
        if (playlists[playlistID][j] == songID1 && index1 == -1) {
            index1 = j;
        }
        if (playlists[playlistID][j] == songID2 && index2 == -1) {
            index2 = j;
        }
        if (index1 != -1 && index2 != -1) {
            break; // both indices found
        }
    }

    // if either songID1 or songID2 not found in the playlist
    if (index1 == -1 || index2 == -1) {
        return false;
    }

    // swap the songs in the playlist
    int temp = playlists[playlistID][index1];
    playlists[playlistID][index1] = playlists[playlistID][index2];
    playlists[playlistID][index2] = temp;

    return true; // songs swapped successfully
}

// move a song to a new location within a playlist if the song and playlist exist
bool MoveSong(int numPlaylists, int playlists[][MAX_SONGS], int songID, int playlistID, int newLocation) {
    // check if playlistID is valid
    if (playlistID < 0 || playlistID >= numPlaylists) {
        return false; // invalid playlist ID
    }

    int currentIdx = -1;
    int playlistLength = 0;

    // find current index of songID in the playlist
    for (int j = 0; j < MAX_SONGS; j++) {
        if (playlists[playlistID][j] == -1) {
            break; // end of playlist
        }
        if (playlists[playlistID][j] == songID && currentIdx == -1) {
            currentIdx = j;
        }
        playlistLength++;
    }

    // if songID not found in the playlist
    if (currentIdx == -1) {
        return false;
    }

    // if current and new locations are the same
    if (currentIdx == newLocation) {
        return false;
    }

    // determine actual new location within playlist bounds
    int actualNewLocation = (newLocation >= playlistLength) ? playlistLength - 1 : newLocation;

    // move the song within the playlist
    if (currentIdx < actualNewLocation) {
        int temp = playlists[playlistID][currentIdx];
        for (int j = currentIdx; j < actualNewLocation; j++) {
            playlists[playlistID][j] = playlists[playlistID][j + 1];
        }
        playlists[playlistID][actualNewLocation] = temp;
    } else {
        int temp = playlists[playlistID][currentIdx];
        for (int j = currentIdx; j > actualNewLocation; j--) {
            playlists[playlistID][j] = playlists[playlistID][j - 1];
        }
        playlists[playlistID][actualNewLocation] = temp;
    }

    return true; // song moved successfully
}

// print Spotify playlist URLs for a specified playlist if it exists
void PrintSpotify(int numPlaylists, int playlists[][MAX_SONGS], int playlistID, string urls[]) {
    // check if playlistID is valid
    if (playlistID >= 0 && playlistID < numPlaylists) {
        // iterate through the playlist and print song URLs
        for (int i = 0; i < MAX_SONGS; ++i) {
            int songID = playlists[playlistID][i];
            if (songID != -1) {
                std::cout << urls[songID] << std::endl;
            }
        }
    }
}

// save changes to library and playlist files
void SaveChanges(string libraryFile, string playlistFile, int librarySize,
                 int numPlaylists, string artists[], string titles[], string genres[],
                 string urls[], int playlists[][MAX_SONGS], string playlistNames[]) {

    // if library is empty, create empty file
    if (librarySize == 0) {
        ofstream libraryStream(libraryFile);
        if (libraryStream.is_open()) {
            libraryStream << endl; // save empty line
            libraryStream.close();
        } else {
            cout << "Error opening library file: " << libraryFile << endl;
            return;
        }

        // create empty playlist file with a placeholder line
        ofstream playlistStream(playlistFile);
        if (playlistStream.is_open()) {
            playlistStream << "Empty Playlist" << endl;
            playlistStream.close();
        } else {
            cout << "Error opening playlist file: " << playlistFile << endl;
            return;
        }

        return;
    }

    // save library data to file
    ofstream libraryStream(libraryFile);
    if (libraryStream.is_open()) {
        for (int i = 0; i < librarySize; ++i) {
            libraryStream << artists[i] << "\t" << titles[i] << "\t" << genres[i] << "\t" << urls[i];
            if (i != librarySize - 1) {
                libraryStream << endl;
            }
        }
        libraryStream.close();
    } else {
        cout << "Error opening library file: " << libraryFile << endl;
        return;
    }

    // save playlist data to file
    ofstream playlistStream(playlistFile);
    if (playlistStream.is_open()) {
        if (numPlaylists == 0) {
            playlistStream.close(); // if no playlists, close the file
            return;
        }

        for (int i = 0; i < numPlaylists; ++i) {
            playlistStream << playlistNames[i] << endl;
            for (int j = 0; j < MAX_SONGS; ++j) {
                if (playlists[i][j] != -1) {
                    playlistStream << playlists[i][j];
                    if (playlists[i][j + 1] != -1) {
                        playlistStream << endl;
                    }
                }
            }
            playlistStream << endl; // separate playlists with a blank line
        }

        playlistStream.close();
    } else {
        cout << "Error opening playlist file: " << playlistFile << endl;
        return;
    }


}

int FindSongID(string artists[], string titles[], string genres[], int librarySize) {
    string searchType;
    string searchString;
    int choice;
    const int MAX_MATCHES = 1000;
    int matches[MAX_MATCHES];
    int matchCount = 0;

    // Prompt user for search type (artist, title, or genre)
    do {
        cout << "Would you like to search by artist (A), title (T), or genre (G)? \n";
        cin >> searchType;
        // Convert to uppercase for case-insensitive comparison
        for (char &c : searchType) {
            c = toupper(c);
        }
    } while (searchType != "A" && searchType != "T" && searchType != "G");

    // Prompt user for search string
    cin.ignore(); // Clear input buffer
    if (searchType == "A") {
        cout << "What artist would you like to search for? \n";
    } else if (searchType == "T") {
        cout << "What title would you like to search for? \n";
    } else if (searchType == "G") {
        cout << "What genre would you like to search for? \n";
    }
    getline(cin, searchString);f // Read the search string

    // Search and store matches
    for (int i = 0; i < librarySize; ++i) {
        if ((searchType == "A" && artists[i].find(searchString) != string::npos) ||
            (searchType == "T" && titles[i].find(searchString) != string::npos) ||
            (searchType == "G" && genres[i].find(searchString) != string::npos)) {
            matches[matchCount++] = i;
        }
    }

    // If no matches found, return -1
    if (matchCount == 0) {
        cout << "No matches found." << endl;
        return -1;
    }

    // Display matches
    for (int i = 0; i < matchCount; ++i) {
        cout << i + 1 << ": \"" << titles[matches[i]] << "\" by " << artists[matches[i]] << endl;
    }

    // Prompt user to choose a song by number or enter -1 if none match
    do {
        cout << "Which number song is your choice? If none of the above, enter -1. \n";
        cin >> choice;
    } while (choice < -1 || choice > matchCount);

    // Return the library index value of the song they chose (0-based index)
    if (choice == -1) {
        return -1;
    } else {
        return matches[choice - 1];
    }
}
int main() {
    string artists[MAX_SONGS], titles[MAX_SONGS], genres[MAX_SONGS], urls[MAX_SONGS];
    int playlists[MAX_PLAYLISTS][MAX_SONGS] = {{-1}};
    string playlistNames[MAX_PLAYLISTS];
    int librarySize = 0, numPlaylists = 0;
    string libraryFile, playlistFile;
    char choice;
    string newLocationStr;
    int newLocation;

    string menu1 = "Select an option:\n1. Access Music Library\n2. Access Playlists\n3. Quit\n";
    string menu2 = "Select an option:\n1. Add a song to your library\n2. Remove a song from your library\n3. Search your library\n4. Go back\n";
    string menu3 = "Select an option:\n1. Open an existing playlist\n2. Open a new playlist\n3. Go back\n";
    string menu4 = "Select an option:\n1. Add a song to your playlist\n2. Remove a song from your playlist\n3. Swap the position of two songs in your playlist\n4. Move a song to a new position in your playlist\n5. Print your playlist\n6. Go back\n";
    string menu5 = "Select an option:\n1. Save and Quit\n2. Quit without saving\n3. Go back\n";

// Opening library
    cout << "Would you like to open the default library? Y/N" << endl;
    cin >> choice;
    if (choice == 'Y' || choice == 'y') {
        libraryFile = "musiclibrary.tsv";
        while ((librarySize = ReadLibrary(libraryFile, artists, titles, genres, urls)) == -1) {
            cout << "Cannot open that file. Try again:" << endl;
            cin >> libraryFile;
        }
    } else {
        cout << "What library file would you like to open?" << endl;
        cin >> libraryFile;
        while ((librarySize = ReadLibrary(libraryFile, artists, titles, genres, urls)) == 0) {
            cout << "Cannot open that file. Try again:" << endl;
            cin >> libraryFile;
        }
    }

    // Opening playlists
    cout << "Would you like to open the default playlists? Y/N" << endl;
    cin >> choice;
    if (choice == 'Y' || choice == 'y') {
        playlistFile = "musicplaylists.txt";
        while ((numPlaylists = ReadPlaylists(playlistFile, playlists, playlistNames)) == -1) {
            cout << "Cannot open that file. Try again:" << endl;
            cin >> playlistFile;
        }
    } else {
        cout << "What playlist file would you like to open?" << endl;
        cin >> playlistFile;
        while ((numPlaylists = ReadPlaylists(playlistFile, playlists, playlistNames)) == 0) {
            cout << "Cannot open that file. Try again:" << endl;
            cin >> playlistFile;
        }
    }



    cout << "Library and playlists loaded." << endl;

    int mainChoice, subChoice, subSubChoice;
    string artist, title, genre, url, newName;
    int songID, playlistID, songID1, songID2;

    while (true) {
        cout << menu1;
        cin >> mainChoice;

        switch (mainChoice) {
            case 1:
                while (true) {
                    cout << menu2;
                    cin >> subChoice;
                    if (subChoice == 4) break;

                    switch (subChoice) {
                        case 1:
                            cout << "What is the artist name?" << endl;
                            cin.ignore();
                            getline(cin, artist);
                            cout << "What is the song title?" << endl;
                            getline(cin, title);
                            cout << "What is the genre?" << endl;
                            getline(cin, genre);
                            cout << "What is the spotify URL?" << endl;
                            getline(cin, url);
                            librarySize = AddSongLibrary(librarySize, artist, title, genre, url, artists, titles, genres, urls);
                            cout << "Song added." << endl;
                            break;
                        case 2:
                            songID = FindSongID(artists, titles, genres, librarySize);
                            if (songID != -1) {
                                librarySize = DeleteSongLibrary(librarySize, artists, titles, genres, urls, songID, playlists);
                                cout << "Song removed." << endl;
                            } else {
                                cout << "Song not found." << endl;
                            }
                            break;
                        case 3:
                            songID = FindSongID(artists, titles, genres, librarySize);
                            if (songID != -1) {
                                cout << "Found: \"" << titles[songID] << "\" by " << artists[songID] << endl;
                            } else {
                                cout << "Song not found." << endl;
                            }
                            break;
                        default:
                            cout << "Invalid selection. Please try again." << endl;
                    }
                }
                break;
            case 2:
                while (true) {
                    cout << menu3;
                    cin >> subChoice;
                    if (subChoice == 3) break;

                    switch (subChoice) {
                        case 1:
                            cout << "Which playlist would you like to open?" << endl;
                            cin >> playlistID;
                            for (int i = 0; i < numPlaylists; ++i) {
                                cout << i + 1 << ": " << playlistNames[i] << endl;
                            }

                            --playlistID;
                            while (true) {
                                cout << menu4;
                                cin >> subSubChoice;
                                if (subSubChoice == 6) break;

                                switch (subSubChoice) {
                                    case 1:
                                        songID = FindSongID(artists, titles, genres, librarySize);
                                        if (songID != -1 && AddSongPlaylist(numPlaylists, playlists, songID, playlistID)) {
                                            cout << "Song added to playlist." << endl;
                                        } else {
                                            cout << "Failed to add song to playlist." << endl;
                                        }
                                        break;
                                    case 2:
                                        songID = FindSongID(artists, titles, genres, librarySize);
                                        if (songID != -1 && DeleteSongPlaylist(numPlaylists, playlists, songID, playlistID)) {
                                            cout << "Song removed from playlist." << endl;
                                        } else {
                                            cout << "Failed to remove song from playlist." << endl;
                                        }
                                        break;
                                    case 3:
                                        cout << "Enter the ID of the first song to swap:" << endl;
                                        songID1 = FindSongID(artists, titles, genres, librarySize);
                                        cout << "Enter the ID of the second song to swap:" << endl;
                                        songID2 = FindSongID(artists, titles, genres, librarySize);
                                        if (songID1 != -1 && songID2 != -1 && SwapSongs(numPlaylists, playlists, songID1, songID2, playlistID)) {
                                            cout << "Songs swapped." << endl;
                                        } else {
                                            cout << "Failed to swap songs." << endl;
                                        }
                                        break;
                                    case 4:
                                        songID = FindSongID(artists, titles, genres, librarySize);
                                        cout << "Enter the new location (or 'end' to move to the end):" << endl;
                                        cin >> newLocationStr;
                                        if (newLocationStr == "end") {
                                            newLocation = MAX_SONGS;
                                        } else {
                                            newLocation = stoi(newLocationStr);
                                        }
                                        if (songID != -1 && MoveSong(numPlaylists, playlists, songID, playlistID, newLocation)) {
                                            cout << "Song moved." << endl;
                                        } else {
                                            cout << "Failed to move song." << endl;
                                        }
                                        break;
                                    case 5:
                                        cout << "Select an option:\n1. Print by artists/titles\n2. Print Spotify Playlist\n3. Go back" << endl;
                                        cin >> subChoice;
                                        if (subChoice == 1) {
                                            PrintPlaylist(numPlaylists, playlists, playlistID, artists, titles);
                                        } else if (subChoice == 2) {
                                            PrintSpotify(numPlaylists, playlists, playlistID, urls);
                                        }
                                        break;
                                    default:
                                        cout << "Invalid selection." << endl;
                                }
                            }
                            break;
                        case 2:
                            cout << "What would you like to name the playlist?" << endl;
                            cin.ignore();
                            getline(cin, newName);
                            numPlaylists = NewPlaylist(numPlaylists, playlistNames, newName);
                            // Directly move to manage the newly created playlist
                            playlistID = numPlaylists - 1;
                            while (true) {
                                cout << menu4;
                                cin >> subSubChoice;
                                if (subSubChoice == 6) break;

                                switch (subSubChoice) {
                                    case 1:
                                        songID = FindSongID(artists, titles, genres, librarySize);
                                        if (songID != -1 && AddSongPlaylist(numPlaylists, playlists, songID, playlistID)) {
                                            cout << "Song added to playlist." << endl;
                                        } else {
                                            cout << "Failed to add song to playlist." << endl;
                                        }
                                        break;
                                    case 2:
                                        songID = FindSongID(artists, titles, genres, librarySize);
                                        if (songID != -1 && DeleteSongPlaylist(numPlaylists, playlists, songID, playlistID)) {
                                            cout << "Song removed from playlist." << endl;
                                        } else {
                                            cout << "Failed to remove song from playlist." << endl;
                                        }
                                        break;
                                    case 3:
                                        cout << "Enter the ID of the first song to swap:" << endl;
                                        songID1 = FindSongID(artists, titles, genres, librarySize);
                                        cout << "Enter the ID of the second song to swap:" << endl;
                                        songID2 = FindSongID(artists, titles, genres, librarySize);
                                        if (songID1 != -1 && songID2 != -1 && SwapSongs(numPlaylists, playlists, songID1, songID2, playlistID)) {
                                            cout << "Songs swapped." << endl;
                                        } else {
                                            cout << "Failed to swap songs." << endl;
                                        }
                                        break;
                                    case 4:
                                        songID = FindSongID(artists, titles, genres, librarySize);
                                        cout << "Enter the new location (or 'end' to move to the end):" << endl;
                                        cin >> newLocationStr;
                                        if (newLocationStr == "end") {
                                            newLocation = MAX_SONGS;
                                        } else {
                                            newLocation = stoi(newLocationStr);
                                        }
                                        if (songID != -1 && MoveSong(numPlaylists, playlists, songID, playlistID, newLocation)) {
                                            cout << "Song moved." << endl;
                                        } else {
                                            cout << "Failed to move song." << endl;
                                        }
                                        break;
                                    case 5:
                                        cout << "Select an option:\n1. Print by artists/titles\n2. Print Spotify Playlist\n3. Go back" << endl;
                                        cin >> subChoice;
                                        if (subChoice == 1) {
                                            PrintPlaylist(numPlaylists, playlists, playlistID, artists, titles);
                                        } else if (subChoice == 2) {
                                            PrintSpotify(numPlaylists, playlists, playlistID, urls);
                                        }
                                        break;
                                    default:
                                        cout << "Invalid selection." << endl;
                                }
                            }
                            break;
                        default:
                            cout << "Invalid selection." << endl;
                    }
                }
                break;
            case 3:
                while (true) {
                    cout << menu5;
                    cin >> subChoice;
                    if (subChoice == 3) break;

                    switch (subChoice) {
                        case 1:
                            cout << "Would you like to save the library to the current location? Y/N" << endl;
                            cin >> choice;
                            if (choice == 'N' || choice == 'n') {
                                cout << "Enter the new library file location:" << endl;
                                cin >> libraryFile;
                            }
                            cout << "Would you like to save the playlists to the current location? Y/N" << endl;
                            cin >> choice;
                            if (choice == 'N' || choice == 'n') {
                                cout << "Enter the new playlists file location:" << endl;
                                cin >> playlistFile;
                            }
                            SaveChanges(libraryFile, playlistFile, librarySize, numPlaylists, artists, titles, genres, urls, playlists, playlistNames);
                            return 0;
                        case 2:
                            return 0;
                        default:
                            cout << "Invalid selection." << endl;
                    }
                }
                break;
            default:
                cout << "Invalid selection." << endl;
        }
    }
    return 0;
}