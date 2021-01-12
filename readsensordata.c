   vo_uint32 udwTemHumData[100];   // 声明数组，用来接收单总线方式传感器的温湿度数据
    vf_MemFill(udwTemHumData, 100 * sizeof(vo_uint32), VO_ZERO);    //对数组进行初始化

    vf_ReadData_AM2305(AM2305_GPIO, udwTemHumData);     // 使用vf_ReadData_AM2305 函数读出温湿度数据

    if (udwTemHumData[4] == ((udwTemHumData[0] + udwTemHumData[1] + udwTemHumData[2] + udwTemHumData[3]) & 0xFF))
    {
        uwTemperature   = udwTemHumData[2];
        uwTemperature <<= 8               ;
        uwTemperature  |= udwTemHumData[3];
        stConstEnvSensor.dbTemperature = uwTemperature / 10.00;

        uwHumidity   = udwTemHumData[0];
        uwHumidity <<= 8               ;
        stConstEnvSensor.dbHumidity    = uwHumidity / 10.00;
    }
    else
    {
        vf_Print("Tem_Hum Ret Check Failed!\n");

        return (vo_void *)(-1);
    }
    
    
