#include "settings.h"
#include "InitDeInit.h"
#include <stdio.h>
#include <stdlib.h>

// Сохранение рекордов
void SaveHighScores(int scores[], int size) 
{
    FILE* file = fopen("highscores.txt", "w");
    if (file) 
    {
        for (int i = 0; i < size; i++) 
        {
            fprintf(file, "%d\n", scores[i]);
        }
        fclose(file);
    }
}

// Загрузка рекордов
void LoadHighScores(int scores[], int size) 
{
    FILE* file = fopen("highscores.txt", "r");
    if (file) 
    {
        for (int i = 0; i < size; i++) 
        {
            fscanf(file, "%d", &scores[i]);
        }
        fclose(file);
    }
    else 
    {
        for (int i = 0; i < size; i++) 
        {
            scores[i] = 0;
        }
    }
}

// Загрузка настроек
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

// Сохранение настроек
void SaveSettings(const Settings* settings) 
{
    FILE* file = fopen("settings.txt", "w");
    if (file) 
    {
        fprintf(file, "%d", settings->soundEnabled);
        fclose(file);
    }
}

// Функция, которая возвращает текущее состояние звука
int getCurrentSoundSetting() 
{
    return globalSettings.soundEnabled;
}

// Переключение звука
void ToggleSound() 
{
    globalSettings.soundEnabled = !globalSettings.soundEnabled;  // Переключение состояния звука
    SaveSettings(&globalSettings);  // Сохранение текущих настроек

    // Применение изменения состояния звука
    if (globalSettings.soundEnabled) 
    {
        Mix_ResumeMusic();
    }
    else 
    {
        Mix_PauseMusic();
    }
}