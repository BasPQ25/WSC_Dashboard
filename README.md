## DE TESTAT MODULE:
	DISPLAY:	
		BOOT DISPLAY *****MERGEEEE*****
		MAIN DISPLAY *****MERGEEEE*****
		MPPT DISPLAY
		ERROR DISPLAY-> CEL ASEMANATOR CU CEL DE BOOT INSA APARE SWO SI CAN ERRORS.
	
	BUTTONS:
		DE TESTAT BUTOANE PANEL
		DE TESTAT BUTOANE VOLAN
	
	CRUISE:
		DE TESTAT CRUISE CONTROL PROHELION
		DE TESTAT CRUISE CU PID
		DE TESTAT SOLAR ONLY CONTROL
	
	AUXILIARY:
		TESTAT DACA TOATE LUMINILE MERG CONFORM 
	
	TELEMETRY:
		TESTAT DACA DATELE SE TRIMIT CUM TREBUIE
		
		
		
ADRESE BUTOANE VOLAN:

A0 in dreapta de tot, A1 mijloc, A2 stanga

	000 - DISPLAY_SWITCH D0
	001 - SIGN_RIGHT     D1
	010 - SIGN_LEFT      D2
	011 - BRAKE SWAP     D3
	100 - CRUISE_DOWN    D4
	101 - CRUISE_UP      D5
	110 - CRUISE ON      D6
	111 - AVARIE         D7 
	
	
	#idei care nu mergeau:
		Atentie la timeout-urile de la comunicatii produse de task-uri prea indelungate..
		