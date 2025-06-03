#include"main.h"

#define INTEGRAL_SATURATION_VALUE (PEDAL_MAX)

uint32_t PID_setpoint;
float target_speed;
float cur_speed;
float prev_speed;
float PID_output;

uint32_t buf[4] = {0};
uint8_t head = 0;
uint8_t tail = 3;
int32_t total = 0L;

uint32_t average(int32_t PID_Output)
{
   buf[tail] = PID_Output;
   total += PID_Output;
   if (++tail >= 4 ) tail = 0;
   total -= buf[head];
   if (++head >= 4 ) head = 0;
   return total >> 2; //impartit la 4;
}

void update_PID(void)
{
	float err = 0;
    float integral_sum = 0;
    float derivative_speed = 0;

    err = target_speed - cur_speed;

	integral_sum += err;
    if (integral_sum > INTEGRAL_SATURATION_VALUE ) integral_sum = INTEGRAL_SATURATION_VALUE ;

    derivative_speed = prev_speed - cur_speed;
    prev_speed = cur_speed;

    PID_output = PID_KP * err + PID_KI * integral_sum + PID_KD * derivative_speed;

    PID_setpoint = average(PID_output);

	if (PID_setpoint < PEDAL_MIN) PID_setpoint = PEDAL_MIN;
    if (PID_setpoint > PEDAL_MAX) PID_setpoint = PEDAL_MAX;
}
