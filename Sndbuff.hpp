#ifndef SNDBUFF_HPP_INCLUDED
#define SNDBUFF_HPP_INCLUDED
#define DIR_SND "snd/"

#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
using namespace std;

const string names[] =  {   "walking.wav",
                            "walking_inside.wav",
                            "birds.wav",
                            "hited.wav",
                            "hited_body.wav",
                            "throw.wav",
                            "died.wav",
                            "ftake.wav"
                        };
const string mnames[] = {   "music1.ogg"
                        };
class Sndbuff
{
    public:
        vector<sf::SoundBuffer*> *sndbuffs;
        vector<sf::Sound*> *snds;
        sf::Music *music;
        enum SndName {SND_WALKING, SND_WALKING_INSIDE, SND_BIRDS, SND_HITED, SND_HITED_BODY, SND_THROW, SND_DIED, SND_FTAKE};      // kolejnosc musi byc zgodna z names[]
        enum MusicName {MUSIC_MAIN};                // muzyczki - kolejnosc zgodna z mnames[]
        Sndbuff();
        ~Sndbuff();
        sf::Sound* Get(enum SndName sn);
        sf::Music* Music(enum MusicName sn);
};

#endif // SNDBUFF_HPP_INCLUDED

