if (!bcm2835_init())
{
    return VO_RET_FAIL;
}

vo_uint32 vf_ReadData_AM2305(vo_uint32 udwGPIO_PORT, vo_uint32 *pudwData)
{
    vo_uint32 udwI     = 0;
    vo_uint32 udwJ     = 0;
    vo_uint32 udwCnt   = 0;
    vo_uint32 udwState = HIGH;

    vo_uint16 uwTemperature = 0;
    vo_uint16 uwHumidity    = 0;

    /*
    if (!bcm2835_init())
    {
        return VO_RET_OK;
    }
    */
 
    // Set GPIO pin to output
    bcm2835_gpio_fsel(udwGPIO_PORT, BCM2835_GPIO_FSEL_OUTP);
 
    bcm2835_gpio_write(udwGPIO_PORT, HIGH);
    bcm2835_delay(500); // 500ms
    bcm2835_gpio_write(udwGPIO_PORT, LOW);
    bcm2835_delayMicroseconds(2000);  // 2ms
 
    // Set GPIO pin to input
    bcm2835_gpio_fsel(udwGPIO_PORT, BCM2835_GPIO_FSEL_INPT);
 
    // wait for pin to drop?
    while (bcm2835_gpio_lev(udwGPIO_PORT) == 1)
    {
        bcm2835_delayMicroseconds(1);
    }
 
    // reading data
    for (udwI = 0; udwI < 100; udwI++)
    {
        udwCnt = 0;
        while (bcm2835_gpio_lev(udwGPIO_PORT) == udwState)
        {
            udwCnt++;
            if(udwCnt == 1000)
                break;
        }
        if (udwCnt == 1000) break;
        udwState = bcm2835_gpio_lev(udwGPIO_PORT);
 
        // shove each bit into the storage bytes
        if ((udwI > 3) && (udwI % 2 == 0))
        {
            pudwData[udwJ / 8] <<= 1;
            if (udwCnt > 200)
                pudwData[udwJ / 8] |= 1;
            udwJ++;
        }
    }

    #ifdef _EV_DEBUG_
    if (pudwData[4] == ((pudwData[0] + pudwData[1] + pudwData[2] + pudwData[3]) & 0xFF))
    {
        uwTemperature   = pudwData[2];
        uwTemperature <<= 8          ;
        uwTemperature  |= pudwData[3];

        uwHumidity      = pudwData[0];
        uwHumidity    <<= 8          ;

        printf("Temperature is %d, Humidity is %d\n", uwTemperature, uwHumidity);

        return VO_RET_OK;
    }
    else
    {
        printf("check failed!\n");
        return -1;
    }
    #endif

    /*
    bcm2835_close();
    */

    return VO_RET_OK;
}

bcm2835_close();
