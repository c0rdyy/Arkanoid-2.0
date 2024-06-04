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

#endif // SETTINGS_H
