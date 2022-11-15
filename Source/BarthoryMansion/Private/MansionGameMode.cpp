// Fropple Studios Copyright


#include "MansionGameMode.h"
#include "fmod_studio.hpp"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

int AMansionGameMode::GetTimeForEvent(UFMODEvent* EventIn)
{
    if (EventsAndTime.Contains(EventIn)) {
        return *EventsAndTime.Find(EventIn);
    }
    return 0;
}

void AMansionGameMode::SaveMusicTime(FFMODEventInstance EventMusic)
{
    int PosReturn;
    EventMusic.Instance->getTimelinePosition(&PosReturn);
}

void AMansionGameMode::ChangeMusic(UFMODEvent* MusicEvent)
{
    if (MusicEvent != CurrentEvent) {
        PreviousPlayingMusic = CurrentPlayingMusic;
        if (CurrentEvent) {
            if (EventsAndTime.Contains(CurrentEvent)) {
                EventsAndTime.Remove(CurrentEvent);
            }
            int PositionToSave;
            PreviousPlayingMusic.Instance->getTimelinePosition(&PositionToSave);
            EventsAndTime.Add(CurrentEvent, PositionToSave);
        }
        
        CurrentPlayingMusic = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MusicEvent, true);
        if (CurrentPlayingMusic.Instance) {
            CurrentPlayingMusic.Instance->setVolume(0);
            if (MusicEvent) { //I dont check before cause it may be an no music area
                if (EventsAndTime.Contains(MusicEvent)) {
                    int playTime = *EventsAndTime.Find(MusicEvent);
                    CurrentPlayingMusic.Instance->setTimelinePosition(playTime);
                }
            }
        }
        LowerPreviousMusic();

        CurrentEvent = MusicEvent;
        MusicChanged.Broadcast();
    }
}

void AMansionGameMode::StopCurrentMusic()
{
    CurrentPlayingMusic.Instance->stop(FMOD_STUDIO_STOP_MODE::FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

UFMODEvent* AMansionGameMode::GetCurrentMusic()
{
    return CurrentEvent;
}
