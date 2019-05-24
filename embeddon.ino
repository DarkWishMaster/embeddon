#define TCAADDR 0x70

#include <Wire.h>

#define SENS_CNT      5
#define TIMER_SAMPLE  TIM16


#include <MPU6050.h>

MPU6050 sens0;
MPU6050 sens1;
MPU6050 sens2;
MPU6050 sens3;
MPU6050 sens4;


void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}



/* Handler for stimer */
static stimer_t TimHandle;
byte sync_pattern[] = {0x55, 0xAA};
static void blink(stimer_t *htim) {
  UNUSED(htim);
  static uint8_t sens_cnt = 0;
  static byte    buff[6];

  if (sens_cnt == 0)
  {    
    // send sync pattern
    Serial.write(sync_pattern, 2);
  }
  // read sensors round robin
  switch(sens_cnt)
  {
    case 0:
      tcaselect(2);
      sens0.readAcceleration(buff);
      break;
    case 1:
      tcaselect(3);
      sens1.readAcceleration(buff);
      break;
    case 2:
      tcaselect(4);
      sens2.readAcceleration(buff);
      break;
    case 3:
      tcaselect(5);
      sens3.readAcceleration(buff);
      break; 
    case 4:
      tcaselect(6);
      sens4.readAcceleration(buff);
      break; 
    default:
      break;
  }

  Serial.write(buff, 6);
  
  sens_cnt = sens_cnt + 1;
  if (sens_cnt >= SENS_CNT)
  {
    sens_cnt = 0;
  }
}

void ConfigClock(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

}


void init_sensors(void)
{   
    tcaselect(2);
    sens0.init();
    delay(10);
    tcaselect(3);
    sens1.init();
    delay(10);
    tcaselect(4);
    sens2.init();
    delay(10);
    tcaselect(5);
    sens3.init();
    delay(10);
    tcaselect(6);
    sens4.init();
    
    delay(200);
}

void setup() {
  ConfigClock();

  Serial.begin(500000);
  Wire.begin();
  Wire.setClock(400000);
  pinMode(LED_BUILTIN, OUTPUT);

  init_sensors();
  
  /* Set TIMx instance. */
  TimHandle.timer = TIMER_SAMPLE;
  /* Timer set to 10ms */
  TimerHandleInit(&TimHandle, 1000 - 1, (uint16_t)(HAL_RCC_GetHCLKFreq() / 1000000) - 1);

  attachIntHandle(&TimHandle, blink);

}

void loop() {
//  tcaselect(2);
//  int16_t x, y, z;
//  sens0.readAccelerationXYZ(&x, &y, &z);
//  Serial.println(z);
//
//  delay(100);
}
