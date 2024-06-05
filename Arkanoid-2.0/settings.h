#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H  

#define _CRT_SECURE_NO_WARNINGS

typedef struct 
{
    int soundEnabled;
} Settings;

void LoadSettings(Settings* settings);
void SaveSettings(const Settings* settings);
int getCurrentSoundSetting();
void ToggleSound();

void SaveHighScores(int scores[], int size);
void LoadHighScores(int scores[], int size);

#endif // SETTINGS_H
