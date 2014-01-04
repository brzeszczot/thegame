#include "Sndbuff.hpp"
using namespace std;

Sndbuff::Sndbuff()
{
    music = new sf::Music;
    sndbuffs = new vector<sf::SoundBuffer*>;
    size_t length = (size_t)sizeof(names)/sizeof(string);
    snds = new vector<sf::Sound*>;

    for(size_t ii=0;ii<length;ii++)
    {
        sf::SoundBuffer *sndb = new sf::SoundBuffer;
        string temp = DIR_SND+names[ii];
        sndb->LoadFromFile(temp.c_str());
        sndbuffs->push_back(sndb);

        sf::Sound *snd = new sf::Sound;
        snd->SetBuffer(*sndb);
        snds->push_back(snd);
    }
    //Sound.SetPitch(1.5f);
    //Sound.SetVolume(75.f);
    //Sound.Pause();
    //Sound.Play(); // To resume after a call to Pause()
    //Sound.Stop();
}

sf::Sound* Sndbuff::Get(enum SndName sn)
{
    return (*snds)[(int)sn];
}

sf::Music* Sndbuff::Music(enum MusicName sn)
{
    music->OpenFromFile(DIR_SND+mnames[(int)sn]);
    music->SetVolume(10.f);
    music->Play();
    return music;

//    Music.GetDuration()   << " sec"
//    Music.GetSampleRate()    << " samples / sec"
//    Music.GetChannelsCount() << " channels"
//    Music.GetPlayingOffset() << "sec"
}

Sndbuff::~Sndbuff()
{
    for(size_t ii=0;ii<(size_t)sndbuffs->size();ii++)
        delete (*sndbuffs)[ii];
    delete sndbuffs;
}
