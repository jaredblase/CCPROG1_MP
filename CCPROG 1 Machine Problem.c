/*****************************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in
studying and applying the concepts learned. I have constructed the functions and their respective algorithms 
and corresponding code by myself. The program was run, tested, and debugged by my own efforts. I further certify 
that I have not copied in part or whole or otherwise plagiarized the work of other students and/or persons.

Jared Blase Del Castillo Sy, DLSU ID# 11929200
******************************************************************************************************************/
/*
	Description: The POS asks the user for the desired size, neckline, quantity, and add-ons of the t-shirt.
	Due to the company catering to the customization of the t-shirt, there should be at least 1 add-on and a minimum of
	5 t-shirts per order. Once the ordering phase is done, the program will tell the customer when pickup is available then
	save the order for tracking.
	
	Programmed by: Jared Blase Del Castillo Sy		S-16
	Last modified: November 22, 2019
	Version: 4.0
	[Acknowledgements: stackoverflow.com]
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define HOUR 3600

void addMore(char, char*),
	 addOns(char*),
	 breakdown(float),
	 checkForPickups(int),
	 computeAndDisplayPrice(int, char, float*, char*),
     makeShirt(char, char, char*, char(*)[17]),
     makeShirtColumn(int, int, int, int, char, char(*)[17]),
	 order(int, int*, int*, int*, float*, int),
	 paymentAndChange(float),
	 placement(char, char*),
	 printDash(),
	 printIntro(),
	 printPosition(int),
	 saveOrder(int, int, char, char, int*);
     
char checkYorN(),
	 chooseAddOn(),
	 chooseNeck(),
	 chooseSize(),
	 logoColor(),
	 printMainMenu(),
	 printOrderMenu();

int checkPosition(char, int, char*),
	checkLayout(char, char*),
    chooseQuantity(),
	computeAndDisplayTime(int, char*, int*, int),
    printReceipt(int, char, char, float*, char*, char(*)[17], int*, int);

int main()
{
	char cSelected_Opt;
	int nDay = 1,
	    nOrderDay = 0,
	    nDay_Quantity = 0,
	    nOrder_Number = 1,
	    nTime_Left = 0;
	float fDay_Revenue = 0;    
	FILE *fpStatus = fopen ("Status.txt", "r");
	
	/* Creates a Orders.txt if it doesn't exist
	   but reads it otherwise. */
	if (fpStatus == NULL)
	{
		fpStatus = fopen ("Status.txt", "w");
		fprintf (fpStatus, "Day: 1\n");
		fprintf (fpStatus, "Workload left: 0\n");
		fprintf (fpStatus, "Current order number: 1\n");
		fprintf (fpStatus, "Total number of shirts ordered: 0\n");
		fprintf (fpStatus, "Total amount received: 0\n\n");
	}
	else
	{
		fscanf (fpStatus, "%*s %d", &nDay);
		fscanf (fpStatus, "%*s %*s %d", &nTime_Left);
		fscanf (fpStatus, "%*s %*s %*s %d", &nOrder_Number);
		fscanf (fpStatus, "%*s %*s %*s %*s %*s %d", &nDay_Quantity);
		fscanf (fpStatus, "%*s %*s %*s %f", &fDay_Revenue);
	}
	fclose (fpStatus);
	
	printIntro();
	printf ("Day %d", nDay);
	do
	{
		cSelected_Opt = printMainMenu();
		switch (cSelected_Opt)
		{
			case '1':
				order(nDay, &nDay_Quantity, &nTime_Left, 
				      &nOrder_Number, &fDay_Revenue, nDay == nOrderDay);
				nOrderDay = nDay;
				
				/* Update the Orders.txt file */
				fopen ("Status.txt", "r+");
				fprintf (fpStatus, "Day: %d\n", nDay);
				fprintf (fpStatus, "Workload left: %d\n", nTime_Left);
				fprintf (fpStatus, "Current order number: %d\n", nOrder_Number);
				fprintf (fpStatus, "Total number of shirts ordered: %d\n", nDay_Quantity);
				fprintf (fpStatus, "Total amount received: %f\n\n", fDay_Revenue);
				fclose (fpStatus);
				break;
			
			case '2':
				printDash();
				printf ("Order Summary for the day:\n");
				printf ("Total number of shirts ordered: %d\n", nDay_Quantity);
				printf ("Total amount received: P %.2f\n", fDay_Revenue);
				printDash();
				break;
				
			case '3':
				/* Refresh for the day */
				nDay_Quantity = fDay_Revenue = 0;
				nDay++;
				nTime_Left -= 12 * HOUR;
				
				if (nTime_Left < 0)
					nTime_Left = 0;
				
				system ("cls");
				printf ("Starting new day...\n");
				printf ("\nDay %d\n", nDay);
				checkForPickups(nDay);
				
				/* Update Orders.txt */
				fopen ("Status.txt", "r+");
				fprintf (fpStatus, "Day: %d\n", nDay);
				fclose (fpStatus);
				break;
		}
	} while (cSelected_Opt != '4');
	
	printf ("\nProgram terminating for maintenance...");
	
	return 0;
}

/* Displays the introduction of the program. */
void
printIntro()
{
	printDash();
	printf ("Hello and welcome to Team Spirit Personalized Shirts Inc.\n");
	printf ("This is an intuitive purchase order system.\n");
	printf ("Please use the Menu to navigate and Y or N to signify yes or no, respectively.\n");
	printf ("Thank you!\n");
	printDash();
}

/* Displays dashes when called */
void
printDash()
{
	printf ("-------------------------------------------------------------------------------------------\n");
}

/* This displays the main menu and asks the user to choose
   a menu option. It will loop if the input is invalid.
   Precondition: The user inputs a single character.
   @return the selected menu option of the user.
*/
char
printMainMenu()
{
	char cAns;

	printf ("\n|-----------------------------------|\n");
	printf ("|             Main Menu             |\n");
	printf ("|1 - Order                          |\n");
	printf ("|2 - Show order summary for the day |\n");
	printf ("|3 - Start new day                  |\n");
	printf ("|4 - Shutdown for maintenance       |\n");
	printf ("|-----------------------------------|\n");
	do {
		printf ("\nEnter option: ");
		scanf ("%c%*c", &cAns);
		if (cAns != '1' && cAns != '2' && cAns != '3' && cAns != '4') {
			printf ("Invalid choice.\n");
			cAns = 0;
		}
	} while (cAns == 0);
	
	return cAns;	
}

/* Main function for ordering
   @param pDay_Quantity is the address of the total shirts sold for the day.
   @param pDay_Revenue is the address of the total revenue for the day.
   @param nDay is the current day of ordering.
   @param pTime_Left is the address of the time left to process the previous orders.
   @param pOrder_Number is the address of the order number to be assigned to the current order.
*/
void
order(int nDay, int *pDay_Quantity, int *pTime_Left, int *pOrder_Number, float *pDay_Revenue, int sameDayOrder)
{
	int nQuantity, i,
	    nDays_Until_Pickup;
	    
	char cNeck,
	     cSize,
	     cSelected_Opt,
	     aAdd_Layout[6],
	     aShirt[31][17];
	     
	float fTotal_Price;
	
	nQuantity = cNeck = cSize = 0;     
	do
	{
		/* refresh the contents of the layout */
		for (i = 0; i <= 5; i++)
			aAdd_Layout[i] = '\0';
	
		do  /* Loops until the user checks out */
		{
			cSelected_Opt = printOrderMenu();
			switch (cSelected_Opt)
			{
				case '1':
					cSize = chooseSize();
					printf ("Size chosen!\n");
					break;
				
				case '2':
					nQuantity = chooseQuantity();
					printf ("Quantity entered!\n");
					break;
				
				case '3':
					cNeck = chooseNeck();
					printf ("Neckline chosen!\n");
					break;
				
				case '4':
					addOns(aAdd_Layout);
					break;
					
				case '5':
					if (cSize == 0)
					{
						printf ("You have not chosen a size yet.\n");
						cSelected_Opt = 0;
					}
					
					if (nQuantity == 0)
					{
						printf ("You have not entered a quantity yet.\n");
						cSelected_Opt = 0;
					}
					
					if (cNeck == 0)
					{
						printf ("You have not chosen a neckline yet.\n");
						cSelected_Opt = 0;
					}
					
					if (checkLayout('A', aAdd_Layout) == 6)
					{
						printf ("You have not placed an add-on yet.\n");
						cSelected_Opt = 0;
					}
					break;
			}
		} while (cSelected_Opt != '5');

		nDays_Until_Pickup = printReceipt(nQuantity, cSize, cNeck, &fTotal_Price,
		                                  aAdd_Layout, aShirt, pTime_Left, sameDayOrder);
		                                  
		printf ("\nNote that inputting 'N' will refresh the add-on, but everything else will be stored and can still be changed\n");
		cSelected_Opt = checkYorN("Confirm order");
	} while (cSelected_Opt != 'Y');
	
	paymentAndChange(fTotal_Price);
	saveOrder(nQuantity, nDay + nDays_Until_Pickup,
	          cSize, cNeck, pOrder_Number);
	printf ("\nYour order has been placed! Thank you.\n");
	
	*pDay_Quantity += nQuantity;
	*pDay_Revenue += fTotal_Price;
}

/* This displays the order menu and asks the user to choose
   a menu option. It will loop if the input is invalid.
   Precondition: The user inputs a single character.
   @return the selected order option of the user.
*/
char
printOrderMenu()
{
	char cAns;
	
	printf ("\n|--------------|\n");
	printf ("|  Order Form  |\n");
	printf ("|1 - Size      |\n");
	printf ("|2 - Quantity  |\n");
	printf ("|3 - Neckline  |\n");
	printf ("|4 - Add-Ons   |\n");
	printf ("|5 - Check out |\n");
	printf ("|--------------|\n");
	
	do
	{
		printf ("\nEnter option: ");
		scanf ("%c%*c", &cAns);
		if (cAns != '1' && cAns != '2' && cAns != '3' && cAns != '4'&& cAns !='5')
		{
			printf ("Invalid choice.\n");
			cAns = 0;
		}
	} while (cAns == 0);
	
	return cAns;	
}

/* Displays the options for shirt sizes and takes the first letter 
   of the desired size.
   Precondition: The input is a single character.
   @return the capitalized letter of the desired size
*/
char
chooseSize()
{
	char cAns;
	
	printf ("|-----------------------|\n");
	printf ("|      Size Options     |\n");
	printf ("|S - Small              |\n");
	printf ("|M - Medium             |\n");
	printf ("|L - Large              |\n");
	printf ("|X - Extra Large (+10%%) |\n");
	printf ("|-----------------------|\n");
	
	/* Asks the user for the shirt size. If invalid input, this block loops 
	   and print "Invalid size." */
	do
	{
		printf ("\nEnter Size: ");
		scanf ("%c%*c", &cAns);
		cAns = toupper (cAns); /* Capitalizes the letter entered */
		
		if (cAns != 'S' && cAns != 'M' && cAns != 'L' && cAns != 'X')
		{
			printf ("Invalid size.\n");
			cAns = 0;
		}
	} while (cAns == 0);
	
	return cAns;
}

/*  Asks for the quantity of shirts.
    Precondition: The input is a whole number.
    @return the quantity of shirts
*/
int
chooseQuantity()
{
	int nAns;
	
	/* Asks the user for the quantity. If invalid input, this block loops 
	   and print "Invalid number of pieces" */
	do {
		printf ("\nEnter the number of pieces (at least 5): ");
		scanf ("%d%*c", &nAns);
		if (nAns < 0)
			printf ("Invalid number of pieces.\n");
		else if (nAns < 5)
			printf ("Too few. Invalid number of pieces.\n");
	} while (nAns < 5);
	
	return nAns;
}

/*  Displays the options for neckline types and takes the first letter 
    of the desired choice.
    Precondition: The user inputs a single character.
    @return the capitalized letter of the desired neckline
*/
char
chooseNeck()
{
	char cAns;
	
	printf ("\n|-----------------|\n");
	printf ("|Neckline options:|\n");
	printf ("|V - V-Neck       |\n");
	printf ("|R - Round-neck   |\n");
	printf ("|-----------------|\n");
	
	/* Asks the user for the choice of neckline. If invalid input, this block loops */
	do
	{
		printf ("\nEnter choice of neckline: ");
		scanf ("%c%*c", &cAns);
		cAns = toupper (cAns); /* capitalizes the input */
		
		if (cAns != 'V' && cAns != 'R')
		{
			printf ("Invalid neckline\n");
			cAns = 0;
		}
	} while (cAns == 0);
	
	return cAns;
}

/* Main function of the Add-Ons part
   @param pLayout is the string which contains the add-ons and their positions.
*/
void
addOns(char *pAdd_Layout)
{
	char cAns = '\0',
	     cSelected_Add = '\0';
	
	/* This statement will loop until there is no more available slot or
	   if the user chooses to stop adding more. */
	while (cAns != 'N' && checkLayout('A', pAdd_Layout) != 0)
	{
		cSelected_Add = chooseAddOn();
		addMore(cSelected_Add, pAdd_Layout);
		cAns = checkYorN("Would you like another add-on");
	}
	
	/* Runs when all the spaces are occupied */ 
	if (checkLayout('A', pAdd_Layout) == 0)
		printf ("Sorry! You have already taken up all the available spaces for all of the designs.\n");
}

/* Displays the add-on options and checks the user's input
   and will loop if the input is invalid.
   Precondition: The user inputs a single character.
   @return the capitalized letter of the add-on of choice.
*/
char
chooseAddOn()
{
	char cAns;
	
	/* Display options */
	printf ("\n|---------------|\n");
	printf ("|Add-on Options:|\n");
	printf ("|L - Logo       |\n");
	printf ("|P - Patch      |\n");
	printf ("|O - pOcket     |\n");
	printf ("|---------------|\n");
		
	/* This block will loop if input is invalid. */
	do 
	{
		printf ("\nEnter add-on: ");
		scanf ("%c%*c", &cAns);
		cAns = toupper (cAns); /* capitalilzes the input */
		
		if (cAns != 'L' && cAns != 'P' && cAns != 'O')
		{
			printf ("Invalid choice.\n");
			cAns = 0;
		}
	} while (cAns == 0);
	
	return cAns;
}

/* This will decide what to do based on the user's input and calls placement() for placement of the add-on
   This will loop if the user wants to keep adding the same type of design.
   It will, however, stop once all spaces for the desired add-on have been occupied.
   @param cAdd contains the letter of the desired add-on.
   @param pAdd_Layout is the address of the string which contains the add-ons and their positions.
*/
void
addMore(char cAdd, char *pLayout)
{
	char cAns = '\0',
	     cLogo;
	
	while (cAns != 'N' && checkLayout(cAdd, pLayout) != 0)
	{
		switch (cAdd) {
			case 'L':
				cLogo = logoColor();
				placement(cLogo, pLayout);
				cAns = checkYorN("Add another logo");
				break;
				
			case 'P':
				placement('P', pLayout);
				cAns = checkYorN("Add another patch");
				break;
				
			case 'O':
				placement('O', pLayout);
				cAns = checkYorN("Add another pocket");
				break;
		}
	}
	
	/* Runs when all the spaces for the particular design are occupied */
	if (checkLayout(cAdd, pLayout) == 0)
		printf ("Sorry! You have already taken up all the available spaces for this design.\n");
}

/* Checks if there are still available slots for add-ons by scanning each character of the string.
   This will decide which positions to check for based on the parameter.
   @param cSelectedOpt is the letter of the add-on the user wants to add.
   @param pLayout is the address of the string containing the layout.
   @return 1 or 0 to signify true or false, respectively.
*/
int
checkLayout(char cSelectedOpt, char *pAdd_Layout)
{
	int i,
		start = 0,
	    limit = 6,
	    space = 0;
	
	/* If the cSelectedOpt passed is not any of L, P, or O,
	   It will automatically check all of the spaces. */
	switch (cSelectedOpt)
	{
		case 'L':
		case 'P':
			start = 0;
			limit = 4;
			break;
	
		case 'O':
			start = 2;
			limit = 6;
			break;
	}
	
	/* Check the specified range in the string. And for every space
	   the variable 'space' adds by 1.*/
	for (i = start; i < limit; i++)
	{
		if (pAdd_Layout[i] == '\0')
			space++;	
	}
	
	return space;
}

/* Takes the input of the user that indicates the number of colors in the logo 
   and will loop if the input is invalid.
   Precondition: The user inputs a single character. 
   @return the number of color the logo has
*/
char
logoColor()
{
	char cColors;
	
	do
	{
		printf ("\nHow many colors does the logo have (1-5): ");
		scanf ("%c%*c", &cColors);
		if ( cColors != '1' && cColors != '2' && cColors != '3' && cColors != '4' && cColors != '5')
		{
			printf ("Invalid choice.\n");
			cColors = 0;
		}
	} while (cColors == 0);
	
	return cColors;
}

/* Displays the placement options to the user and will attempt to place the
   desired add-on to the desired position.
   Precondition: The user inputs an integer.
   @param cAdd is the letter of the wanted add-on.
   @param pAdd_Layout is the address of the string which contains the add-ons and their positions.
*/
void
placement(char cAdd, char *pAdd_Layout)
{
	int nAns;
	
	switch (cAdd) {	
	case 'P':
		printf ("\nPatch");
		break;
	
	case 'O':
		printf ("\nPocket");
		break;
		
	default:
		printf ("\nLogo");
		break;
	}
	
	printf (" placement options:\n");

	if (cAdd == 'O') /* Since pockets can only be placed on the bottom left and right */
	{
		printf ("1 - Bottom Right\n");
		printf ("2 - Bottom Left\n");
	}
	else 
	{
		printf ("1 - Right Sleeve\n");
		printf ("2 - Left Sleeve\n");
	}
	printf ("3 - Right Chest\n");
	printf ("4 - Left Chest\n");
	
	/* Loops if invalid input or if the space is already occupied. */
	do
	{
		switch (cAdd)
		{
			case 'P':
				printf ("\nEnter patch placement: ");
				break;
		
			case 'O':
				printf ("\nEnter pocket placement: ");
				break;
			
			default:	
				printf ("\nEnter logo placement: ");
				break;
		}	
		scanf ("%d%*c", &nAns);
		if ( nAns != 1 && nAns != 2 && nAns != 3 && nAns != 4)
		{
			printf ("Invalid choice.\n");
			nAns = 0;
		}
		else 
		{
			nAns = checkPosition(cAdd, nAns, pAdd_Layout);
		}
	} while (nAns == 0);
	
	pAdd_Layout[nAns - 1] = cAdd;
	printf ("Add-on placed!\n");
}

/* This checks to see if the desired position is occupied.
   @param cAdd is the letter of the desired add on.
   @param nPosition is the desired placement of the add on. (relative to the string)
   @param pAdd_Layout is the address of the sLayout string
   @return 0 or other values to signify false or true, respectively.
*/
int
checkPosition(char cAdd, int nPosition, char *pAdd_Layout)
{	
	/* Since the 1st and 2nd options in the placement of pockets is different from the other two add-ons */
	if (cAdd == 'O' && nPosition <= 2)
		nPosition += 4;
	
	/* This block will test to see if the desired placement is already occupied */
	if (pAdd_Layout[nPosition - 1] == '\0')
	{
		return nPosition;
	}
	else
	{
		printf ("Invalid. Already indicated to place a ");
		switch (pAdd_Layout[nPosition - 1])
		{
			case 'P':
				printf ("patch.\n");
				break;
				
			case 'O':
				printf ("pocket.\n");
				break;
			
			default:
				printf ("logo.\n");
				break;
		}
		return 0;
	}
}

/* Checks input for yes or no questions, loops if invalid statement.
   Precondition: The user inputs a single character.
   @param question is the question to be asked.
   @return the capitalized Y or N.
*/
char
checkYorN(char question[])
{
	char cAns;
	
	do
	{
		printf ("\n%s? (Y/N) ", question);
		scanf ("%c%*c", &cAns); /* receives the input */
		cAns = toupper (cAns); /* capitalizes the input */
		
		if (cAns != 'N' && cAns != 'Y')
			printf ("Invalid answer.\n");
	
	} while (cAns != 'N' && cAns != 'Y');
	
	return cAns;
}

/* Prints the receipt
   @param nQuantity is the quantity of shirts ordered.
   @param cSize is the letter of the chosen size.
   @param cNeck is the letter of the chosen neckline.
   @param pTotal_Price is the address of the total price for the order.
   @param pAdd_Layout is the address of the sLayout string.
   @param pShirt is the address of the aShirt 2-D array.
   @param pTime_Left is the address of the variable for time remaining to finish the previous orders.
   @return the number of days to produce the order.
*/
int
printReceipt(int nQuantity, char cSize, char cNeck, float *pTotal_Price, 
             char *pAdd_Layout, char pShirt[][17], int *pTime_Left, int sameDayOrder)
{
	printf ("\nOrder Summary:\n");
	printf ("%10d pcs of ", nQuantity);
	
	/* Prints the whole word of size based on the letter input */
	switch (cSize)
	{
		case 'S':
			printf ("Small ");
			break;
				
		case 'M':
			printf ("Medium ");
			break;
		
		case 'L':
			printf ("Large ");
			break;
			
		case 'X':
			printf ("Extra Large ");
			break;
	}
	
	/* Prints the whole word of the neckline based on the letter input */
	switch (cNeck)
	{
		case 'V':
			printf ("V");
			break;
			
		case 'R':
			printf ("Round");
			break;
	}
	printf ("-neck shirt with");
	
	computeAndDisplayPrice(nQuantity, cSize, pTotal_Price, pAdd_Layout);
	makeShirt(cSize, cNeck, pAdd_Layout, pShirt);
	
	return computeAndDisplayTime(nQuantity, pAdd_Layout, pTime_Left, sameDayOrder);
}

/* Computes and displays the price of each add on.
   @param nQuan is the quantity of shirts ordered.
   @param cSize is the letter of the chosen size.
   @param pTotalPrice is the address of the total price.
   @param pLayout is the address of the sLayout string.
*/
void
computeAndDisplayPrice(int nQuantity, char cSize, float *pTotal_Price, char *pAdd_Layout)
{
	int i;
	
	float fShirtPrice,
	      fLogo1Price = 18,
	      fLogo2Price = 20,
	      fLogo3Price = 25,
	      fPatchPrice = 15,
	      fPocketPrice = 5,
	      fLogoDisc = 1,
	      fAddOnTotal = 0;
	
	/* Assignment of unit price based on the input number of shirts
	   and logo discount */ 	
	if (nQuantity > 500)
	{
		fShirtPrice = 75;
		fLogoDisc *= 0.75;
	}	
	else if (nQuantity >= 300)
	{
		fShirtPrice = 80;
		fLogoDisc *= 0.9;
	}
	else
	{
		fShirtPrice = 100;
	}
		
	/* Adds 10% to shirt price if shirt size is XL */
	if (cSize == 'X')
		fShirtPrice *= 1.10;
		
	printf("\t\tP %12.2f @\n", fShirtPrice);
	
	/* Checks every character in the array for a value. If a value is present,
	   only then will it output */
	for (i = 0; i < 6; i++) {
		if (pAdd_Layout[i] != '\0')
		{
			printf ("                  ");
			switch (pAdd_Layout[i])
			{
				case '1':
					printf ("Logo ");
					printPosition(i);
					printf (", %c color\t\tP %12.2f @", pAdd_Layout[i], fLogo1Price * fLogoDisc);
					fAddOnTotal += fLogo1Price * fLogoDisc;
					pAdd_Layout[i] = 'L';
					break;
				
				case '2':
					printf ("Logo ");
					printPosition(i);
					printf (", %c color\t\tP %12.2f @", pAdd_Layout[i], fLogo2Price * fLogoDisc);
					fAddOnTotal += fLogo2Price * fLogoDisc;
					pAdd_Layout[i] = 'L';
					break;
				
				case '3':
				case '4':
				case '5':
					printf ("Logo ");
					printPosition(i);
					printf (", %c color\t\tP %12.2f @", pAdd_Layout[i], fLogo3Price * fLogoDisc);
					fAddOnTotal += fLogo3Price * fLogoDisc;
					pAdd_Layout[i]= 'L';
					break;
				
				case 'P':
					printf ("Patch ");
					printPosition(i);
					if (i == 3 || i == 4)
						printf ("\t");
					printf ("  \t\tP %12.2f @", fPatchPrice);
					fAddOnTotal += fPatchPrice;
					break;
					
				case 'O':
					printf ("Pocket ");
					printPosition(i);
					if (i == 2  || i == 3)
						printf ("\t");
					printf ("\t\tP %12.2f @", fPocketPrice);
					fAddOnTotal += fPocketPrice;
					break;
			}
		printf ("\n");
		}
	}
	printDash();
	printf ("\t\t\t\t\t\t\tP %12.2f @ x %d\n", fAddOnTotal + fShirtPrice, nQuantity);
	*pTotal_Price = (fAddOnTotal + fShirtPrice) * nQuantity;
	printf ("\t\t\t\tTotal Bill for Order\tP %12.2f\n\n", (fAddOnTotal + fShirtPrice) * nQuantity);
}

/* Outputs the mock t-shirt by determining the dimensions of the shirt
   and loops while calling printShirtColumn() for each column in a row
   @param cSize is the letter to represent the size of the shirt.
   @param cNeck is the letter to represent the neckline of the shirt.
   @param pLayout is the address of the sLayout string.
   @param pShirt is the address of the aShirt 2-D array.
*/
void
makeShirt(char cSize, char cNeck, char *pAdd_Layout, char pShirt[][17])
{
	int nLength = 10, /* The length, width, and shoulder width start with the specs of small t-shirt */
	    nWidth = 18,
	    nShoulder = 4,
	    nMultiplier = 0,
	    row, /* variable for the row of the shirt */
	    i,j; 
	
	/* Increases the multiplier based on the size of the shirt */    
	switch (cSize)
	{
		case 'X':
			nMultiplier++;
			
		case 'L':
			nMultiplier++;
			
		case 'M':
			nMultiplier++;
			break;
	}
	
	/* Increases the length and width based on the multiplier since for every change in size,
	   the length increases by 2, and the width by 4. */
	nLength += 2 * nMultiplier;
	nWidth += 4 * nMultiplier;
	nShoulder+= 2 * nMultiplier;
	
	printf ("Mock Shirt:\n");
	
	/* Stores the undesigned shirt into the 2D array */
	for (row = 1; row <= nLength; row++)
	{
		makeShirtColumn(row, nLength, nWidth, nShoulder, cNeck, pShirt);
	}

	/* Insert the designs */
	pShirt[4][3] = pAdd_Layout[0];
	pShirt[nWidth - 3][3] = pAdd_Layout[1];
	pShirt[6][4] = pAdd_Layout[2];
	pShirt[nWidth - 5][4] = pAdd_Layout[3];
	pShirt[6][nLength - 1] = pAdd_Layout[4];
	pShirt[nWidth - 5][nLength - 1] = pAdd_Layout[5];
	
	/* Print the mock t-shirt */
	for (i = 1; i <= nLength; i++)
	{
		for (j = 1; j <= nWidth; j++)
			printf ("%c", pShirt[j][i]);
		printf ("\n");
	}
	printf ("\n");
}

/* Prints each column differently depending on the row number.
   @param row is the current row number.
   @param nLength is the height of the shirt.
   @param nWidth is the width of the shirt.
   @param nShoulder is the shoulder width of the shirt.
   @param cNeck is the letter of the chosen neckline of the shirt.
   @param pShirt is the address of the aShirt 2-D array.
*/
void
makeShirtColumn(int row, int nLength, int nWidth, int nShoulder, char cNeck, char pShirt[][17])
{
	int col; /*variable for column of the shirt */

	for (col = 1; col <= nWidth; col++) {
		switch (row) /* This switch case is for the first 4 rows of the shirt where the sleeves are. */
		{
			case 1: 
				if ((col >= 4 && col < 4 + nShoulder) || (col >= 8 + nShoulder && col <= nWidth - 3))
					pShirt[col][row] = 'X';
				else
					pShirt[col][row] = ' ';
				break;
				
			case 2:
				if (cNeck == 'R')
				{
					if (col == 3 || col == nWidth - 2 || (col >= 4 + nShoulder && col < nShoulder + 8))
						pShirt[col][row] = 'X';
					else
						pShirt[col][row] = ' ';
				}
				else
				{
					if (col == 3 || col == nWidth - 2 || col == 4 + nShoulder || col == 7 + nShoulder)
						pShirt[col][row] = 'X';
					else
						pShirt[col][row] = ' ';
				}
				break;
				
			case 3:
				if (cNeck == 'R')
				{
					if (col == 2 || col == nWidth - 1)
						pShirt[col][row] = 'X';
					else
						pShirt[col][row] = ' ';
				}
				else
				{
					if (col == 2 || col == nWidth - 1 || col == 5 + nShoulder || col == 6 + nShoulder)
						pShirt[col][row] = 'X';
					else
						pShirt[col][row] = ' ';
				}
				break;
				
			case 4:
				if (col <= 4 || col > nWidth - 4)
					pShirt[col][row] = 'X';
				else
					pShirt[col][row] = ' ';
				break;
		}
			
		/* This is for the lower part of the shirt */
		if (row > 4 && row < nLength)
		{
			if (col == 4 || col == nWidth - 3)
				pShirt[col][row] = 'X';
			else
				pShirt[col][row] = ' ';
		}
		/* This is for the last row of the shirt */
		else if (row == nLength)
		{
			if (col >= 4 && col <= nWidth - 3)
				pShirt[col][row] = 'X';
			else
				pShirt[col][row] = ' ';	
		}
	}
}

/* Prints the text of the position of each design on the receipt
   based on its order in the sLayout string.
   @param nPosition is the number that signifies the current position in the sLayout string.
*/
void
printPosition(int nPosition)
{
	switch (nPosition)
	{
		case 0:
			printf ("on right sleeve");
			break;
			
		case 1:
			printf ("on left sleeve");
			break;
			
		case 2:
			printf ("on right chest");
			break;
			
		case 3:
			printf ("on left chest");
			break;
		
		case 4:
			printf ("on bottom right");
			break;
		
		case 5:
			printf ("on bottom left\t");
			break;
	}
}

/* Computes for the production time and outputs both the time and the pickup date.
   @param nQuan is the quantity of shirts ordered.
   @param pLayout is the address of the sLayout string.
   @param pTimeLeft is the address of the variable for time remaining to finish the previous orders.
   @return the number of days to produce the order.
*/
int
computeAndDisplayTime(int nQuantity, char *pAdd_Layout, int *pTime_Left, int sameDayOrder)
{
	int i,
	    nTime = 0,
	    nDays = 0;
	
	/* counts the number of each kind of add-on, and adds the corresponding time
	   for each shirt in seconds */
	for (i = 0; i < 6; i++)
	{
		switch (pAdd_Layout[i])
		{
			case 'L':
				nTime += 60;
				break;
				
			case 'P':
				nTime += 20;
				break;
				
			case 'O':
				nTime += 30;
				break;
		}
	}
	
	/* The time for each shirt multiplied by the quantity for the total time */
	nTime *= nQuantity;
	
	/* Add the time to time left. The time, in seconds, is divided by 12 hours since the machine only works 12 hours a day, and
	   added by 1 since the production will only start the day after ordering, but if the time left to work is greater than
	   12 hours, there is no need to add another day. */
	if (sameDayOrder || *pTime_Left > 12 * HOUR)
		*pTime_Left += nTime;
	else
		*pTime_Left = nTime + 12 * HOUR;
	
	nDays = *pTime_Left / (12 * HOUR);
	
	/* If there is a remainder, then the day will be added by 1 since the customer can only pick it up
	   the day after production finishes */
	if (nTime % (3600 * 12) != 0)
		nDays++;
	
	printf ("Production Time: ");
	if (nTime / 3600 > 0)
		printf ("%d hours ", nTime / 3600);
	nTime %= 3600;
	
	if (nTime / 60 > 0)
		printf ("%d minutes ", nTime / 60);
		
	if (nTime % 60 > 0)	
		printf ("%d seconds", nTime % 60);
	printf ("\n");
	
	printf ("Order can be picked up after %d days\n", nDays);
	printf ("Production may take long than usual due to the stacking of previous orders.\n");
	
	return nDays;
}

/* Asks the user for the payment. If the input is less that the total price,
   the function loops.
   Precondition: The user's input is a number.
   @param fTotal_Price is the total price of the order.
*/
void
paymentAndChange(float fTotal_Price)
{
	float fCash,
	      fChange;
	
	do
	{
		printf ("\nAmount paid by the customer: ");
		scanf ("%f%*c", &fCash);
		if (fCash < 0)
			printf ("Amount entered is invalid.\n");
		else if (fCash < fTotal_Price)
			printf ("Amount entered is less than the total price.\n");
	} while (fCash < fTotal_Price);
	
	fChange = fCash - fTotal_Price;
	printf ("\nTotal change: P %.2f\n", fChange);
	
	if (fChange != 0)
	{
		printf ("Breakdown of change:\n");
		breakdown(fChange);
	}
	else
		printf ("Thank you for paying the exact amount!\n");
}

/* This breaks down the change into smaller denominations
   @param fChange is the change of the user.
*/
void
breakdown(float fChange)
{
	float fDenomination[13] = {1000, 500, 200, 100, 50, 20, 10, 5, 1, 0.5, 0.25, 0.1, 0.05};
	
	int nDenomination[13] = {100000, 50000, 20000, 10000, 5000, 2000, 1000, 500, 100, 50, 25, 10, 5},
	    nChange = fChange * 100 + 0.5, /* multiplied by 100 to remove the decimal place, and added by 0.5 for rounding off */
	    i;
	
	for (i = 0; i < 13; i++)
	{
		if (nChange / nDenomination[i] != 0) 
		{
			printf ("\tPhp %7.2f - %d\n", fDenomination[i], nChange / nDenomination[i]);
			nChange %= nDenomination[i];
		}
	}
}

/* This saves the order to a text file.
   @param nQuan is the quantity of the shirts.
   @param nPickup_Day is the assigned day of pickup.
   @param cSize is the character of the chosen size.
   @param cNeck is the character of the chosen neckline.
   @param pOrder_Number is the address of the order number that will be assigned to it.
*/
void
saveOrder(int nQuantity, int nPickup_Day, char cSize, char cNeck, int *pOrder_Number)
{
	FILE *pFile = fopen("Orders.txt", "a");
	
	fprintf (pFile, "Order# %d\n", *pOrder_Number);
	fprintf (pFile, "Pickup Day: %d\n", nPickup_Day);
	fprintf (pFile, "%d pcs of ", nQuantity);
	/* Prints the whole word of size based on the letter input */
	switch (cSize)
	{
		case 'S':
			fprintf (pFile, "Small ");
			break;
				
		case 'M':
			fprintf (pFile, "Medium ");
			break;
		
		case 'L':
			fprintf (pFile, "Large ");
			break;
			
		case 'X':
			fprintf (pFile, "Extra Large ");
			break;
	}
	
	/* Prints the whole word of the neckline based on the letter input */
	switch (cNeck)
	{
		case 'V':
			fprintf (pFile, "V");
			break;
			
		case 'R':
			fprintf (pFile, "Round");
			break;
	}
	
	fprintf (pFile, "-neck shirt\n\n");
	fclose (pFile);
	(*pOrder_Number)++;
}

/* This checks if there are available orders for pickup for the day.
   @param nDay is the current day.
*/
void
checkForPickups(int nDay)
{
	char sDescription[60];
	     
	int nPickup_Day = 0,
		nPickups = 0;

	FILE *pFile = fopen("Orders.txt", "r");
	
	printDash();
	
	/* Takes the pickup day of the orders in the Orders.txt file.
	   If the pikcup date is less than the current day, it checks
	   the next order in line until it is greater than the current day. */

	
	while (nPickup_Day <= nDay && getc(pFile) != EOF)
	{
		fscanf (pFile, "%*s %*s\n");
		fscanf (pFile, "%*s %*s %d\n", &nPickup_Day);
		fgets(sDescription, 60, pFile);
		if (nPickup_Day == nDay)
		{
			printf ("%s", sDescription);
			nPickups++;
		}
		fscanf (pFile, "\n\n");
	}
	
	if (nPickups == 0)
		printf ("No pickups for today.\n");
	else
		printf ("\n\n%d order(s) ready for pickup.\n", nPickups);
	
	printDash();
	fclose(pFile);
}
