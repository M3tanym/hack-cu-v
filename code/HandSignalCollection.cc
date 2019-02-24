#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#include "main.h"
#include "Leap.h"
#include "HandSignal.h"
#include "HandSignalCollection.h"

using namespace std;

HandSignalCollection::HandSignalCollection() {
    ifstream f("trained_signals.gs");
    if (f) {
        string magic;
        getline(f, magic);
        if (magic.compare("#GS") == 0) {
            int num;
            f >> num;
            for (int i = 0; i < num; i++) {
                string line;
                getline(f, line);// remove extra \n char
                getline(f, line);// read in signal name
                names.push_back(line);
                getline(f, line);// read in signal command
                commands.push_back(line);
                
                f >> fingers;
                
                for (int j = 0; j < hs.fingers; j++) {
                    int x;
                    f >> x;
                    hs.fingerTypes[j] = static_cast<Finger::Type>(x);
                }
                
                for (int j = 0; j < hs.fingers; j++) {
                    f >> hs.fingerLengths[j];
                }
                
                for (int j = 0; j < hs.fingers; j++) {
                    f >> hs.fingerExtended[j];
                }
                
                for (int j = 0; j < hs.fingers; j++) {
                    for (int b = 0; b < 4; b++) {
                        for (int w = 0; w < 3; w++) {
                            f >> hs.boneStarts[j][b][w];
                        }
                    }
                }
                
                for (int j = 0; j < hs.fingers; j++) {
                    for (int b = 0; b < 4; b++) {
                        for (int w = 0; w < 3; w++) {
                            f >> hs.boneEnds[j][b][w];
                        }
                    }
                }
                
                for (int j = 0; j < hs.fingers; j++) {
                    for (int b = 0; b < 4; b++) {
                        for (int w = 0; w < 3; w++) {
                            f >> hs.boneDirs[j][b][w];
                        }
                    }
                }
            }
            if (!f) {
                if (DEBUG) cout << "[HandSignalCollection] error: trained_signals.gs format invalid! Signal data may be corrupted!" << endl;
                return;
            }
        }
    }
    if (DEBUG) cout << "[HandSignalCollection] Reading file trained_signals.gs failed!" << endl;
}

bool HandSignalCollection::save() const {
    ofstream savef("trained_signals.gs");
    
    if (savef) {
        savef << "#GS\n" << signals.size() << '\n';
        for (int i = 0; i < signals.size(); i++) {
            savef << names[i] << '\n' << commands[i] << '\n';
            HandSignal &hs = signals[i];
            savef << hs.fingers << '\n';
            savef << hs.settings.fingerLengthDiff << ' ' << hs.settings.positionDiff << ' ' << hs.settings.directionDiff << '\n';
            
            for (int j = 0; j < hs.fingers; j++) {
                savef << static_cast<int>(hs.fingerTypes[j]) << ' ';
            }
            savef << '\n'
            
            for (int j = 0; j < hs.fingers; j++) {
                savef << hs.fingerLengths[j] << ' ';
            }
            savef << '\n'
            
            for (int j = 0; j < hs.fingers; j++) {
                savef << hs.fingerExtended[j] << ' ';
            }
            savef << '\n'
            
            for (int j = 0; j < hs.fingers; j++) {
                for (int b = 0; b < 4; b++) {
                    for (int w = 0; w < 3; w++) {
                        savef << hs.boneStarts[j][b][w] << ' ';
                    }
                }
            }
            savef << '\n';
            
            for (int j = 0; j < hs.fingers; j++) {
                for (int b = 0; b < 4; b++) {
                    for (int w = 0; w < 3; w++) {
                        savef << hs.boneEnds[j][b][w] << ' ';
                    }
                }
            }
            savef << '\n';
            
            for (int j = 0; j < hs.fingers; j++) {
                for (int b = 0; b < 4; b++) {
                    for (int w = 0; w < 3; w++) {
                        savef << hs.boneDirs[j][b][w] << ' ';
                    }
                }
            }
            savef << '\n';
        }
        savef.close();
        return true;
    }
    return false;
}

void HandSignalCollection::exec(int i) const {
    // TODO exec command
}

bool add(HandSignal::HandSignal &hs, std::string &name, std::string &command) {
    signals.push_back(hs);
    names.push_back(name);
    commands.push_back(command);
    return save();
}

bool remove(int i) {
    if (i < signals.size()) {
        signals.erase(signals.start() + i);
        names.erase(names.start() + i);
        commands.erase(commands.start() + i);
        bool result = save();
        if (!save) {
            if (DEBUG) cout << "[HandSignalCollection] .remove() couldn't save the file" << endl;
            return false;
        }
        return true;
    }
    return false;
}

bool remove(std::string &name) {
    for (int i = 0; i < names.length; i++) {
        if (names[i].compare(name) == 0) {
            return remove(i);
        }
    }
    return false;
}