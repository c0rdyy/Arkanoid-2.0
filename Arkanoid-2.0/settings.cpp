#include "settings.h"
#include "InitDeInit.h"
#include <stdio.h>
#include <stdlib.h>

void LoadSettings(Settings* settings) 
{
    FILE* file = fopen("settings.txt", "r");
    if (file) 
    {
        fscanf(file, "%d", &settings->soundEnabled);
        fclose(file);
    }
    else 
    {
        settings->soundEnabled = 1;
    }
}

void SaveSettings(const Settings* settings) 
{
    FILE* file = fopen("settings.txt", "w");
    if (file) 
    {
        fprintf(file, "%d", settings->soundEnabled);
        fclose(file);
    }
}

int getCurrentSoundSetting() 
{
    return globalSettings.soundEnabled;  // ���������� ������� ��������� �����
}

void ToggleSound() 
{
    globalSettings.soundEnabled = !globalSettings.soundEnabled;  // ������������ ��������� �����
    SaveSettings(&globalSettings);  // ���������� ������� ��������

    // ���������� ��������� ��������� �����
    if (globalSettings.soundEnabled) {
        Mix_ResumeMusic();
    }
    else {
        Mix_PauseMusic();
    }
}