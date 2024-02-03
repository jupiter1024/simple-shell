#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include <dirent.h>

using namespace std;
string coppiedFilePath;
string realPath;
vector<string> currentDirectory;
string m;
string coppiedFileName;

void list() {
    if (currentDirectory.empty()) {
        cout << "no directory specified" << endl;

    } else {

        DIR *dir = opendir(realPath.c_str());
        dirent *entry;
        while ((entry = readdir(dir)) != nullptr) {
            cout << entry->d_name << endl;
        }
        closedir(dir);
    }
}


void GetCommand(string *input, string *command, string *path) {
    int commandFirstExist = input->find(" ");
    int commandSlashExist = input->find("/");
    if ((commandFirstExist < commandSlashExist || commandSlashExist == -1) && commandFirstExist != -1) {
        *command = input->substr(0, commandFirstExist);
        *path = input->substr(commandFirstExist);

    } else if ((commandSlashExist < commandFirstExist || commandFirstExist == -1) && commandSlashExist != -1) {
        *command = input->substr(0, commandSlashExist);
        *path = input->substr(commandSlashExist);

    } else {
        *command = *input;
        return;
    }
}

void cd(const char *path) {
    realPath = "c:";
    realPath += path;
    string c = "cd ";
    c += path;
    if (access(realPath.c_str(), F_OK) == 0) {
        int d = system(c.c_str());
        if (d == 0) {
            currentDirectory.push_back(m);
        } else {
            cout << "Command execution failed or returned non-zero: " << endl;
        }
        return;
    } else if (!currentDirectory.empty()) {
        string command;
        string lastPath;
        GetCommand(&currentDirectory.back(), &command, &lastPath);
        realPath = "c:" + lastPath + path;
        if (access(realPath.c_str(), F_OK) == 0) {
            c = currentDirectory.back() + path;
            int d = system(c.c_str());
            if (d == 0) {
                currentDirectory.push_back(c);
            }
        } else {
            cout << "invalid path";

        }
        return;
    }

}

void copy(string *path) {
    if (currentDirectory.empty()) {
        cout << "no directory specified" << endl;

    } else {

        DIR *dir = opendir(realPath.c_str());
        dirent *entry;
        bool findFile;
        coppiedFileName = path->substr(1);
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_name == coppiedFileName) {
                findFile = true;
                break;
            } else {
                findFile = false;
            }
        }
        if (findFile == 1) {
            coppiedFilePath = currentDirectory.back();
        } else {
            cout << "file not found";

        }

        closedir(dir);
    }
}

string ConvertPath() {
    string convertedPath;

    for (char c: currentDirectory.back()) {
        if (c == '/') {
            convertedPath += "\\\\";
        } else {
            convertedPath += c;
        }
    }
    size_t found = convertedPath.find("cd");
    if (found != std::string::npos) {
        convertedPath.replace(found, 2, "C:");
    }
    return convertedPath;
}

void paste() {
    string ps = ConvertPath();
    string systemCommand = coppiedFilePath + " && copy " + "\"" + coppiedFileName + "\"" + " " + ps;
    system(systemCommand.c_str());
}

void help() {
    cout << "cd change directory " << endl;
    cout << "copy is used to copy files copy [file name]  " << endl;
    cout << "paste used to paste what you have coppied" << endl;
    cout << "list to list all files in the directory " << endl;
    cout << "ipconfig" << endl;
    cout << "exist " << endl;
}

int main() {
    string command;
    string path;
    vector<string> input;
    while (true) {
        getline(cin, m);
        input.push_back(m);
        GetCommand(&input.back(), &command, &path);
        if (command == "cd") {

            cd(*reinterpret_cast<const char **>(&path));
        } else if (command == "exists") {
            return 0;

        } else if (command == "ipconfig") {
            system(command.c_str());
        } else if (command == "copy") {
            copy(&path);
        } else if (command == "paste") {
            paste();
        } else if (command == "help") {
            help();
        }
        if (currentDirectory.empty()) {
            continue;
        } else {
            cout << currentDirectory.back() + "$";
        }
    }
}

