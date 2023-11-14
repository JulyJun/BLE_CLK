/*
 * songs.h
 *
 *  Created on: Nov 13, 2023
 *      Author: 박병준
 *
 *      기준: CLCK = 84Mhz ARR = 2000 Prescaler: 84
 */

#ifndef INC_SONGS_H_
#define INC_SONGS_H_

#include "tim.h"

#define REST 0
#define C4 3822
#define D4 3405
#define E4 3033
#define F4 2863
#define G4 2551
#define A4 2272
#define B4 2024
#define C5 1911
#define D5 1702
#define E5 1516
#define F5 1431
#define G5 1275
#define A5 1136
#define B5 1012
#define C6 955
#define D6 851
#define E6 758
#define F6 715
#define G6 637
#define A6 568
#define B6 506
#define C7 477



void threeBear_song(int* index);
void springWater_song(int* index);
void remove_tone();

#endif /* INC_SONGS_H_ */