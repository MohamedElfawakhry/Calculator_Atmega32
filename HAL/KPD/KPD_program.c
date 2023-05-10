#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include <util/delay.h>
#include "../../MCAL/DIO/DIO_Interface.h"

#include "KPD_interface.h"
#include "KPD_private.h"
#include "KPD_config.h"

u8 KPD_Au8ColsArr[KPD_COLS_NUM]= {KPD_C0_PIN , KPD_C1_PIN , KPD_C2_PIN , KPD_C3_PIN  };
u8 KPD_Au8RowsArr[KPD_ROWS_NUM]= {KPD_R0_PIN , KPD_R1_PIN , KPD_R2_PIN , KPD_R3_PIN  };

u8 KPD_Au8KeysArr[KPD_ROWS_NUM][KPD_COLS_NUM]= KPD_KEYS ;
void KPD_voidInit(void)
{
    /*Configure the direction of Col pins as output */
    DIO_voidSetPinDirection(KPD_PORT , KPD_C0_PIN , OUTPUT );
    DIO_voidSetPinDirection(KPD_PORT , KPD_C1_PIN , OUTPUT );
    DIO_voidSetPinDirection(KPD_PORT , KPD_C2_PIN , OUTPUT );
    DIO_voidSetPinDirection(KPD_PORT , KPD_C3_PIN , OUTPUT );

    /*Configure the direction of Row pins as output */
    DIO_voidSetPinDirection(KPD_PORT , KPD_R0_PIN , INPUT );
    DIO_voidSetPinDirection(KPD_PORT , KPD_R1_PIN , INPUT );
    DIO_voidSetPinDirection(KPD_PORT , KPD_R2_PIN , INPUT );
    DIO_voidSetPinDirection(KPD_PORT , KPD_R3_PIN , INPUT );

    /*initialize the output pins as high --> Deactivate*/
    /*initialize the input pins as pull up*/
    DIO_voidSetPortValue( KPD_PORT , 255 );

}

u8 KPD_u8GetPressedKey(void)
{
    u8 Local_u8ColsIterator ;
    u8 Local_u8RowsIterator ;
    u8 Local_u8PinVal       ;
    u8 Local_u8PressedKey  = 0xff ;
    u8 Local_u8Flag = 0     ;

    for(Local_u8ColsIterator = 0 ; Local_u8ColsIterator < KPD_COLS_NUM ; Local_u8ColsIterator++ )
    {
        //activate the current column 
        DIO_voidSetPinValue( KPD_PORT , KPD_Au8ColsArr[ Local_u8ColsIterator ], LOW  );
        for(Local_u8RowsIterator = 0 ; Local_u8RowsIterator < KPD_ROWS_NUM ; Local_u8RowsIterator++)
        {   
            /*Debouncing */
            _delay_ms(2);
            /*Read Row Pin */
            Local_u8PinVal = DIO_u8GetPinValue(KPD_PORT ,KPD_Au8RowsArr[Local_u8RowsIterator] );

            /*If pressed && Debouncing for push button */
            if(Local_u8PinVal == 0 )
            {
                Local_u8PinVal = DIO_u8GetPinValue(KPD_PORT ,KPD_Au8RowsArr[Local_u8RowsIterator] );
                while( Local_u8PinVal == 0)
                {
                    /*Get Key value */
                    Local_u8PinVal = DIO_u8GetPinValue(KPD_PORT ,KPD_Au8RowsArr[Local_u8RowsIterator] );
                }
                Local_u8PressedKey = KPD_Au8KeysArr[Local_u8RowsIterator][Local_u8ColsIterator];
                Local_u8Flag = 1; 
                break;
            }
            
        }
        //Deactivate to the current column
        DIO_voidSetPinValue( KPD_PORT , KPD_Au8ColsArr[ Local_u8ColsIterator ], HIGH  );
        if(Local_u8Flag == 1 )
        {
            break ; 
        }
    }
    return Local_u8PressedKey ; 
}
