#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include <util/delay.h>

#include "../MCAL/DIO/DIO_Interface.h"

#include "../HAL/CLCD/CLCD_interface.h"

#include "../HAL/KPD/KPD_interface.h"

u16 GetValueKeypad ( u8* Copy_pu8Opertor  );
u16 ResultOperationFunction ( u16 Copy_u16Op1 , u16 Copy_u16Op2 , u8 Copy_u8Operator );
u8 Glo_Iterator = 0 ;

int main()
{
	CLCD_voidInit();
	KPD_voidInit();



	u8  Loc_operator , loc ;
	u8 	Flag = 1 ;
	while(1)
	{
			if(Flag == 1)
			{
				Flag = 0 ;
			CLCD_voidSendCommend(0x01);
			u16 Local_u16Op1 = GetValueKeypad( &Loc_operator);

			u16 Local_u16Op2 = GetValueKeypad( &loc );
			Glo_Iterator = 0 ;

			u16 Result = ResultOperationFunction(Local_u16Op1 ,Local_u16Op2 , Loc_operator );
			CLCD_voidSendNumber(Result);
			}
			else
			{
				if(KPD_u8GetPressedKey() =='c')
				{
					Flag = 1;
				}

			}



	}
}
u16 GetValueKeypad (u8* Copy_pu8Opertor )
{
	u16 Local_u16Value = 0 ;
	u8 Local_u8PressedKey;
	u8 Local_Iterator = 0 ;
	u8 Local_AValues[5]= {0};
	/* Get Value from Keypad */
	while(1)
	{
		Local_u8PressedKey = KPD_u8GetPressedKey();
		if(Local_u8PressedKey != 0xff)
		{
			/*To clear number using C button*/
			if(Local_u8PressedKey == 'c')
			{
				Local_Iterator-- ;
				CLCD_voidSetPosition(CLCD_ROW_1 , Glo_Iterator);
				CLCD_voidSendData(' ');
				CLCD_voidSetPosition(CLCD_ROW_1 , Glo_Iterator);
				Glo_Iterator--;

			}
			else
			{
				CLCD_voidSendData(Local_u8PressedKey);
				Local_AValues[Local_Iterator] = Local_u8PressedKey ;
				Local_Iterator++ ;
				Glo_Iterator++;
			}

			if(Local_u8PressedKey == '*' || Local_u8PressedKey =='/' || Local_u8PressedKey == '+' ||
					Local_u8PressedKey == '-' || Local_u8PressedKey == '=')
			{
				break ;
			}
		}

	}

	/* Make the Value integer number to make operations */
	u8 j = 0 ;
	while(j != Local_Iterator-1 )
	{
		Local_u16Value *= 10 ;
		Local_u16Value = Local_u16Value + (u16)(Local_AValues[j] -'0');
		j++;
	}

	*Copy_pu8Opertor = Local_AValues[j];

	return Local_u16Value ;
}
u16 ResultOperationFunction ( u16 Copy_u16Op1 , u16 Copy_u16Op2 , u8 Copy_u8Operator )
{
	u16 LOC_result = 0 ;
	switch(Copy_u8Operator)
	{
	case '+' : LOC_result = Copy_u16Op1 + Copy_u16Op2 ;
	break;
	case '-':	LOC_result = Copy_u16Op1 - Copy_u16Op2 ;
	break;
	case '*':	LOC_result = Copy_u16Op1* Copy_u16Op2 ;
	break ;
	case '/':	LOC_result = Copy_u16Op1 / Copy_u16Op2 ;
	break;
	default : break;
	}

	return LOC_result ;
}
