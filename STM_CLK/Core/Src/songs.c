/*
 * songs.c
 *
 *  Created on: Nov 13, 2023
 *      Author: 박병준
 */
#include "songs.h"

const unsigned int threeBears_note[] = {C5,C5,C5,C5,C5,E5,G5,G5,E5,C5,G5,G5,E5,G5,G5,E5,C5,C5,C5,REST,
		G5,G5,E5,C5,G5,G5,G5,G5,G5,E5,C5,G5,G4,G5,REST,
		G5,G5,E5,C5,G5,G5,G5,A5,A5,C5,G5,C5,G4,E5,D5,C5,
		REST
};
const unsigned int threeBears_intvl[] = {40,20,20,40,40,40,20,20,40,40,40,40,40,40,40,40,40,40,20,20,
		40,40,40,40,40,40,20,40,40,40,40,40,40,20,20,
		40,40,40,40,20,20,20,20,40,50,40,50,40,40,40,40,
		0
};

const unsigned int springWater_note[] = {E4, G4, C5, E4, G4, F4, A4, A4,
		G4, B4, D5, F5, E5,D5, C5,

		E4, G4, C5, E4, G4, F4, A4, A4,
		G4, B4, D5, F5, E5, D5, C5,

		C5, E5,G5,F5,E5,D5,
		G4,B4,D5,F5,E5,D5,C5,

		E4,G4,C5,E4,G4,F4,A4,A4,
		G4,B4,D5,F5,E5,D5,C5,
		REST
};

const unsigned int springWater_intvl[] = {
		20,20,20,20,40,40,40,40,
		20,20,20,20,20,20,80,

		20,20,20,20,20,40,40,40,40,
		20,20,20,20,20,20,80,

		40,40,40,60,20,40,
		40,20,20,40,60,20,40,

		20,20,20,20,40,40,40,40,
		20,20,20,20,20,20,80,
		0
};

void threeBear_song()
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	for(int i = 0; i < sizeof(threeBears_note)/sizeof(threeBears_note[0]); i++)
	{
//		TIM3->ARR = note[i];
//		TIM3->CCR3 = note[i]/2;
//		HAL_Delay(intr[i]);	// 소리 길이
//		TIM3->CCR3 = 0;
//		HAL_Delay(500);	// 쉬는 길이
		TIM3->ARR = threeBears_note[i];
		TIM3->CCR3 = threeBears_note[i]/2;
		HAL_Delay(500);
		TIM3->CCR3 = 0;
		HAL_Delay(threeBears_intvl[i]);
		TIM3->CCR3 = threeBears_note[i]/2;
	}
}

void springWater_song()
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	for(int i = 0; i < sizeof(springWater_note)/sizeof(springWater_note[0]); i++)
	{
//		TIM3->ARR = note[i];
//		TIM3->CCR3 = note[i]/2;
//		HAL_Delay(intr[i]);	// 소리 길이
//		TIM3->CCR3 = 0;
//		HAL_Delay(500);	// 쉬는 길이
		TIM3->ARR = springWater_note[i];
		TIM3->CCR3 = springWater_note[i]/2;
		HAL_Delay(500);
		TIM3->CCR3 = 0;
		HAL_Delay(springWater_intvl[i]);
		TIM3->CCR3 = springWater_note[i]/2;
	}
}

